#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <asm/unistd.h>
#include <stdbool.h>

enum {
        IOPRIO_CLASS_NONE,
        IOPRIO_CLASS_RT,
        IOPRIO_CLASS_BE,
        IOPRIO_CLASS_IDLE,
};

enum {
        IOPRIO_WHO_PROCESS = 1,
        IOPRIO_WHO_PGRP,
        IOPRIO_WHO_USER,
};

#define IOPRIO_CLASS_SHIFT 13

const char *to_prio[] = { "none    ", "realtime", "beffort ", "idle    ", };
/*
static int ioprio_set(int which, int who, int ioprio)
{
    return syscall(SYS_ioprio_set, which, who, ioprio);
}
*/
static int ioprio_get(int which, int who)
{
     return syscall(SYS_ioprio_get, which, who);
}

static void proc_scan(void)
{
  DIR *dp;
  struct dirent *entry;
  int cmd_fd;
  int stat_fd;

  char filename[255];
  char cmd[255];
  bool is_kthread;

  if (!(dp = opendir("/proc"))) perror("opendir");

  while ((entry = readdir(dp))) {
    unsigned int u;
    char *pcmd;

    if (!(u = atoi(entry->d_name))) continue;
    sprintf(filename, "/proc/%u/cmdline", u);

    cmd_fd = open(filename, O_RDONLY);
    if(cmd_fd == -1) {
       perror("open file");
    } else {
       if(read(cmd_fd, cmd, sizeof(cmd)) == 0) { //if kernel thread
           is_kthread = true;

           sprintf(filename, "/proc/%u/stat", u);
           stat_fd = open(filename, O_RDONLY);
           read(stat_fd, cmd, sizeof(cmd));
           strtok_r(cmd, "(", &pcmd);
           pcmd = strtok(pcmd, ")");
           close(stat_fd);
       } else {
           is_kthread = false;
           pcmd = cmd;
       }
    }
    close(cmd_fd);

    int ioprio;
    int ioprio_class;
    ioprio = ioprio_get(IOPRIO_WHO_PROCESS, u);
    if (ioprio == -1)
      perror("ioprio_get");
    else {
      ioprio_class = ioprio >> IOPRIO_CLASS_SHIFT;
      ioprio = ioprio & 0xff;
    }

    if(is_kthread) {
      printf("%d\t%s\t%d\t [%s]\n", u, to_prio[ioprio_class], ioprio, pcmd);
    } else {
      printf("%d\t%s\t%d\t %s\n", u, to_prio[ioprio_class], ioprio, pcmd);
    }
  }

  closedir(dp);
}

int main(int argc, char **argv)
{
  printf("PID\tCLASS    \tPRIO\t COMMAND\n");
  proc_scan();
}



//shell code for pioprio
/*

#!/bin/bash

#set -x

pslist=`ps -aux | sed '1d' | awk '{print $2}'`

for pid in $pslist; do
  if [[ $pid -lt $$ ]]; then
    calc=`cat /proc/$pid/cmdline | wc -c`
    if [ $calc -eq "0" ]; then
      echo -e "$pid\t`ionice -p $pid`\t[`cat /proc/$pid/task/$pid/comm`]"
    else
      echo -e "$pid\t`ionice -p $pid`\t`cat /proc/$pid/cmdline`"
    fi
  fi
done

*/
