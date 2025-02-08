#include "low_level_rendering/frame_buffer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static void hcf(void)
{
  for (;;)
  {
    __asm__("hlt");
  }
}

/// This must be jumped to from a bootloader adapter.
/// The linker uses the label `boot_start` as the entry point.
/// This means that this code is bootloader agnostic.
void kernel_start()
{
  // For example, draw a white pixel at the center of the screen.
  draw_pixel(&fb, fb.width / 2, fb.height / 2, 0xFFFFFFFF);

  hcf();
}