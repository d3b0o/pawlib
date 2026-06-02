#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdint.h>
#include <stddef.h>
#include "pawlib.h"

void print_status(t_process p);
void debug_bytes(t_data d, int via);
int append_byte(t_data *d, uint8_t c);

#endif
