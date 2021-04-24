# soal-shift-sisop-modul-1-B02-2021

## Soal Nomor 1

###Penyelesaian

```
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
```
Program diatas merupakan program Daemon yang akan dijalankan di latar belakang atau background. Pada bagian program
```
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
   ```
bertujuan untuk menjalankan program 6 jam sebelum waktu 22.22 yaitu jam 16.22, dikarenakan pada soal waktu ulang tahun Stevany itu adalah jam 22.22. Ketika sedang dalam waktu 
16.22, program akan menjalankan berbagai fungsi yang terdapat di dalam fungsi main nya. Fungsi yang pertama dikerjakan yaitu fungsi ```downloadAllThreeArchive();```. Fungsi ini berisikan fungsi yang akan menjalankan fungsi ```downloadFromGoogleDrives();```
```
void downloadAllThreeArchive() {
    int child_status;
    pid_t download_child_id;
    download_child_id = fork();

    if (download_child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (download_child_id == 0) {
         downloadFromGoogleDrives();
    }
}
```
untuk melakukan pendownload-an pada 3 file yaitu Musik_for_Stevany.zip, Foto_for_Stevany.zip, dan Film_for_Stevany.zip.
```
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

```
Pada fungsi ini, wget digunakan untuk mendownload file dan execl bertugas untuk menjalankan atau mengesekusi perintah tersebut. 
Setelah, selesai melakukan pendownload-an pada 3 file tersebut, maka yang akan dijalankan selanjutnya adalah ```unzipDownloadedArchive();```. Fungsi ini bertujuan untuk mengekstrak file yang telah terdownload menjadi sebuah directory atau folder.
``` void unzipDownloadedArchive() {
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
```
Untuk Pengunzipan sendiri telah ada perintahnya yaitu  ```/bin/unzip```. Disini, untuk mempermudah pengunzipan, maka akan dimasukkan dulu kedalam variable dan kemudian variable tersebut di eksekusi menggunakan command execv().
Setelah dilakukannya pengekstrakan file, maka file tersebut akan di jalankan fungsi ```moveLocation()```. Pada fungsi ini, folder pada file yang telah diekstrak tersebut akan dipindahkan atau melakukan rename dengan cara remove directory ke directory dengan nama Fylm, Pyoto, dan Musyik dikarenakan Stevany sangat menyukai huruf 'Y'.
```
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
```
Untuk pemindahan atau rename directory ini, menggunakan command mv yaitu  ```/bin/mv```.
Kemudian, setelah dilakukannya pemindahan nama directory, maka menunggu atau melakukan perubahan waktu pada sistem hingga mencapai waktu yang diinginkan yaitu 22.22 yang merupakan waktu ulang tahunnya Stevany.
```
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
```
Setelah waktu telah menunjukkan pukul 22.22, maka program akan menjalan kan fungsi yaitu ```removeAllDirAndMakeAZip();```Pada soal telah dijelaskan, ketika waktu pada sistem telah mencapai waktu 22.22, maka semua Folder akan digabungkan menjadi satu file dan akan di zip. Setelah itu, semua folder atau directory yang tersisa akan di remove dan menyisakan file-file yang hanya memiliki ekxtensi .zip.
```
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
```
Pada, execl() terdapat command -r yang berguna untuk melakukan penzipan folder dan remove folder secara bersamaan.

## Soal Nomor 2

### A. Membuat program C untuk unzip file pets.zip dengan kondisi hanya file foto saja

#### Penyelesaian

```c++
pid_t child_id;
  int status;
  child_id = fork();
  if (child_id < 0) {
    char *argmd[]={"mkdir", "-p","/home/arkan/modul2/petshop", NULL};
    execv("/bin/mkdir", argmd);
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    char *argunz[]={"unzip", "-q", "/home/arkan/modul2/pets.zip","-x","*/*","-d", "/home/arkan/modul2/petshop",NULL};
    execv("/bin/unzip", argunz);
    }

  else{
    while((wait(&status)) > 0);
   }
```

#### Penjelasan

