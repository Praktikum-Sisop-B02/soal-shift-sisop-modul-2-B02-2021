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

#define MKDIR_SLEEP_TIME 5
#define DIR_MAX_LENGTH 50
#define IMAGE_COUNT 10
#define IMAGE_DOWNLOAD_DELAY 1

// void checkForCommandLineArgs(int argc, char const *argv[]) {
//     printf("jalan");
//     return;
//     for (int i = 1; i < argc; i++) {
//         //cek status -x atau -z
//     }
// }

void makeNewDirectory(char dir_name[DIR_MAX_LENGTH]) {
    char *argv[] = {"mkdir", "-p", dir_name, NULL};
    execv("/bin/mkdir", argv);
}

void downloadPictures(char dir_name[DIR_MAX_LENGTH]) {
    //download 10 gambar
    for (int image_count = 0; image_count < IMAGE_COUNT; image_count++) {
        pid_t child_id;
        int child_status;

        child_id = fork();

        if (child_id == 0) {
            char *argv[] = {"wget", "-q", dir_name, NULL};
            execv("/bin/wget", argv);
            printf("anak download gambar ke - %d  %s\n", image_count, dir_name);
            exit(EXIT_SUCCESS);
        }

        sleep(IMAGE_DOWNLOAD_DELAY);
    }

    printf("anak quit\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    // checkForCommandLineArgs(argc, argv);

    //main loop
    while (1) {
        pid_t child_id;
        int child_status;

        time_t t = time(NULL);
        struct tm *time = localtime(&t);
        char new_dir[DIR_MAX_LENGTH];
        strftime(new_dir, sizeof(new_dir), "%Y-%m-%d_%H:%M:%S", time);

        child_id = fork();

        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
            makeNewDirectory(new_dir);
        } else {
            // while (wait(&child_status) > 0) {
            //     //tunggu dir dibuat
            // }

            pid_t download_gambar_child_id;
            download_gambar_child_id = fork();

            if (download_gambar_child_id == 0) {
                downloadPictures(new_dir);
            } else {
                //ga lapo lapo langsugn start timer lagi
            }
        }
        sleep(MKDIR_SLEEP_TIME);
    }

    return 0;
}