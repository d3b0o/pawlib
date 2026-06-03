#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

t_data paw_recv(t_process p, int size)
{
  t_data d = paw_init_data_size(size); 
  d.len = read(p.pout, d.data, d.size);

  if (p.log_level < 2)
    paw_debug_bytes_type(d, 0);

  return d;
}

t_data paw_recvline(t_process p)
{
  t_data d = paw_init_data();
  uint8_t b;

  while (read(p.pout, &b, 1) == 1) {
    paw_append_byte(&d, b);
    if (b == '\n') {
      break;
    }
  }

  if (p.log_level < 2)
    paw_debug_bytes_type(d, 0);
  return d;
}


t_data paw_recvuntil(t_process p, char *delimiter, int delimiter_len)
{
  t_data d = paw_init_data();
  uint8_t b;
  
  while (read(p.pout, &b, 1) == 1)
  {
    paw_append_byte(&d, b);
    if (memmem(d.data, d.len, delimiter, delimiter_len) != NULL) { break; }
  }

  if (p.log_level < 2)
    paw_debug_bytes_type(d, 0);

  return d;
}
