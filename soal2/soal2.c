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