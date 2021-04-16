#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define MKDIR_SLEEP_TIME 40
#define STR_MAX_LENGTH 75
#define IMAGE_COUNT 10
#define IMAGE_DOWNLOAD_DELAY 5
#define CAESAR_SHIFT 5

int kill_child_immediately = 0;

void checkForCommandLineArgs(int argc, char const *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp("-z", argv[i]) == 0) {
            kill_child_immediately = 1;
        } else if (strcmp("-x", argv[i]) == 0) {
            kill_child_immediately = 0;
        }
    }
}

void makeKiller(pid_t process_id, pid_t session_id) {
    char file_name[] = "killer.sh";
    FILE *killer_file = fopen(file_name, "w");

    fputs("#!bin/bash\n\n", killer_file);

    if (kill_child_immediately) {
        fprintf(killer_file, "pkill -s %d\n", session_id);
    } else {
        fprintf(killer_file, "kill -SIGTERM %d\n", process_id);
    }

    fprintf(killer_file, "rm -f %s\n", file_name);

    fclose(killer_file);
    return;
}

void downloadPicture(char dir_name[STR_MAX_LENGTH]) {
    time_t epoch_seconds = time(NULL);
    char download_url[STR_MAX_LENGTH] = "https://picsum.photos/";
    char picture_size_string[20];
    sprintf(picture_size_string, "%d", epoch_seconds % 1000 + 50);
    strcat(download_url, picture_size_string);

    //debug
    // printf("%s %s", download_url, dir_name);

    struct tm *time = localtime(&epoch_seconds);
    char file_name[STR_MAX_LENGTH];
    strftime(file_name, sizeof(file_name), "%Y-%m-%d_%H:%M:%S", time);

    char download_destination[STR_MAX_LENGTH];
    sprintf(download_destination, "%s/%s", dir_name, file_name);

    //debug
    // printf(" %s\n", download_destination);

    char *argv[] = {"wget", "-qO", download_destination, download_url, NULL};
    execv("/bin/wget", argv);
}

void makeStatus(char dir_name[STR_MAX_LENGTH]) {
    char file_name[STR_MAX_LENGTH];
    sprintf(file_name, "%s/%s", dir_name, "status.txt");
    FILE *status_file = fopen(file_name, "w");

    if (status_file == NULL) {
        exit(EXIT_FAILURE);
    }

    //caesar cipher
    char status[] = "Download Success";
    for (int i = 0; status[i] != '\0'; i++) {
        char character = status[i];
        if (character >= 'a' && character <= 'z') {
            character = character + CAESAR_SHIFT;
            if (character > 'z') {
                character = character - 'z' + 'a' - 1;
            }
            status[i] = character;
        } else if (character >= 'A' && character <= 'Z') {
            character = character + CAESAR_SHIFT;
            if (character > 'Z') {
                character = character - 'Z' + 'A' - 1;
            }
            status[i] = character;
        }
    }

    fputs(status, status_file);

    fclose(status_file);
    return;
}

void archiveDir(char dir_name[STR_MAX_LENGTH]) {
    char zip_name[STR_MAX_LENGTH];
    sprintf(zip_name, "%s.zip", dir_name);
    char *argv[] = {"zip", "-qr", zip_name, dir_name, NULL};
    execv("/bin/zip", argv);
}

void makeDir(char dir_name[STR_MAX_LENGTH]) {
    char *argv[] = {"mkdir", dir_name, NULL};
    execv("/bin/mkdir", argv);
}

void removeDir(char dir_name[STR_MAX_LENGTH]) {
    char *argv[] = {"rm", "-rf", dir_name, NULL};
    execv("/bin/rm", argv);
}

void makeNewDirectoryAndDownloadPictures(char dir_name[STR_MAX_LENGTH]) {
    int child_status;
    pid_t make_dir_child_id;

    make_dir_child_id = fork();

    if (make_dir_child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (make_dir_child_id == 0) {
        makeDir(dir_name);
    }

    while (wait(&child_status) > 0) {
        // tunggu dir selesai dibuat
    }

    //download 10 gambar
    for (int image_count = 0; image_count < IMAGE_COUNT; image_count++) {
        pid_t download_picture_child_id;
        download_picture_child_id = fork();

        if (download_picture_child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (download_picture_child_id == 0) {
            downloadPicture(dir_name);
        }

        sleep(IMAGE_DOWNLOAD_DELAY);
    }

    while (wait(&child_status) > 0) {
        //tunggu download selesai
    }

    makeStatus(dir_name);

    pid_t archive_child_id;

    archive_child_id = fork();

    if (archive_child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (archive_child_id == 0) {
        archiveDir(dir_name);
    } else {
        while (wait(&child_status) > 0) {
            //tunggu selesai archive
        }
        removeDir(dir_name);
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    pid_t main_process_id = getpid(),
          main_session_id = getsid(main_process_id);
    checkForCommandLineArgs(argc, argv);
    makeKiller(main_process_id, main_session_id);

    //main loop
    while (1) {
        pid_t child_id;

        time_t t = time(NULL);
        struct tm *time = localtime(&t);
        char new_dir[STR_MAX_LENGTH];
        strftime(new_dir, sizeof(new_dir), "%Y-%m-%d_%H:%M:%S", time);

        child_id = fork();

        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
            makeNewDirectoryAndDownloadPictures(new_dir);
        } else {
            //parent nggak ngapa ngapain
        }
        sleep(MKDIR_SLEEP_TIME);
    }

    return 0;
}