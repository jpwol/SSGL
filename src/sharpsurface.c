#include "SharpSurface.h"

SharpSurface* SharpGetWindowSurface(SharpWindow* window) {
  SharpSurface* surface = (SharpSurface*)malloc(sizeof(SharpSurface));

  surface->w = window->w;
  surface->h = window->h;

  surface->pixels = window->data->image->data;
  surface->ximage = window->data->image;

  // Not necessary for the window surface, will use for other surfaces tho
  // surface->ximage = XCreateImage(
  //     window->display, DefaultVisual(window->display, window->screen),
  //     DefaultDepth(window->display, window->screen), ZPixmap, 0,
  //     (char*)surface->pixels, surface->w, surface->h, 32, surface->w *
  //     4);

  return surface;
}

void SharpSetDrawColor(SharpSurface* surface, int r, int g, int b) {}

void SharpClearSurfaceColor(SharpSurface* surface, unsigned int color) {
  uint32_t* pixels = (uint32_t*)surface->pixels;
  size_t total_pixels = surface->w * surface->h;

  __m256i color_vec = _mm256_set1_epi32(color);
  size_t i = 0;
  for (; i + 8 <= total_pixels; i += 8) {
    _mm256_storeu_si256((__m256i*)&pixels[i], color_vec);
  }

  for (; i < total_pixels; i++) {
    pixels[i] = color;
  }
}

void SharpDrawRect(SharpSurface* surface, SharpRect* rect) {}

void SharpFillRect(SharpSurface* surface, SharpRect* rect, unsigned int color) {
  int x = rect->x;
  int w = rect->x + rect->w;
  int y = rect->y;
  int h = rect->y + rect->h;

  if (x < 0) x = 0;
  if (w > surface->w) w = surface->w;
  if (y < 0) y = 0;
  if (h > surface->h) h = surface->h;

  if (x >= w || y >= h) {
    return;
  }

  unsigned int* pixel = (unsigned int*)(surface->pixels);
  for (int j = y; j < h; j++) {
    unsigned int* row_start = pixel + j * surface->w;
    int i = x;
    for (; i + 4 <= w; i += 4) {
      row_start[i] = color;
      row_start[i + 1] = color;
      row_start[i + 2] = color;
      row_start[i + 3] = color;
    }

    for (; i < w; i++) {
      row_start[i] = color;
    }
  }
}

void SharpFillColor(SharpSurface* surface) {}

void SharpUpdateWindowSurface(SharpWindow* window) {
  XShmPutImage(window->display, window->window, window->data->gc,
               window->data->image, 0, 0, 0, 0, window->w, window->h, False);
  XSync(window->display, False);
}

void SharpBlitSurface(SharpSurface* surface) {}

void SharpDestroySurface(SharpSurface* surface) { free(surface); }
