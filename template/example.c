#include <pawlib.h>
#include <stdio.h>
#include <stdlib.h>

void setup( process *p )
{
    char *cmds[] = { "b main" };
    paw_attach( p, cmds, NULL );
}

int main()
{
    process *p = paw_open_process( "ls" );

    setup( p );

    paw_recv( p, 2 );
    paw_recvuntil( p, "src" );
    paw_recvline( p );
    paw_recvline( p );

    data payload = paw_init_data();
    paw_add( &payload, "HELLO\n\x00" );

    paw_debug( payload );

    paw_add_repeat( &payload, "AB", 10 );

    paw_sendline( p, payload );

    paw_close( p );
}
