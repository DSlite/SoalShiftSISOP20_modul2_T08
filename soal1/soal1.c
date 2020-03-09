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
#include <wait.h>
#include <signal.h>


int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("ERROR! Argumen Salah\n");
    return 0;
  }
  if (!(*argv[1] == '*' || (atoi(argv[1]) >= 0 && atoi(argv[1]) <= 59))) {
    printf("ERROR! Argumen Salah\n");
    return 0;
  }
  if (!(*argv[2] == '*' || (atoi(argv[2]) >= 0 && atoi(argv[2]) <= 59))) {
    printf("ERROR! Argumen Salah\n");
    return 0;
  }
  if (!(*argv[3] == '*' || (atoi(argv[3]) >= 0 && atoi(argv[3]) <= 23))) {
    printf("ERROR! Argumen Salah\n");
    return 0;
  }

  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

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

  while (1) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char jam[5], menit[5], detik[5];
    strftime(jam, 5, "%H", tm);
    strftime(menit, 5, "%M", tm);
    strftime(detik, 5, "%S", tm);

    if ((strcmp(argv[1], detik) == 0) || *argv[1] == '*') {
      if ((strcmp(argv[2], menit) == 0) || *argv[2] == '*') {
        if ((strcmp(argv[3], jam) == 0) || *argv[3] == '*') {
          pid_t child_id;

          child_id = fork();

          if (child_id == 0) {
            char *argvChild[] = {"bash", argv[4], NULL};
            execv("/bin/bash", argvChild);
          } else if (child_id > 0) {
            while(wait(NULL) > 0);
          }
        }
      }
    }
    sleep(1);
  }
}
