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

char path_to_film[] = "~/Documents/SISOPS/Codes/Modul\\ 2/soal-shift-sisop-modul-2-B02-2021/soal1/Fylm",
     path_to_music[] = "~/Documents/SISOPS/Codes/Modul\\ 2/soal-shift-sisop-modul-2-B02-2021/soal1/Musyik",
     path_to_photo[] = "~/Documents/SISOPS/Codes/Modul\\ 2/soal-shift-sisop-modul-2-B02-2021/soal1/Pyoto";

void removeAllDirAndMakeAZip() {
    int child_status;
    pid_t zip_child_id;
    zip_child_id = fork();
    if (zip_child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (zip_child_id == 0) {
        execl("/bin/zip", "zip", "-r", "Lopyu_Stevany.zip", "Fylm", "Pyoto", "Musyik", (char *)NULL);
    } else {
        while (wait(&child_status) > 0) {
        }
        execl("/bin/rm", "rm", "-r", "Fylm", "Pyoto", "Musyik", (char *)NULL);
    }
}

void moveLocation() {
    int child_status;
    pid_t move_child_id;
    move_child_id = fork();
    if (move_child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (move_child_id == 0) {
        execl("/bin/mv", "mv", "FILM", "Fylm", (char *)NULL);
    } else {
        while ((wait(&child_status)) > 0) {
        }
        move_child_id = fork();
        if (move_child_id < 0) {
            exit(EXIT_FAILURE);
        }
        if (move_child_id == 0) {
            execl("/bin/mv", "mv", "MUSIK", "Musyik", (char *)NULL);
        } else {
            while ((wait(&child_status)) > 0) {
            }
            execl("/bin/mv", "mv", "FOTO", "Pyoto", (char *)NULL);
        }
    }
}

void downloadFromGoogleDrives() {
    int child_status;
    pid_t downloads_child_id;
    downloads_child_id = fork();
    if (downloads_child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (downloads_child_id == 0) {
        execl("/bin/wget", "wget", "--no-check-certificate", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "-qO", "Musik_for_Stevany.zip", (char *)NULL);
    } else {
        while ((wait(&child_status)) > 0) {
        }
        downloads_child_id = fork();
        if (downloads_child_id < 0) {
            exit(EXIT_FAILURE);
        }
        if (downloads_child_id == 0) {
            execl("/bin/wget", "wget", "--no-check-certificate", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "-qO", "Film_for_Stevany.zip", (char *)NULL);
        } else {
            while ((wait(&child_status)) > 0) {
            }
            execl("/bin/wget", "wget", "--no-check-certificate", "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "-qO", "Foto_for_Stevany.zip", (char *)NULL);
        }
    }
}

void unzipDownloadedArchive() {
    int child_status;
    pid_t unzip_child_id;
    unzip_child_id = fork();

    if (unzip_child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (unzip_child_id == 0) {
        char *argv[] = {"unzip", "Musik_for_Stevany.zip", NULL};
        execv("/bin/unzip", argv);
    } else {
        while ((wait(&child_status)) > 0) {
        }

        unzip_child_id = fork();

        if (unzip_child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (unzip_child_id == 0) {
            char *argv[] = {"unzip", "Foto_for_Stevany.zip", NULL};
            execv("/bin/unzip", argv);
        } else {
            while ((wait(&child_status)) > 0) {
            }
            char *argv[] = {"unzip", "Film_for_Stevany.zip", NULL};
            execv("/bin/unzip", argv);
        }
    }
}

void downloadAllThreeArchive() {
    int child_status;
    pid_t download_child_id;
    download_child_id = fork();

    if (download_child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (download_child_id == 0) {
        // downloadFromGoogleDrives();
    }
}

int main() {
    pid_t pid, sid; // Variabel untuk menyimpan PID

    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
      * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
      * (nilai variabel pid < 0) */
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
      * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    char time_string[100];
    time_t current = time(NULL);
    struct tm *time_current = localtime(&current);
    strftime(time_string, sizeof(time_string), "%m/%d_%H:%M", time_current);
    while (strcmp(time_string, "04/09_16:22") != 0) {
        sleep(1);
        time_t current = time(NULL);
        struct tm *time_current = localtime(&current);
        strftime(time_string, sizeof(time_string), "%m/%d_%H:%M", time_current);
    }

    int child_status;
    pid_t child_id;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (child_id == 0) {
        downloadAllThreeArchive();
    } else {
        while (wait(&child_status) > 0) {
        }
        child_id = fork();

        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
            unzipDownloadedArchive();
        } else {
            while (wait(&child_status) > 0) {
            }
            child_id = fork();

            if (child_id < 0) {
                exit(EXIT_FAILURE);
            }

            if (child_id == 0) {
                moveLocation();
            } else {
                while (wait(&child_status) > 0) {
                }
                char time_string[100];
                time_t current = time(NULL);
                struct tm *time_current = localtime(&current);
                strftime(time_string, sizeof(time_string), "%m/%d_%H:%M", time_current);
                while (strcmp(time_string, "04/09_22:22") != 0) {
                    sleep(1);
                    time_t current = time(NULL);
                    struct tm *time_current = localtime(&current);
                    strftime(time_string, sizeof(time_string), "%m/%d_%H:%M", time_current);
                }
                child_id = fork();

                if (child_id < 0) {
                    exit(EXIT_FAILURE);
                }

                if (child_id == 0) {
                    removeAllDirAndMakeAZip();
                }
            }
        }
    }
}
