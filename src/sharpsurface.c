#include "SharpSurface.h"

SharpSurface* SharpGetWindowSurface(SharpWindow* window) {
  SharpSurface* s = (SharpSurface*)malloc(sizeof(SharpSurface));

  s->w = window->w;
  s->h = window->h;

  s->pixels = window->data->image->data;
  s->ximage = window->data->image;

  return s;
}

SharpSurface* SharpCreateSurface(SharpWindow* window, int width, int height) {
  SharpSurface* s = (SharpSurface*)malloc(sizeof(SharpSurface));

  // NEED TO ALLOCATE shminfo for SURFACE
  //
  // CONSIDER CHANGING WINDOW TO HOLD A SHARP SURFACE INSTEAD OF XShmSegmentInfo
  // AND PUT XShmSegmentInfo into SURFACE STRUCT      MAYBE??
  s->ximage = XShmCreateImage(window->display,
                              DefaultVisual(window->display, window->screen),
                              DefaultDepth(window->display, window->screen),
                              ZPixmap, NULL, NULL, width, height);

  return s;
}

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

void SharpDrawPoint(SharpSurface* surface, SharpPoint* point,
                    unsigned int color) {
  if (point->x < 0 || point->y < 0 || point->x >= surface->w ||
      point->y >= surface->h)
    return;

  ((unsigned int*)surface->pixels)[point->y * surface->w + point->x] = color;
}

void SharpDrawCircle(SharpSurface* surface, SharpPoint* point, int radius,
                     unsigned int color) {
  int x = radius;
  int y = 0;
  int decision = 1 - radius;

  int width = surface->w;
  int height = surface->h;

  unsigned int* pixels = (unsigned int*)surface->pixels;

  while (x >= y) {
    // precompute each group of points for every iteration
    int px[] = {point->x + x, point->x - x, point->x + x, point->x - x,
                point->x + y, point->x - y, point->x + y, point->x - y};

    int py[] = {point->y + y, point->y + y, point->y - y, point->y - y,
                point->y + x, point->y + x, point->y - x, point->y - x};

    for (int i = 0; i < 8; i++) {
      // Bounds check each point to make sure they're within the surface
      int mask_x = px[i] >= 0 && px[i] < width;
      int mask_y = py[i] >= 0 && py[i] < height;
      int index = (py[i] * width) + px[i];

      // lil bit o' bit math; if either mask evaluates to false, they both do,
      // which zeroes out the index
      index &= -(mask_x & mask_y);
      pixels[index] = color;

      // branchless programming fucking sucks
      // also, two's complement fucking sucks. signed integers can suck me
    }

    // ((unsigned int*)
    //      surface->pixels)[((point->y + y) * surface->w) + (point->x + x)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y + y) * surface->w) + (point->x - x)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y - y) * surface->w) + (point->x + x)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y - y) * surface->w) + (point->x - x)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y + x) * surface->w) + (point->x + y)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y + x) * surface->w) + (point->x - y)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y - x) * surface->w) + (point->x + y)] =
    //     color;
    // ((unsigned int*)
    //      surface->pixels)[((point->y - x) * surface->w) + (point->x - y)] =
    //     color;

    y++;

    if (decision <= 0) {
      decision += 2 * y + 1;
    } else {
      x--;
      decision += 2 * (y - x) + 1;
    }
  }
}

void SharpUpdateWindowSurface(SharpWindow* window) {
  XShmPutImage(window->display, window->window, window->data->gc,
               window->data->image, 0, 0, 0, 0, window->w, window->h, False);
  XSync(window->display, False);
}

void SharpBlitSurface(SharpSurface* surface) {}

void SharpDestroySurface(SharpSurface* surface) { free(surface); }
