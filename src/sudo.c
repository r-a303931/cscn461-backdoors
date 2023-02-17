#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t ppid = getppid();
  time_t rawtime;
  struct tm *info;
  char currenttimestr[80];
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(currenttimestr, 80, "%Y-%m-%d %H:%M", info);
  char *user = getenv("USER");
  char *ssh = getenv("SSH_CLIENT");

  int cmdbuflen = 0;
  for (int i = 0; i < argc; i++) {
    cmdbuflen += strlen(argv[i]) + 1;
  }

  char *cmdbuf = (char *)malloc(cmdbuflen);
  memset(cmdbuf, 0, cmdbuflen);
  char *cmdbufptr = cmdbuf;

  for (int i = 0; i < argc; i++) {
    int len = strlen(argv[i]);
    memcpy(cmdbufptr, argv[i], len);
    cmdbufptr += len;
    *cmdbufptr = ' ';
    cmdbufptr += 1;
  }
  *(cmdbufptr - 1) = '\0';

  if (!user)
    user = "(unknown)";

  if (!ssh)
    ssh = "(local)";

  char *tty = ttyname(0);

  FILE *log = fopen("/var/log/sudo_history", "a");
  if (log) {
    fprintf(log, "%s - %s - %s - %d - %s -- %s\n", currenttimestr, user, tty,
            ppid, ssh, cmdbuf);
    fclose(log);
  }

  char sudopath[] = "/usr/sbin/sudo\0";
  if (strlen(argv[0]) > strlen(sudopath) + 1) {
    memcpy(argv[0], sudopath, strlen(sudopath));
    argv[0][strlen(sudopath)] = '\0';
  }
  char **cargv = (char **)malloc(argc + 1);
  cargv[0] = sudopath;

  for (int i = 1; i < argc; i++) {
    cargv[i] = argv[i];
  }
  cargv[argc] = NULL;

  return execvp(sudopath, cargv);
}
