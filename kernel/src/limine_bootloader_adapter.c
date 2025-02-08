// Using Limine

#include "framebuffer/framebuffer.h"
#include <stdbool.h>
#include <stddef.h>

#include "../deps/limine/limine.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((
    used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
               section(".limine_requests_"
                       "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
    used,
    section(
        ".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
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

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void boot_start(void)
{
  /*
  // Ensure the bootloader actually understands our base revision (see spec).
  if (LIMINE_BASE_REVISION_SUPPORTED == false)
  {
    hcf();
  }

  // Ensure we got a framebuffer.
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1)
  {
    hcf();
  }

  // Fetch the first framebuffer.
  struct limine_framebuffer* framebuffer =
      framebuffer_request.response->framebuffers[0];

  // Note: we assume the framebuffer model is RGB with 32-bit pixels.
  for (size_t i = 0; i < 100; i++)
  {
    volatile uint32_t* fb_ptr = framebuffer->address;
    fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
  }*/

  // We're done, just hang...
  // hcf();

  init_framebuffer();

  __asm__("jmp kernel_start");
}