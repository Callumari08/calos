#include "framebuffer/framebuffer.h"
#include <stdint.h>

inline void hcf()
{
  for (;;)
  {
    __asm__("hlt");
  }
}

void kernel_start()
{
  draw_pixel(&fb, fb.width / 2, fb.height / 2, 0xFFFFFFFF);
  hcf();
}