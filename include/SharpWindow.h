#ifndef _SHARP_WINDOW_H_
#define _SHARP_WINDOW_H_

#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/shm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SharpWindowData {
  XImage* image;
  GC gc;
  XShmSegmentInfo shminfo;
} SharpWindowData;

typedef struct SharpWindow {
  Display* display;
  Window window;
  int screen;
  unsigned int w;
  unsigned int h;
  SharpWindowData* data;
} SharpWindow;

extern int SharpInit();

extern SharpWindow* SharpCreateWindow(int width, int height, const char* title);
extern void SharpDestroyWindow(SharpWindow* window);

#ifdef __cplusplus
}
#endif

#endif
