#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void cmd_do(int argc, char **argv)
{
  printf("cmd is %s", argv[1]);
}

void cmd_help(int argc, char **argv)
{
  printf("insert delete help version");
}

void cmd_version(int argc, char **argv)
{
  printf("version 1");
}

//OOP
struct command
{
  char *name;
  void (*run)(int argc, char **argv);
};

static struct command cmds[] = {
  {"insert", cmd_do},
  {"delete", cmd_do},
  {"help", cmd_help},
  {"-h", cmd_help},
  {"version", cmd_version},
};

#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof(_a[0]))

int main(int argc, char **argv)
{
  int i;
  errno = 0;

  if(argc < 2) {
    printf("....\n");
    cmd_help(0, NULL);
    goto out;
  }

  for(i = 0; i<ARRAY_SIZE(cmds); i++) {
    if(strcmp(argv[1], cmds[i].name) == 0) {
      cmds[i].run(argc, argv);
      goto out;
    }
  }
  
  cmd_help(argc, argv);
out:
  exit(0);
}
