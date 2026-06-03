#ifndef INTERNAL_H
#define INTERNAL_H

#include "pawlib.h"
#include <stddef.h>
#include <stdint.h>

void paw_print_status( process p );
void paw_debug_bytes_type( data d, int via );
int  paw_append_byte( data *d, uint8_t c );

#endif
