#ifndef PAWLIB_H
#define PAWLIB_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define payload_add(p, data, size) payload_append(p, (uint8_t *)(data), size)

typedef struct
{
    int pin;
    int pout;
    char *name;
    int pid;
    int log_level;
} t_process;

typedef struct
{
  size_t size;
  uint8_t *data;
  size_t len;
} t_data;


t_process open_process(char *file);

void sendline(t_process p, t_data d);
void pwn_send(t_process p, t_data d);

t_data recv(t_process p, int size);
t_data recvline(t_process p);
t_data recvuntil(t_process p, char *delimiter, int delimiter_len);


t_data init_data(int size);
t_data init_data_d();
t_data init_payload();
void payload_append(t_data *p, uint8_t *data, int size);
void clean_data(t_data d);

#endif
