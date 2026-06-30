#include "internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void paw_sendline( process *p, data d )
{
    paw_append_byte( &d, 0xa );
    paw_send( p, d );
}

void paw_send( process *p, data d )
{
    ssize_t n = write( p->pin, d.data, d.len );
    if ( n == -1 )
        perror( "paw_send: write" );
    if ( p->log_level < 2 )
        paw_debug_bytes_type( d, 1 );
}
