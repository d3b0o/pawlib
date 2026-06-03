#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

void paw_print_status(t_process p)
{
  printf("[i] Binary loaded\n");
  printf("Name:\t%s\n", p.name);
  printf("pid:\t%d\n", p.pid);
  printf("\n\n");
}

t_process paw_open_process(char *file)
{
  int pipe_sf[2];
  int pipe_fs[2];
  pipe(pipe_sf);
  pipe(pipe_fs);
  t_process p;

  char c = 'X';

  int pid;
  pid = fork();

  if (pid != 0)
  {
    p.name = file;
    p.pin = pipe_fs[1];
    p.pout = pipe_sf[0];
    p.pid = pid;
    p.log_level = 0;
    
    paw_print_status(p);
    return p;
  }
  else
  {
    dup2(pipe_sf[1], 1);
    close(pipe_sf[0]);
    close(pipe_sf[1]);

    dup2(pipe_fs[0], 0);
    close(pipe_fs[0]);
    close(pipe_fs[1]);
    
    char *args[] = {file, NULL};
    execvp(file, args);
  }
  return p;
}

