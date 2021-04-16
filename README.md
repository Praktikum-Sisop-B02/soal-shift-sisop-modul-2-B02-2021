# soal-shift-sisop-modul-1-B02-2021

## Soal Nomor 1

## Soal Nomor 2

## Soal Nomor 3

### A. Membuat sebuah program C yang membuat sebuah direktori baru dengan format timestamp YYYY-mm-dd_HH:ii:ss setiap 40 detik

#### Penyelesaian

```C++

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

//...

void makeDir(char dir_name[STR_MAX_LENGTH]) {
    char *argv[] = {"mkdir", dir_name, NULL};
    execv("/bin/mkdir", argv);
}

//...

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

    // ...

    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[]) {
    //...

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
```

#### Penjelasan

Dibuat sebuah main program yang akan selalu melakukan loop untuk membuat child process yang akan membuat sebuah direktori dengan format timestamp dimana timestamp bisa didapatkan dengan menggunakan bantuan `time()` yang kemudian di convert menjadi string dengan bantuan fungsi `strftime()` dan disimapan dalam variabel `new_dir`.

```C++
        time_t t = time(NULL);
        struct tm *time = localtime(&t);
        char new_dir[STR_MAX_LENGTH];
        strftime(new_dir, sizeof(new_dir), "%Y-%m-%d_%H:%M:%S", time);
```

Kemudian program akan melakukan `fork()` untuk membuat sebuah child process yang akan menghandle pembuatan direktori dan juga pendownloadan gambar. Paren tidak akan melakukan apa apa dan akan sleep selama 40 detik. Dan child process akan menjalankan fungsi `makeNewDirectoryAndDownloadPictures()`

```C++
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
```

Child process pun akan membuat sebuah child process lagi untuk melakukan pembuatan direktori menggunakan `execv()`, dan kemudian akan menunggu hingga direktori selesai dibuat.

```C++
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

    // ...

    exit(EXIT_SUCCESS);
}

```

Child process untuk membuat direktori akan menjalankan perintah berikut :

```C++
void makeDir(char dir_name[STR_MAX_LENGTH]) {
    char *argv[] = {"mkdir", dir_name, NULL};
    execv("/bin/mkdir", argv);
}
```

### B. Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

#### Penyelesaian

Dilakukan sedikit perubahan dibandingkan dengan penyelesaian dari nomor A. yaitu

```C++
void downloadPicture(char dir_name[STR_MAX_LENGTH]) {
    time_t epoch_seconds = time(NULL);
    char download_url[STR_MAX_LENGTH] = "https://picsum.photos/";
    char picture_size_string[20];
    sprintf(picture_size_string, "%d", epoch_seconds % 1000 + 50);
    strcat(download_url, picture_size_string);

    struct tm *time = localtime(&epoch_seconds);
    char file_name[STR_MAX_LENGTH];
    strftime(file_name, sizeof(file_name), "%Y-%m-%d_%H:%M:%S", time);

    char download_destination[STR_MAX_LENGTH];
    sprintf(download_destination, "%s/%s", dir_name, file_name);

    char *argv[] = {"wget", "-qO", download_destination, download_url, NULL};
    execv("/bin/wget", argv);
}

//...

void makeNewDirectoryAndDownloadPictures(char dir_name[STR_MAX_LENGTH]) {
    int child_status;

    //membuat direktori

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

    //...

    exit(EXIT_SUCCESS);
}

```

#### Penjelasan

Child process yang tadi nya bertanggung jawab untuk membuat direktori tadi juga bertanggung jawab untuk mengontrol proses download 10 gambar setelah direktori selesai dibuat. Akan dilakukan looping sebanyak 10 kali, dimana pada setiap loop akan dilakukan fork untuk menghasilkan sebuah child process yang bertanggung jawab untuk mendownload 1 gambar. Sebagai parent, tidak akan melakukan apapun dan sleep selama 5 detik. Jika looping sudah selesai parent akan menunggu semua proses download selesai.

```C++
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
```

Child yang bertanggung jawab untuk mendownload 1 gambar akan membuat url request berserta ukuran gambar yang ingin direquest. Untuk mendapatkan epoch unix dilakukan dengan fungsi `time(NULL)` kemudian dilakukan perhitungan dan dihubungkan url yang kemudian bisa digunakan untuk mendownload.

```C++
    time_t epoch_seconds = time(NULL);
    char download_url[STR_MAX_LENGTH] = "https://picsum.photos/";
    char picture_size_string[20];
    sprintf(picture_size_string, "%d", epoch_seconds % 1000 + 50);
    strcat(download_url, picture_size_string);
```

Kemudian akan dibuat terlebih dahulu nama file yang akan digunakan. Yaitu dengan mengubah `time_t` menjadi sebuah `struct tm` sehingga bisa didapatkan data tahun, tanggal, bulan, dan waktu yang diubah menjadi string dengan bantuan `strftime()`. Lalu nama direktori dan nama file akan digabungkan untuk digunakan sebagai argumen tujuan download perintah `wget`.

```c++
    struct tm *time = localtime(&epoch_seconds);
    char file_name[STR_MAX_LENGTH];
    strftime(file_name, sizeof(file_name), "%Y-%m-%d_%H:%M:%S", time);

    char download_destination[STR_MAX_LENGTH];
    sprintf(download_destination, "%s/%s", dir_name, file_name);

    char *argv[] = {"wget", "-qO", download_destination, download_url, NULL};
    execv("/bin/wget", argv);
```

