#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "internal.h"

void paw_debug_bytes(t_data d)
{
  paw_debug_bytes_type(d, 3);
}

void paw_debug_bytes_type(t_data d, int via)
{
  int bytes_per_line;
  int max_bytes_per_line = 16;
  int x;
  char current_byte;
  int other = 50;
  size_t max_index = d.len - 1;
  struct winsize w;

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  bytes_per_line = (w.ws_col - 20) / 4;
  bytes_per_line = (bytes_per_line / 4) * 4;

  if (bytes_per_line > max_bytes_per_line || bytes_per_line < 4) {
    bytes_per_line = max_bytes_per_line;
  }

  for (int i=0; i<d.len; i+=bytes_per_line)
  {
    if (via == 0) {
      printf(ANSI_COLOR_GRAY "-> " ANSI_COLOR_RESET);
    } else if (via == 1) {
      printf(ANSI_COLOR_GRAY "<- " ANSI_COLOR_RESET);
    } else {
      printf(ANSI_COLOR_GRAY "-- " ANSI_COLOR_RESET);
    }
    printf(ANSI_COLOR_GRAY "%0.8x: " ANSI_COLOR_RESET, i);

    for (x=0; x<bytes_per_line; x++)
    {
      if (x % 4 == 0 && x > 0) {
        printf(" ");
      }
      if (i+x > max_index) {
        printf("   ");
      } else {
        current_byte = d.data[i+x];
        if (current_byte > 33 && current_byte < 126) {
          printf("%2x ", current_byte);
        } else {
          printf(ANSI_COLOR_RED "%0.2x " ANSI_COLOR_RESET,  current_byte);
        }
      }
    }

    printf("| ");
    for (x=0; x<bytes_per_line; x++)
    {
      if (i+x > max_index) {break;}
      current_byte = d.data[i+x];
      if (current_byte > 33 && current_byte < 126) {
        printf("%c", current_byte);
      }
      else {
        printf(ANSI_COLOR_RED "@" ANSI_COLOR_RESET);
      }
    }
    printf("\n");
  }

  printf("\n");
}

