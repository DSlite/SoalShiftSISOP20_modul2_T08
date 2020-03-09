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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("ERROR! Argumen Salah\n");
    return 1;
  }
  if (strcmp(argv[1], "-a") != 0 && strcmp(argv[1], "-b") != 0) {
    printf("ERROR! Mode Salah\n");
    return 1;
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

  FILE *pFile;
  pFile = fopen("killer.c", "w");
  if (strcmp(argv[1], "-a") == 0) {
    char *inp = ""
    "#include <unistd.h>\n"
    "#include <wait.h>\n"
    "int main() {\n"
      "pid_t child_id = fork();\n"
      "if (child_id == 0) {\n"
        "char *argv[] = {\"pkill\", \"-9\", \"-s\", \"%d\", NULL};\n"
        "execv(\"/usr/bin/pkill\", argv);\n"
      "}\n"
      "while(wait(NULL) > 0);\n"
      "char *argv[] = {\"rm\", \"killer\", NULL};\n"
      "execv(\"/bin/rm\", argv);\n"
    "}\n";
    fprintf(pFile, inp, sid);
  }
  if (strcmp(argv[1], "-b") == 0) {
    char *inp = ""
    "#include <unistd.h>\n"
    "#include <wait.h>\n"
    "int main() {\n"
      "pid_t child_id = fork();\n"
      "if (child_id == 0) {\n"
        "char *argv[] = {\"kill\", \"-9\", \"%d\", NULL};\n"
        "execv(\"/bin/kill\", argv);\n"
      "}\n"
      "while(wait(NULL) > 0);\n"
      "char *argv[] = {\"rm\", \"killer\", NULL};\n"
      "execv(\"/bin/rm\", argv);\n"
    "}\n";
    fprintf(pFile, inp, getpid());
  }
  fclose(pFile);

  pid = fork();
  if (pid == 0) {
    char *argv[] = {"gcc", "killer.c", "-o", "killer", NULL};
    execv("/usr/bin/gcc", argv);
  }
  while(wait(NULL) != pid);

  pid = fork();
  if (pid == 0) {
    char *argv[] = {"rm", "killer.c", NULL};
    execv("/bin/rm", argv);
  }
  while(wait(NULL) != pid);

  close(STDIN_FILENO);
  close(STDERR_FILENO);
  close(STDOUT_FILENO);

  while(1) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char now[80];
    strftime(now, 80, "%Y-%m-%d_%H:%M:%S", tm);

    pid_t child_id;
    child_id = fork();

    if (child_id == 0) {
      char *argv[] = {"mkdir", now, NULL};
      execv("/bin/mkdir", argv);
    }

    child_id = fork();

    if (child_id == 0) {
      for (int i = 0; i < 20; i++) {
        pid_t exec_id = fork();
        if (exec_id == 0) {
          t = time(NULL);
          tm = localtime(&t);
          char new_now[80], location[160], link[80];
          strftime(new_now, 80, "%Y-%m-%d_%H:%M:%S", tm);
          sprintf(location, "%s/%s", now, new_now);
          sprintf(link, "https://picsum.photos/%ld", ((t%1000)+100));
          char *argv[] = {"wget", "-O", location, link, NULL};
          execv("/usr/bin/wget", argv);
        }
        sleep(5);
      }

      child_id = fork();

      if (child_id == 0) {
        char nama_file[80];
        sprintf(nama_file, "%s.zip", now);
        char *argv[] = {"zip", "-r", nama_file, now, NULL};
        execv("/usr/bin/zip", argv);
      }
      while(wait(NULL) != child_id);
      char *argv[] = {"rm", "-r", now, NULL};
      execv("/bin/rm", argv);
    }
    sleep(30);
  }
}