### C. Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip

### Penyelesaian

```c++
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

//...

void removeDir(char dir_name[STR_MAX_LENGTH]) {
    char *argv[] = {"rm", "-rf", dir_name, NULL};
    execv("/bin/rm", argv);
}

void makeNewDirectoryAndDownloadPictures(char dir_name[STR_MAX_LENGTH]) {
    int child_status;

    //membuat direktori

    //download 10 gambar

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
```

### Penjelasan

Setelah 10 gambar terdownload, maka akan akan dibuat terlebih dahulu sebuah file status.txt didalam direktori dimana gambar tadi di download. Nama file akan digabungkan dahulu dengan nama direktori lalu akan dibuat sebuah file dengan `fopen()`. Jika file tidak berhasil dibuat maka akan exit.

```c++
    char file_name[STR_MAX_LENGTH];
    sprintf(file_name, "%s/%s", dir_name, "status.txt");
    FILE *status_file = fopen(file_name, "w");

    if (status_file == NULL) {
        exit(EXIT_FAILURE);
    }
```

Kemudian tulisan "Download Success" akan dienkripsi dengan menggunakan teknik Ceasar Cipher dengan shift 5. Dan setelah di enkripsi maka akan di print kedalam file yang sudah dibuat.

```c++
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
```

Setelah status selesai dibuat, maka akan dilakukan fork untuk melakukan archive dari folder yang sudah selesai menjadi zip.

```c++
    pid_t archive_child_id;

    archive_child_id = fork();

    if (archive_child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (archive_child_id == 0) {
        archiveDir(dir_name);
    }
```

Dimana child akan menjalankan perintah untuk menghasilkan terlebih dahulu nama zip yang akan dibuat, lalu akan menjalan program `zip`.

```c++
void archiveDir(char dir_name[STR_MAX_LENGTH]) {
    char zip_name[STR_MAX_LENGTH];
    sprintf(zip_name, "%s.zip", dir_name);
    char *argv[] = {"zip", "-qr", zip_name, dir_name, NULL};
    execv("/bin/zip", argv);
}
```

Setelah proses zip selesai, maka program yang melakukan fork tadi akan menjalankan perintah untuk menghapus direktori yang tadi telah di zip.

```c++
    else {
        while (wait(&child_status) > 0) {
            //tunggu selesai archive
        }
        removeDir(dir_name);
    }
```

### D. Ketika program dijalankan maka akan membuat sebuah program bash yang bila dijalankan akan mengehentikan proses.

### Penyeleseaian

```c++
void makeKiller(pid_t process_id) {
    char file_name[] = "killer.sh";
    FILE *killer_file = fopen(file_name, "w");

    fputs("#!bin/bash\n\n", killer_file);

    fprintf(killer_file, "kill -SIGTERM %d\n", process_id);

    fprintf(killer_file, "rm -f %s\n", file_name);

    fclose(killer_file);
    return;
}

//...

int main(int argc, char const *argv[]) {
    pid_t main_process_id = getpid();
    makeKiller(main_process_id);

    //main loop
    //...

    return 0;
}
```

### Penjelasan

Untuk membuat sebuah file killer, maka ketika pertama kali program berjalan maka akan membuat sebuah file yang dinamakan "killer.sh". Dimana ke dalam file tersebut akan dituliskan 2 buah perintah yaitu untuk mengehentikan program dengan menggunakan perintah `kill` dan signal yang digunakan adalah sigterm disertai dengan process_id yang dimaksud. Dan kemudian perintahnya adalah untuk menhapus file "killer.sh" itu sendiri

```c++
    char file_name[] = "killer.sh";
    FILE *killer_file = fopen(file_name, "w");

    fputs("#!bin/bash\n\n", killer_file);

    fprintf(killer_file, "kill -SIGTERM %d\n", process_id);

    fprintf(killer_file, "rm -f %s\n", file_name);

    fclose(killer_file);
```

### E. Ketika program dijalankan dengan argumen -z, maka ketika program killer dijalankan akan menghentikan seluruh operasi, sedangkan jika dengan argumen -x maka ketika program killer dijalankan program utama akan berhenti namun child process dapat tetap berjalan.

### Penyelesaian

```c++

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

//...

int main(int argc, char const *argv[]) {
    pid_t main_process_id = getpid(),
          main_session_id = getsid(main_process_id);
    checkForCommandLineArgs(argc, argv);
    makeKiller(main_process_id, main_session_id);

    //main loop
    //...

    return 0;
}
```

### Penjelasan

Program akan terlebih dahulu melakukan pengecekan terhadap argumen yang diberikan. Jika argumen adalah -z, maka flag `kill_child_immediately` akan menjadi 1, sedangkan jika argumen adalah -z maka flag akan menjadi 0.

```c++
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
```

Modifkasi pada function `makeKiller` adalah merubah perintah yang dituliskan kedalam program bash berdasarkan argumen yang diberikan. Jika argumen adalah -z maka untuk melakukan kill program berserta anak anaknya maka dilkakukan dengan perintah `pkill -s`, -s artinya adalah melakukan kill berdasarkan session id. sedangkan jika argumen adalah -x, maka tetap menggunakan perintah `kill` dan signalnya adalah SIGTERM.

```c++
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
```

### Penyelesaian Akhir

```C++
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
```