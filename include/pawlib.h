#ifndef PAWLIB_H
#define PAWLIB_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define paw_add(p, data, size) paw_add_data(p, (uint8_t *)(data), size)
#define paw_add_repeat(p, data, size, times) paw_add_repeat_data(p, (uint8_t *)(data), size, times)

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

typedef uintptr_t addr_t;

t_process paw_open_process(char *file);

void paw_sendline(t_process p, t_data d);
void paw_send(t_process p, t_data d);

t_data paw_recv(t_process p, int size);
t_data paw_recvline(t_process p);
t_data paw_recvuntil(t_process p, char *delimiter, int delimiter_len);


t_data paw_init_data_size(int size);
t_data paw_init_data();
void paw_add_data(t_data *p, uint8_t *data, int size);
void paw_add_repeat_data(t_data *p, uint8_t *data, int size, int times);
void paw_add_p64(t_data *d, uintptr_t addr);
uintptr_t paw_u64(t_data d);
void paw_clean_data(t_data d);

void paw_debug(t_data d);
#endif
