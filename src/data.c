#include "internal.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

data paw_init_data()
{
    return paw_init_data_size( 16 );
}

data paw_init_data_size( int size )
{
    data d;

    d.len  = 0;
    d.size = size;
    d.data = malloc( sizeof( uint8_t ) * d.size );

    if ( !d.data )
    {
        return d;
    }
    return d;
}

void paw_add_data( data *p, uint8_t *data, int size )
{
    for ( int i = 0; i < size; i++ )
    {
        paw_append_byte( p, data[i] );
    }
}

void paw_add_repeat_data( data *p, uint8_t *data, int size, int times )
{
    for ( int i = 0; i < times; i++ )
    {
        paw_add_data( p, data, size );
    }
}

int paw_append_byte( data *d, uint8_t c )
{
    if ( d->len >= d->size )
    {
        d->size += 16;
        uint8_t *new_buff = realloc( d->data, sizeof( uint8_t ) * d->size );
        if ( !new_buff )
        {
            return 1;
        }
        d->data = new_buff;
    }

    d->data[d->len++] = c;
    return 0;
}

void paw_free( data d )
{
    free( d.data );
}

void paw_add_p64( data *d, addr addr )
{
    paw_add_data( d, (uint8_t *) &addr, sizeof( addr ) );
}

addr paw_u64( data d )
{
    addr value = 0;
    memcpy( &value, d.data, d.len );
    return value;
}
