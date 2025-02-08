#include "../deps/limine/limine.h"
#include "low_level_rendering/frame_buffer.h"
#include <stdbool.h>
#include <stddef.h>

// Set the base revision to 3.
__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// Request a framebuffer from Limine.
__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0,
};

// Markers to ensure the requests aren’t optimized away.
__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;
__attribute__((
    used,
    section(
        ".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

static void hcf(void)
{
  for (;;)
  {
    __asm__("hlt");
  }
}

// This function converts Limine’s framebuffer info to our generic framebuffer.
static void init_framebuffer(void)
{
  // Check that Limine supports our base revision.
  if (LIMINE_BASE_REVISION_SUPPORTED == false)
  {
    hcf();
  }

  // Ensure a framebuffer was provided.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1)
  {
    hcf();
  }

  // For simplicity, pick the first framebuffer.
  struct limine_framebuffer* limine_fb =
      framebuffer_request.response->framebuffers[0];

  // Populate our generic framebuffer structure.
  fb.base = (void*)limine_fb->address;
  fb.width = limine_fb->width;
  fb.height = limine_fb->height;
  fb.pitch = limine_fb->pitch;
  fb.bpp = limine_fb->bpp;
}

void boot_start()
{
  init_framebuffer();
  // Jump to the kernel entry point. The kernel can now use the global 'fb'.
  __asm__("jmp kernel_start");
}
