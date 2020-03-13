#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <dirent.h>

int main() {

  pid_t child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mkdir", "-p", "/home/umum/modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }
  while(wait(NULL) != child_id);
  sleep(5);

  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mkdir", "-p", "/home/umum/modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }
  while(wait(NULL) != child_id);

  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"unzip", "-q", "jpg.zip", "-d", "/home/umum/modul2", NULL};
    execv("/usr/bin/unzip", argv);
  }
  while(wait(NULL) != child_id);

  char f_name[256];
  struct dirent *de;
  DIR *dr = opendir("/home/umum/modul2/jpg");

  while((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
      continue;
    }
    sprintf(f_name+strlen(f_name), "%s\n", de->d_name);
  }

  closedir(dr);

  char *token = strtok(f_name, "\n");

  while (token != NULL) {
    char location[80];
    struct stat location_stat;

    sprintf(location, "/home/umum/modul2/jpg/%s", token);
    stat(location, &location_stat);

    if (S_ISDIR(location_stat.st_mode)) {

      child_id = fork();
      if (child_id == 0) {
        char *argv[] = {"mv", location, "/home/umum/modul2/indomie", NULL};
        execv("/bin/mv", argv);
      }
      while(wait(NULL) != child_id);

      char new_location[80];
      child_id = fork();
      if (child_id == 0) {
        sprintf(new_location, "/home/umum/modul2/indomie/%s/coba1.txt", token);
        char *argv[] = {"touch", new_location, NULL};
        execv("/usr/bin/touch", argv);
      }

      child_id = fork();
      if (child_id == 0) {
        sleep(3);
        sprintf(new_location, "/home/umum/modul2/indomie/%s/coba2.txt", token);
        char *argv[] = {"touch", new_location, NULL};
        execv("/usr/bin/touch", argv);
      }

    } else if (S_ISREG(location_stat.st_mode)) {

      child_id = fork();
      if (child_id == 0) {
        char *argv[] = {"mv", location, "/home/umum/modul2/sedaap", NULL};
        execv("/bin/mv", argv);
      }
      while(wait(NULL) != child_id);

    }

    token = strtok(NULL, "\n");

  }

}
