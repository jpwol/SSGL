#ifndef _SHARP_WINDOW_H_
#define _SHARP_WINDOW_H_

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/shm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SSGLWindowData {
  XImage* image;
  GC gc;
  XShmSegmentInfo shminfo;
} SSGLWindowData;

typedef struct SSGLWindow {
  Display* display;
  Window window;
  int screen;
  unsigned int w;
  unsigned int h;
  SSGLWindowData* data;
} SSGLWindow;

extern int SSGLInit();

extern SSGLWindow* SSGLCreateWindow(int width, int height, const char* title);
extern void SSGLDestroyWindow(SSGLWindow* window);

#ifdef __cplusplus
}
#endif

#endif
