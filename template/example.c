#include <pwnlib.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  t_process p = open_process("ls");
  p.log_level = 0;

  recv(p, 2);
  recvuntil(p, "src", 3);
  recvline(p);
  recvline(p);

  t_data payload = init_payload();
  payload_add(&payload, "HELLO\n\x00", 6);

  printf("%s", payload.data);

  sendline(p, payload);
}
