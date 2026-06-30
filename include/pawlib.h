#ifndef PAWLIB_H
#define PAWLIB_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ANSI_COLOR_RED   "\x1b[31m"
#define ANSI_COLOR_GRAY  "\x1b[90m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define paw_add( p, s )       paw_add_data( ( p ), (uint8_t *) ( s ), sizeof( s ) - 1 )
#define paw_recvuntil( p, s ) paw_recvuntil_n( ( p ), ( s ), sizeof( s ) - 1 )
#define paw_add_repeat( p, s, times )                                                              \
    paw_add_repeat_data( ( p ), (uint8_t *) ( s ), sizeof( s ) - 1, ( times ) )
#define paw_attach( p, cmds )                                                                      \
    paw_attach_n( ( p ), ( cmds ), sizeof( cmds ) / sizeof( ( cmds )[0] ) )
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

process *paw_open_process( char *file );

void paw_sendline( process *p, data d );
void paw_send( process *p, data d );

data paw_recv( process *p, int size );
data paw_recvline( process *p );
data paw_recvuntil_n( process *p, char *delimiter, int delimiter_len );

data paw_init_data_size( int size );
data paw_init_data();
void paw_add_data( data *p, uint8_t *data, int size );
void paw_add_repeat_data( data *p, uint8_t *data, int size, int times );
void paw_add_p64( data *d, uintptr_t addr );
addr paw_u64( data d );
void paw_free( data d );

void paw_debug( data d );
void paw_attach_n( process *p, char *cmds[], int n );

void paw_close( process *p );

#endif