Disini saya menggunakan exec unzip dengan fungsi -x yaitu exclude suatu filetype. Karena diharuskan unzip foto saja dan didalam pets.zip hanya ada foto dan folder, maka kita exclude foldernya dengan */*.

### B. Membuat folder dengan nama masing-masing hewan

#### Penyelesaian

```c++
DIR *dr;
  struct dirent *folder;
  dr=opendir("/home/arkan/modul2/petshop");
  char base[50]="/home/arkan/modul2/petshop/";
  if (dr != NULL){
      while ((folder = readdir (dr))!=NULL){
          char *filename = folder->d_name;
          if (strcmp(filename, ".") == 0 || strcmp(filename, "..")== 0) continue;
          pid_t child_id6;
          child_id6=fork();
          int status6;
          if (child_id6 == 0) continue;

          char *newname;
          int i;
          for(i=0; filename[i] != '\0'; i++);
          int n=i-4+1;
          if (n<1) NULL;
          newname=(char*)malloc(n*sizeof(char));
          for (i=0;i<n-1;i++) newname[i]=filename[i];
          newname[i]='\0';

          char *misah1,*misah2;
          char kind[50], name[50], age[50];
          char *jus;
          while((misah1=strtok_r(newname, "_", &newname))!=NULL){
            int a=2;
            char *t_misah1=misah1;
            
            while((misah2=strtok_r(t_misah1, ";", &t_misah1))!=NULL){
              char jos[99];
              if(a==2){
                pid_t child_id2;
                child_id2=fork();
                int status2;
                if (child_id2 == 0){
                  strcpy(jos,base);
                  strcat(jos,misah2);
                  char *argmk[]={"mkdir","-p",jos, NULL};
                  execv("/bin/mkdir", argmk);
                }
                else{
                  while((wait(&status2)) > 0);
                  strcpy(kind,misah2);
                }
              }
              else if(a==1) strcpy(name, misah2);
              else if(a==0) strcpy(age, misah2);
              a--;
            }
```

#### Penjelasan

disini saya memakai kodingan dari modul untuk directory listing dengan sedikit modifikasi. lalu karena nama file foto terdapat jenis binatang, nama, dan umur yang menjadi 1, maka perlu kita pisah-pisahkan. namun sebelum itu, karena ada beberapa foto yang memiliki 2 binatang berbeda di dalam 1 foto yang dipisahkan oleh _ , maka perlu dipotong menggunakan strtok_r(r disini berfungsi agar string yang dipisah tidak hilang). setelah semua jenis binatang terpisah, baru kita potong lagi dengan cara yang sama, namun pemisahnya disini adalah ; . setelah itu saya masukkan string2 yang terpisah tersebut ke dalam masing2 variabel (kind, name, age). terakhir saya exec mkdir sesuai nama yang diinginkan (disini diinginkan nama jenis binatang).

### C. memasukkan foto ke masing-masing folder yang sesuai dan jika ada 2 nama di 1 foto, dimasukkan ke folder masing-masing

### Penyelesaian

```c++
//memasukkan foto
          pid_t child_id3;
          child_id3=fork();
          int status3;
          char filenya[99], foldernya[99];
          if(child_id3==0){
            strcpy(filenya, base);
            strcat(filenya, kind);
            strcat(filenya, "/");
            strcat(name,".jpg");
            strcat(filenya, name);

            strcpy(foldernya, base);
            strcat(foldernya, filename);
            char* argmv[] = {"cp", foldernya, filenya, NULL};
            execv("/bin/cp", argmv);
          }
```
```c++
//menghapus foto
      while(wait(NULL)>0);
      // printf("%s\n", folder->d_name);
      char base1[50]="/home/arkan/modul2/petshop/";
      strcat(base1, folder->d_name);
      char* argrm[] = {"rm","-r","-f", base1, NULL};
      execv("/bin/rm", argrm);
```

### Penjelasan

Awalnya saya ingin menggonakan exec move, namun saya tidak bisa memindahkan 2 nama yang berada di 1 foto itu, jadi saya memakai copy lalu remove.

### D. Jika ada 2 nama di 1 foto, dimasukkan ke folder masing-masing

### Penyelesaian

```c++
while((misah1=strtok_r(newname, "_", &newname))!=NULL){
            int a=2;
            char *t_misah1=misah1;
            
            while((misah2=strtok_r(t_misah1, ";", &t_misah1))!=NULL){
              char jos[99];
              if(a==2){
                pid_t child_id2;
                child_id2=fork();
                int status2;
                if (child_id2 == 0){
                  strcpy(jos,base);
                  strcat(jos,misah2);
                  char *argmk[]={"mkdir","-p",jos, NULL};
                  execv("/bin/mkdir", argmk);
                }
                else{
                  while((wait(&status2)) > 0);
                  strcpy(kind,misah2);
                }
              }
              else if(a==1) strcpy(name, misah2);
              else if(a==0) strcpy(age, misah2);
              a--;
            }
```

### Penjelasan

Seperti yang sudah dijelaskan di sub nomor b, karena ada beberapa foto yang memiliki 2 binatang berbeda di dalam 1 foto yang dipisahkan oleh _ , maka perlu dipotong menggunakan strtok_r

### E. membuat keterangan sesuai ketentuan di masing2 folder

### Penyelesaian

```c++
        char base2[50]="/home/arkan/modul2/petshop/";
        char ket[50]="/keterangan.txt";
        strcat(base2, kind);
        strcat(base2, ket);
        char skut[99] = "nama : ";
        strcat(skut, name); 
        strcat(skut, "\numur  : "); 
        strcat(skut, age); 
        strcat(skut, " tahun\n\n");
        FILE* txt = fopen(base2, "a");
        fputs(skut, txt);
        fclose(txt);
```

### Penjelasan

saya memakai variabel yang sudah dimasukkan tadi(name, kind, age) untuk langsung di put di file keterangan.txt tadi, dengan cara meng cat nama umur tahun tadi ke suatu variabel.

### Penyelesaian Akhir

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
#include <syslog.h>

int main() {
    //nomor 2a
  pid_t child_id;
  int status;
  child_id = fork();
  if (child_id < 0) {
    char *argmd[]={"mkdir", "-p","/home/arkan/modul2/petshop", NULL};
    execv("/bin/mkdir", argmd);
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    char *argunz[]={"unzip", "-q", "/home/arkan/modul2/pets.zip","-x","*/*","-d", "/home/arkan/modul2/petshop",NULL};
    execv("/bin/unzip", argunz);
    }

  else{
    while((wait(&status)) > 0);
   }
