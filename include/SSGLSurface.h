#ifndef _SHARP_SURFACE_H_
#define _SHARP_SURFACE_H_

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <immintrin.h>
#include <stdlib.h>

#include "SSGLPoint.h"
#include "SSGLRect.h"
#include "SSGLWindow.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SSGLSurface {
  int w, h;
  int pitch;
  void* pixels;
  XImage* ximage;
} SSGLSurface;

SSGLSurface* SSGLGetWindowSurface(SSGLWindow* window);
SSGLSurface* SSGLCreateSurface(SSGLWindow* window, int width, int height);

void SSGLClearSurfaceColor(SSGLSurface* surface, unsigned int color);

void SSGLDrawPoint(SSGLSurface* surface, SSGLPoint* point, unsigned int color);

void SSGLDrawLine(SSGLSurface* surface, int x1, int y1, int x2, int y2,
                  unsigned int color);

void SSGLDrawRect(SSGLSurface* surface, SSGLRect* rect);

void SSGLFillRect(SSGLSurface* surface, SSGLRect* rect, unsigned int color);

void SSGLDrawCircle(SSGLSurface* surface, SSGLPoint* point, int radius,
                    unsigned int color);

void SSGLUpdateWindowSurface(SSGLWindow* window);

void SSGLBlitSurface(SSGLSurface* surface);

void SSGLDestroySurface(SSGLSurface* surface);

#ifdef __cplusplus
}
#endif

#endif
