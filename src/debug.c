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
        printf( ANSI_COLOR_GRAY "%08zx: " ANSI_COLOR_RESET, i );

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
                    printf( ANSI_COLOR_RED "%02x " ANSI_COLOR_RESET, current_byte );
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

static void paw_expand_cmd( char *out, size_t out_size, const char *tmpl, int pid,
                            const char *script )
{
    char   pidstr[16];
    size_t o = 0;

    snprintf( pidstr, sizeof( pidstr ), "%d", pid );

    for ( size_t i = 0; tmpl[i] && o < out_size - 1; )
    {
        const char *sub = NULL;
        size_t      tok = 0;

        if ( strncmp( &tmpl[i], "{pid}", 5 ) == 0 )
        {
            sub = pidstr;
            tok = 5;
        }
        else if ( strncmp( &tmpl[i], "{script}", 8 ) == 0 )
        {
            sub = script;
            tok = 8;
        }

        if ( sub )
        {
            while ( *sub && o < out_size - 1 )
                out[o++] = *sub++;
            i += tok;
        }
        else
        {
            out[o++] = tmpl[i++];
        }
    }

    out[o] = '\0';
}

void paw_attach_n( process *p, char *cmds[], int n, char *terminal )
{
    char        cmd[1024];
    const char *tmpl = terminal ? terminal : "tmux split-window -h 'gdb -p {pid} -x {script}'";

    if ( !getenv( "GDB" ) )
        return;

    FILE *f = fopen( "/tmp/paw_gdb", "w" );
    if ( f )
    {
        fprintf( f, "handle SIGSTOP nostop noprint nopass\n" );

        for ( int i = 0; i < n; i++ )
            fprintf( f, "%s\n", cmds[i] );

        fclose( f );
    }

    paw_expand_cmd( cmd, sizeof( cmd ), tmpl, p->pid, "/tmp/paw_gdb" );
    system( cmd );
}
