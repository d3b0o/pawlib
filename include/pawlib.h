#ifndef PAWLIB_H
#define PAWLIB_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_GRAY  "\x1b[90m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define paw_add( p, data, size ) paw_add_data( p, (uint8_t *) ( data ), size )
#define paw_add_repeat( p, data, size, times )                                                     \
    paw_add_repeat_data( p, (uint8_t *) ( data ), size, times )

typedef struct
{
    int   pin;
    int   pout;
    char *name;
    int   pid;
    int   log_level;
} process;

typedef struct
{
    size_t   size;
    uint8_t *data;
    size_t   len;
} data;

typedef uintptr_t addr;

process paw_open_process( char *file );

void paw_sendline( process p, data d );
void paw_send( process p, data d );

data paw_recv( process p, int size );
data paw_recvline( process p );
data paw_recvuntil( process p, char *delimiter, int delimiter_len );

data paw_init_data_size( int size );
data paw_init_data();
void paw_add_data( data *p, uint8_t *data, int size );
void paw_add_repeat_data( data *p, uint8_t *data, int size, int times );
void paw_add_p64( data *d, uintptr_t addr );
addr paw_u64( data d );
void paw_clean_data( data d );

void paw_debug( data d );
#endif
