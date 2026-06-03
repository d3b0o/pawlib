#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

data paw_recv(process p, int size)
{
  data d = paw_init_data_size(size); 
  d.len = read(p.pout, d.data, d.size);

  if (p.log_level < 2)
    paw_debug_bytes_type(d, 0);

  return d;
}

data paw_recvline(process p)
{
  data d = paw_init_data();
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


data paw_recvuntil(process p, char *delimiter, int delimiter_len)
{
  data d = paw_init_data();
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
