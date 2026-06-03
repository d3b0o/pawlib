#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdint.h>
#include <stddef.h>
#include "pawlib.h"

void paw_print_status(t_process p);
void paw_debug_bytes_type(t_data d, int via);
int paw_append_byte(t_data *d, uint8_t c);

#endif