//////////////////////////////

//nomor 2b
  DIR *dr;
  struct dirent *folder;
  dr=opendir("/home/arkan/modul2/petshop");
  char base[50]="/home/arkan/modul2/petshop/";
  if (dr != NULL){
      while ((folder = readdir (dr))!=NULL){
          char *filename = folder->d_name;
          if (strcmp(filename, ".") == 0 || strcmp(filename, "..")== 0) continue;
          pid_t child_id6;
          child_id6=fork();
          int status6;
          if (child_id6 == 0) continue;

          char *newname;
          int i;
          for(i=0; filename[i] != '\0'; i++);
          int n=i-4+1;
          if (n<1) NULL;
          newname=(char*)malloc(n*sizeof(char));
          for (i=0;i<n-1;i++) newname[i]=filename[i];
          newname[i]='\0';

          char *misah1,*misah2;
          char kind[50], name[50], age[50];
          char *jus;
          while((misah1=strtok_r(newname, "_", &newname))!=NULL){
            int a=2;
            char *t_misah1=misah1;
            
            while((misah2=strtok_r(t_misah1, ";", &t_misah1))!=NULL){
              char jos[99];
              if(a==2){
                pid_t child_id2;
                child_id2=fork();
                int status2;
                if (child_id2 == 0){
                  strcpy(jos,base);
                  strcat(jos,misah2);
                  char *argmk[]={"mkdir","-p",jos, NULL};
                  execv("/bin/mkdir", argmk);
                }
                else{
                  while((wait(&status2)) > 0);
                  strcpy(kind,misah2);
                }
              }
              else if(a==1) strcpy(name, misah2);
              else if(a==0) strcpy(age, misah2);
              a--;
            }
          ///////////////////////////

          //nomor 2c and 2d

          pid_t child_id3;
          child_id3=fork();
          int status3;
          char filenya[99], foldernya[99];
          if(child_id3==0){
            strcpy(filenya, base);
            strcat(filenya, kind);
            strcat(filenya, "/");
            strcat(name,".jpg");
            strcat(filenya, name);

            strcpy(foldernya, base);
            strcat(foldernya, filename);
            char* argmv[] = {"cp", foldernya, filenya, NULL};
            execv("/bin/cp", argmv);
          }
          // while(wait(NULL)>0);
          // pid_t child_id5;
          // child_id5=fork();
          // int status5;
          // if(child_id5==0){

              //nomor 2e
            char base2[50]="/home/arkan/modul2/petshop/";
            char ket[50]="/keterangan.txt";
            strcat(base2, kind);
            strcat(base2, ket);
            char skut[99] = "nama : ";
            strcat(skut, name); 
            strcat(skut, "\numur  : "); 
            strcat(skut, age); 
            strcat(skut, " tahun\n\n");
            FILE* txt = fopen(base2, "a");
            fputs(skut, txt);
            fclose(txt);
          // }
      }
      while(wait(NULL)>0);
      // printf("%s\n", folder->d_name);
      char base1[50]="/home/arkan/modul2/petshop/";
      strcat(base1, folder->d_name);
      char* argrm[] = {"rm","-r","-f", base1, NULL};
      execv("/bin/rm", argrm);
    }
   //////////////////////         
      (void) closedir (dr);
}
}
```

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
