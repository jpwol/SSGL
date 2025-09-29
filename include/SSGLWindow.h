#ifndef _SSGL_WINDOW_H_
#define _SSGL_WINDOW_H_

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/shm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SSGL_WINDOW_TYPE_NORMAL,
  SSGL_WINDOW_TYPE_DIALOG,
  SSGL_WINDOW_TYPE_FULLSCREEN,
} SSGLWindowType;

typedef struct SSGLSurfaceData {
  XImage* image;
  GC gc;
  XShmSegmentInfo shminfo;
} SSGLSurfaceData;

typedef struct SSGLWindow {
  Display* display;
  Window window;
  int screen;
  unsigned int w;
  unsigned int h;
  SSGLSurfaceData* data;
} SSGLWindow;

extern int SSGLInit();

extern SSGLWindow* SSGLCreateWindow(int width, int height, const char* title,
                                    SSGLWindowType wtype);
extern void SSGLDestroyWindow(SSGLWindow* window);

#ifdef __cplusplus
}
#endif

#endif
