#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

void sendline(t_process p, t_data d)
{
  append_byte(&d, 0xa);
  pwn_send(p, d);
}

void pwn_send(t_process p, t_data d)
{
  write(p.pin, d.data, d.len);
  if (p.log_level < 2)
    debug_bytes(d, 1);
}
