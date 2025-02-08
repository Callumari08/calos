#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

// A bootloader-agnostic framebuffer structure.
typedef struct Framebuffer {
    void *base;      // Base memory address of the framebuffer
    uint32_t width;  // Screen width in pixels
    uint32_t height; // Screen height in pixels
    uint32_t pitch;  // Number of bytes per row
    uint32_t bpp;    // Bits per pixel
} Framebuffer;

// Global framebuffer instance that both the bootloader adapter and kernel code use.
extern Framebuffer fb;

// A simple drawing function that writes a pixel.
static inline void draw_pixel(const Framebuffer *fb, uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb->width || y >= fb->height) return;
    uint8_t *pixel = (uint8_t *)fb->base + y * fb->pitch + x * (fb->bpp / 8);
    *(uint32_t *)pixel = color;
}

#endif
