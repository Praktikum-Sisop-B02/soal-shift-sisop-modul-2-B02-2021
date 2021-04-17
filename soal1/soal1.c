#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <string.h>

int main() {
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  time_t current = time(NULL);
  struct tm *time_current = localtime(&current);

  int current_hour = time_current->tm_hour;
  int current_min = time_current->tm_min;

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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

   close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) {
    if(current_hour==22&&current_min==22){
        execl("/usr/bin/zip", "-r", "Lopyu_Stevany.zip", "Musyik", "Pyoto", "Fylm", NULL);
        execl("/bin/rm", "-r", "Musyik", "Pyoto", "Fylm", NULL);
//      execl("/bin/rm", "-r", "Musik_for_Stevany", "Foto_for_Stevany", "Film_for_Stevany", NULL);
    }
    else if((current_hour>=16&&current_min>=22)&&(current_hour<22&&current_min<=22)){
        execl("/usr/bin/wget", "-b", "drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", "-O", "Musyik.zip", NULL);
        execl("/usr/bin/wget", "-b", "drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", "-O", "Pyoto.zip", NULL);
        execl("/usr/bin/wget", "-b", "drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", "-O", "Fylm.zip", NULL);

        execl("/usr/bin/unzip", "Musyik.zip", NULL);
	execl("/usr/bin/unzip", "Pyoto.zip", NULL);
        execl("/usr/bin/unzip", "Fylm.zip", NULL);

    }
   // execl("/usr/bin/wget",/* "--no-check-certificate",*/ "drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",/* "-O"$
   // execl("/usr/bin/wget",/* "--no-check-certificate",*/ "drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",/* "-O"$
   // execl("/usr/bin/wget",/* "--no-check-certificate",*/ "drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",/* "-O"$
    
   // execl("/usr/bin/unzip", "Musik_for_Stevany.zip", NULL);
   // execl("/usr/bin/unzip", "Foto_for_Stevany.zip", NULL);
   // execl("/usr/bin/unzip", "Film_for_Stevany.zip", NULL);

    //execl("/bin/cp", "Musik_for_Stevany", "Musyik", NULL);
   // execl("/bin/rm", "Musik_for_Stevany", NULL);

    //execl("/bin/cp", "Foto_for_Stevany", "Pyoto", NULL);
      // execl("/bin/rm", "Foto_for_Stevany", NULL);

    //execl("/bin/cp", "Film_for_Stevany", "Fylm", NULL);
   // execl("/bin/rm", "Film_for_Stevany", NULL);

    
      // execl("/usr/bin/zip", "Lopyu_Stevany", "Musyik", "Pyoto", "Fylm", NULL);
      // execl("/bin/rm", "-r", "Musyik", "Pyoto", "Fylm", NULL);
    

    sleep(30);
  }
}


