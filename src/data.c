#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

t_data init_data_d()
{
  return init_data(16);
}

t_data init_payload()
{
  return init_data(16);
}

t_data init_data(int size)
{
  t_data d;

  d.len = 0;
  d.size = size;
  d.data = malloc(sizeof(uint8_t) * d.size);

  if (!d.data) {
    return d;
  }
  return d;
}

void payload_append(t_data *p, uint8_t *data, int size)
{ 
  for (int i=0; i<size; i++){
    append_byte(p, data[i]);
  }
}

void payload_repeat(t_data *p, uint8_t *data, int size, int times)
{
  for (int i=0; i<times; i++)
  {
    payload_add(p, data, size);
  }  
}

int append_byte(t_data *d, uint8_t c)
{
  if (d->len >= d->size)
  {
    d->size += d->size;
    uint8_t *new_buff = realloc(d->data, sizeof(uint8_t) * d->size);
    if (!new_buff) {
      return 1;
    }
    d->data = new_buff;
  }

  d->data[d->len++] = c;
  return 0;
}

void clean_data(t_data d)
{
  free(d.data);
}
