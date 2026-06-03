#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

void paw_sendline(t_process p, t_data d)
{
  paw_append_byte(&d, 0xa);
  paw_send(p, d);
}

void paw_send(t_process p, t_data d)
{
  write(p.pin, d.data, d.len);
  if (p.log_level < 2)
    paw_debug_bytes(d, 1);
}
