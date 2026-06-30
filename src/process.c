#include "internal.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/prctl.h>
#ifndef PR_SET_PTRACER
#define PR_SET_PTRACER 0x59616d61
#endif
#ifndef PR_SET_PTRACER_ANY
#define PR_SET_PTRACER_ANY ( (unsigned long) -1 )
#endif
#endif

void paw_print_status( process *p )
{
    printf( "Name:\t%s\n", p->name );
    printf( "PID:\t%d\n", p->pid );
}

process *paw_open_process( char *file )
{
    int      pipe_sf[2], pipe_fs[2];
    process *p = malloc( sizeof( process ) );

    signal( SIGPIPE, SIG_IGN );

    if ( p == NULL )
    {
        perror( "malloc" );
        exit( 1 );
    }

    if ( pipe( pipe_sf ) == -1 || pipe( pipe_fs ) == -1 )
    {
        perror( "pipe" );
        exit( 1 );
    }

    int pid = fork();
    if ( pid == -1 )
    {
        perror( "fork" );
        exit( 1 );
    }

    if ( pid == 0 )
    {
        dup2( pipe_sf[1], 1 );
        dup2( pipe_fs[0], 0 );
        close( pipe_sf[0] );
        close( pipe_sf[1] );
        close( pipe_fs[0] );
        close( pipe_fs[1] );

        if ( getenv( "GDB" ) )
        {
#ifdef __linux__
            /* Allow any debugger to attach despite Yama ptrace_scope. */
            prctl( PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0 );
#endif
            raise( SIGSTOP );
        }

        char *args[] = { file, NULL };
        execvp( file, args );
        perror( "execvp" );
        _exit( 127 );
    }

    close( pipe_sf[1] );
    close( pipe_fs[0] );

    p->name      = file;
    p->pin       = pipe_fs[1];
    p->pout      = pipe_sf[0];
    p->pid       = pid;
    p->log_level = 0;

    paw_print_status( p );
    return p;
}
