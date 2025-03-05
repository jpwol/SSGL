#include "SharpWindow.h"

#include <stdio.h>

int SharpInit() { return 0; }

SharpWindow* SharpCreateWindow(int width, int height, const char* title) {
  // gang? fonem?
  SharpWindow* _w = (SharpWindow*)malloc(sizeof(SharpWindow));

  _w->w = width;
  _w->h = height;

  _w->display = XOpenDisplay(NULL);
  if (!_w->display) {
    fprintf(stderr, "Failed to open X display\n");
    exit(1);
  }

  // probably just 0 lmao
  _w->screen = DefaultScreen(_w->display);

  // _w->window = XCreateWindow(_w->display, XRootWindow(_w->display, 0), 10,
  // 10,
  //                            width, height, 0, 0, 1, NULL, 1, NULL);

  // Change to XCreateWindow as soon as I know what the fuck that is
  _w->window = XCreateSimpleWindow(
      _w->display, RootWindow(_w->display, 0), 10, 10, width, height, 1,
      BlackPixel(_w->display, _w->screen), WhitePixel(_w->display, _w->screen));

  if (!_w->window) {
    fprintf(stderr, "Failed to create X window\n");
    exit(1);
  }

  XSelectInput(_w->display, _w->window,
               ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                   ButtonReleaseMask | StructureNotifyMask);
  // Set the title of the window
  XStoreName(_w->display, _w->window, title);

  XMapWindow(_w->display, _w->window);

  _w->data = (SharpWindowData*)malloc(sizeof(SharpWindowData));
  _w->data->gc = XCreateGC(_w->display, _w->window, 0, NULL);
  _w->data->image =
      XShmCreateImage(_w->display, DefaultVisual(_w->display, _w->screen),
                      DefaultDepth(_w->display, _w->screen), ZPixmap, NULL,
                      &_w->data->shminfo, width, height);

  _w->data->shminfo.shmid =
      shmget(IPC_PRIVATE, width * height * 4, IPC_CREAT | 0777);
  if (_w->data->shminfo.shmid == -1) {
    perror("shmget failed");
    exit(1);
  }

  _w->data->image->data = (char*)shmat(_w->data->shminfo.shmid, NULL, 0);
  _w->data->shminfo.shmaddr = _w->data->image->data;

  if ((long)_w->data->shminfo.shmaddr == -1) {
    perror("shmat failed");
    exit(1);
  }

  _w->data->shminfo.readOnly = False;
  if (!XShmAttach(_w->display, &_w->data->shminfo)) {
    fprintf(stderr, "XShmAttach failed\n");
    exit(1);
  }

  return _w;
}

void SharpDestroyWindow(SharpWindow* window) {
  XShmDetach(window->display, &window->data->shminfo);
  shmdt(window->data->image->data);
  XDestroyWindow(window->display, window->window);
  XCloseDisplay(window->display);
  free(window);
}
