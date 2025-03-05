#ifndef _SHARP_SURFACE_H_
#define _SHARP_SURFACE_H_

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <immintrin.h>
#include <stdlib.h>

#include "SharpRect.h"
#include "SharpWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SharpSurface {
  int w, h;
  int pitch;
  void* pixels;
  XImage* ximage;
} SharpSurface;

SharpSurface* SharpGetWindowSurface(SharpWindow* window);
void SharpSetDrawColor(SharpSurface* surface, int r, int g, int b);
void SharpClearSurfaceColor(SharpSurface* surface, unsigned int color);
void SharpDrawRect(SharpSurface* surface, SharpRect* rect);
void SharpFillRect(SharpSurface* surface, SharpRect* rect, unsigned int color);
void SharpFillColor(SharpSurface* surface);
void SharpDestroySurface(SharpSurface* surface);
void SharpUpdateWindowSurface(SharpWindow* window);
void SharpBlitSurface(SharpSurface* surface);

#ifdef __cplusplus
}
#endif

#endif
