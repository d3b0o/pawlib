#include "internal.h"

#include <string.h>

static int    paw_argc = 0;
static char **paw_argv = NULL;

__attribute__( ( constructor ) ) static void paw_auto_args( int argc, char **argv, char **envp )
{
    (void) envp;
    if ( !paw_argv )
    {
        paw_argc = argc;
        paw_argv = argv;
    }
}

void paw_args( int argc, char **argv )
{
    paw_argc = argc;
    paw_argv = argv;
}

int paw_argflag( const char *name )
{
    for ( int i = 1; i < paw_argc; i++ )
        if ( strcmp( paw_argv[i], name ) == 0 )
            return 1;

    return 0;
}
