#include <pawlib.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  process p = paw_open_process("ls");
  p.log_level = 0;

  paw_recv(p, 2);
  paw_recvuntil(p, "src", 3);
  paw_recvline(p);
  paw_recvline(p);

  data payload = paw_init_data();
  paw_add(&payload, "HELLO\n\x00", 6);

  paw_debug(payload);

  paw_add_repeat(&payload, "AB", 2, 10);
  
  paw_sendline(p, payload);
}
