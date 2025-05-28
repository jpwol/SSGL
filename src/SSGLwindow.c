#include "SSGLWindow.h"

#include <stdio.h>

#ifdef __linux__
#define OS_IMP 1
#elif defined(_WIN32)
#define OS_IMP 2
#endif

int SSGLInit() {
  if (OS_IMP == 1) {
    printf("found linux\n");
  } else if (OS_IMP == 2) {
    printf("found windows\n");
  } else {
    printf("unidentified os\n");
  }
  return 0;
}

SSGLWindow* SSGLCreateWindow(int width, int height, const char* title) {
  // gang? fonem?
  SSGLWindow* _w = (SSGLWindow*)malloc(sizeof(SSGLWindow));

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

  _w->window = XCreateWindow(
      _w->display, RootWindow(_w->display, 0), 10, 10, width, height, 0,
      DefaultDepth(_w->display, _w->screen), CopyFromParent,
      DefaultVisual(_w->display, _w->screen), 0, 0);

  if (!_w->window) {
    fprintf(stderr, "Failed to create X window\n");
    exit(1);
  }

  // Make the window floating by default by window managers.
  // FIXME!!!: Abstract this further away. Check for window manager or display
  // environment first before doing this. Hug me. Kiss me.
  Atom wm_window_type = XInternAtom(_w->display, "_NET_WM_WINDOW_TYPE", False);
  Atom wm_window_type_dialog =
      XInternAtom(_w->display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
  XChangeProperty(_w->display, _w->window, wm_window_type, XA_ATOM, 32,
                  PropModeReplace, (unsigned char*)&wm_window_type_dialog, 1);

  XSelectInput(_w->display, _w->window,
               ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                   ButtonReleaseMask | StructureNotifyMask);

  // Set the title of the window
  XStoreName(_w->display, _w->window, title);

  XMapWindow(_w->display, _w->window);

  _w->data = (SSGLWindowData*)malloc(sizeof(SSGLWindowData));
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

void SSGLDestroyWindow(SSGLWindow* window) {
  XShmDetach(window->display, &window->data->shminfo);
  shmdt(window->data->image->data);
  XDestroyWindow(window->display, window->window);
  XCloseDisplay(window->display);
  free(window);
}
