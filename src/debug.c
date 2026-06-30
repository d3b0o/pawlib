#include "internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void paw_debug( data d )
{
    paw_debug_bytes_type( d, 3 );
}

void paw_debug_bytes_type( data d, int via )
{
    int            bytes_per_line;
    int            max_bytes_per_line = 16;
    int            x;
    uint8_t        current_byte;
    size_t         max_index = d.len - 1;
    struct winsize w;

    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );

    bytes_per_line = ( w.ws_col - 20 ) / 4;
    bytes_per_line = ( bytes_per_line / 4 ) * 4;

    if ( bytes_per_line > max_bytes_per_line || bytes_per_line < 4 )
    {
        bytes_per_line = max_bytes_per_line;
    }

    for ( size_t i = 0; i < d.len; i += bytes_per_line )
    {
        if ( via == 0 )
        {
            printf( ANSI_COLOR_GRAY "-> " ANSI_COLOR_RESET );
        }
        else if ( via == 1 )
        {
            printf( ANSI_COLOR_GRAY "<- " ANSI_COLOR_RESET );
        }
        else
        {
            printf( ANSI_COLOR_GRAY "-- " ANSI_COLOR_RESET );
        }
        printf( ANSI_COLOR_GRAY "%0.8zx: " ANSI_COLOR_RESET, i );

        for ( int x = 0; x < bytes_per_line; x++ )
        {
            if ( x % 4 == 0 && x > 0 )
            {
                printf( " " );
            }
            if ( i + x > max_index )
            {
                printf( "   " );
            }
            else
            {
                current_byte = d.data[i + x];
                if ( current_byte > 33 && current_byte < 126 )
                {
                    printf( "%2x ", current_byte );
                }
                else
                {
                    printf( ANSI_COLOR_RED "%0.2x " ANSI_COLOR_RESET, current_byte );
                }
            }
        }

        printf( "| " );
        for ( x = 0; x < bytes_per_line; x++ )
        {
            if ( i + x > max_index )
            {
                break;
            }
            current_byte = d.data[i + x];
            if ( current_byte > 33 && current_byte < 126 )
            {
                printf( "%c", current_byte );
            }
            else
            {
                printf( ANSI_COLOR_RED "@" ANSI_COLOR_RESET );
            }
        }
        printf( "\n" );
    }

    printf( "\n" );
}

void paw_attach_n( process *p, char *cmds[], int n )
{
    char cmd[512];

    if ( !getenv( "TMUX" ) )
    {
        fprintf( stderr, "[!] paw_attach necesita correr dentro de tmux\n" );
        return;
    }

    if ( cmds && n > 0 )
    {
        FILE *f = fopen( "/tmp/paw_gdb", "w" );
        if ( f )
        {
            for ( int i = 0; i < n; i++ )
                fprintf( f, "%s\n", cmds[i] );  // un comando por línea
            fclose( f );
        }
        snprintf( cmd, sizeof( cmd ), "tmux split-window -h 'gdb -p %d -x /tmp/paw_gdb'", p->pid );
    }
    else
    {
        snprintf( cmd, sizeof( cmd ), "tmux split-window -h 'gdb -p %d'", p->pid );
    }

    system( cmd );
}
