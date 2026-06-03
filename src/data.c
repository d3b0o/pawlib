#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "internal.h"

t_data paw_init_data()
{
  return paw_init_data_size(16);
}

t_data paw_init_data_size(int size)
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

void paw_add_data(t_data *p, uint8_t *data, int size)
{ 
  for (int i=0; i<size; i++){
    paw_append_byte(p, data[i]);
  }
}

void paw_add_repeat_data(t_data *p, uint8_t *data, int size, int times)
{
  for (int i=0; i<times; i++)
  {
    paw_add_data(p, data, size);
  }  
}

int paw_append_byte(t_data *d, uint8_t c)
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
