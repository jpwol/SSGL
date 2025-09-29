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

/* clang-format off */
static Window SSGLCreateXWindow(SSGLWindow* win, const char* title) {
 win->window = XCreateWindow(
    win->display,
    RootWindow(win->display, 0),
    10, 10,
    win->w, win->h,
    0,
    DefaultDepth(win->display, win->screen),
    CopyFromParent,
    DefaultVisual(win->display, win->screen),
    0,
    0
  );

  if (!win->window) {
    fprintf(stderr, "Failed to create X window\n");
    exit(1);
  }
  
  XStoreName(win->display, win->window, title);
  XSelectInput(win->display, win->window,
               ExposureMask | KeyPressMask | KeyReleaseMask |
               ButtonPressMask | ButtonReleaseMask | StructureNotifyMask);

  return win->window;
}

/* clang-format on */

static void SSGLSetWindowType(SSGLWindow* win, SSGLWindowType wtype) {
  Atom wm_window_type = XInternAtom(win->display, "_NET_WM_WINDOW_TYPE", False);
  Atom atom;

  switch (wtype) {
    case SSGL_WINDOW_TYPE_DIALOG:
      atom = XInternAtom(win->display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
      XChangeProperty(win->display, win->window, wm_window_type, XA_ATOM, 32,
                      PropModeReplace, (unsigned char*)&atom, 1);
      break;
    case SSGL_WINDOW_TYPE_FULLSCREEN: {
      Atom wm_state = XInternAtom(win->display, "_NET_WM_STATE", False);
      Atom fullscreen =
          XInternAtom(win->display, "_NET_WM_STATE_FULLSCREEN", False);
      XChangeProperty(win->display, win->window, wm_state, XA_ATOM, 32,
                      PropModeReplace, (unsigned char*)&fullscreen, 1);
    } break;
    case SSGL_WINDOW_TYPE_NORMAL:
    default:
      break;
  }
}

static SSGLSurfaceData* SSGLCreateSurfaceData(SSGLWindow* win) {
  SSGLSurfaceData* data = malloc(sizeof(SSGLSurfaceData));
  data->gc = XCreateGC(win->display, win->window, 0, NULL);
  data->image =
      XShmCreateImage(win->display, DefaultVisual(win->display, win->screen),
                      DefaultDepth(win->display, win->screen), ZPixmap, NULL,
                      &data->shminfo, win->w, win->h);
  data->shminfo.shmid =
      shmget(IPC_PRIVATE, win->w * win->h * 4, IPC_CREAT | 0777);
  if (data->shminfo.shmid == -1) {
    perror("shmget failed");
    exit(1);
  }

  data->image->data = shmat(data->shminfo.shmid, NULL, 0);
  data->shminfo.shmaddr = data->image->data;

  if ((long)data->shminfo.shmaddr == -1) {
    perror("shmat failed");
    exit(1);
  }

  data->shminfo.readOnly = False;

  if (!XShmAttach(win->display, &data->shminfo)) {
    fprintf(stderr, "XShmAttach failed\n");
    exit(1);
  }

  return data;
}

SSGLWindow* SSGLCreateWindow(int width, int height, const char* title,
                             SSGLWindowType wtype) {
  SSGLWindow* win = malloc(sizeof(SSGLWindow));
  win->w = width;
  win->h = height;

  win->display = XOpenDisplay(NULL);
  if (!win->display) {
    fprintf(stderr, "Failed to open X display\n");
    exit(1);
  }

  win->screen = DefaultScreen(win->display);
  win->window = SSGLCreateXWindow(win, title);

  win->data = SSGLCreateSurfaceData(win);
  XMapWindow(win->display, win->window);
  SSGLSetWindowType(win, wtype);

  return win;
}

// SSGLWindow* SSGLCreateWindow(int width, int height, const char* title) {
//   // gang? fonem?
//
//   // SSGLWindow* _w = (SSGLWindow*)malloc(sizeof(SSGLWindow));
//   SSGLWindow* _w = malloc(sizeof(SSGLWindow));
//
//   _w->w = width;
//   _w->h = height;
//
//   _w->display = XOpenDisplay(NULL);
//   if (_w->display == NULL) {
//     fprintf(stderr, "Failed to open X display\n");
//     exit(1);
//   }
//
//   // probably just 0
//   _w->screen = DefaultScreen(_w->display);
//
//   // _w->window = XCreateWindow(_w->display, XRootWindow(_w->display, 0), 10,
//   // 10,
//   //                            width, height, 0, 0, 1, NULL, 1, NULL);
//
//   // Change to XCreateWindow as soon as I know what the fuck that is
//   // _w->window = XCreateSimpleWindow(
//   //     _w->display, RootWindow(_w->display, 0), 10, 10, width, height, 1,
//   //     BlackPixel(_w->display, _w->screen), WhitePixel(_w->display,
//   //     _w->screen));
//
//   _w->window = XCreateWindow(
//       _w->display, RootWindow(_w->display, 0), 10, 10, width, height, 0,
//       DefaultDepth(_w->display, _w->screen), CopyFromParent,
//       DefaultVisual(_w->display, _w->screen), 0, 0);
//
//   if (!_w->window) {
//     fprintf(stderr, "Failed to create X window\n");
//     exit(1);
//   }
//
//   // Make the window floating by default by window managers.
//   // FIXME!!!: Abstract this further away. Check for window manager or
//   display
//   // environment first before doing this. Hug me. Kiss me.
//   Atom wm_window_type = XInternAtom(_w->display, "_NET_WM_WINDOW_TYPE",
//   False); Atom wm_window_type_dialog =
//       XInternAtom(_w->display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
//   XChangeProperty(_w->display, _w->window, wm_window_type, XA_ATOM, 32,
//                   PropModeReplace, (unsigned char*)&wm_window_type_dialog,
//                   1);
//
//   XSelectInput(_w->display, _w->window,
//                ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask
//                |
//                    ButtonReleaseMask | StructureNotifyMask);
//
//   // Set the title of the window
//   XStoreName(_w->display, _w->window, title);
//
//   XMapWindow(_w->display, _w->window);
//
//   _w->data = malloc(sizeof(SSGLWindowData));
//   _w->data->gc = XCreateGC(_w->display, _w->window, 0, NULL);
//   _w->data->image =
//       XShmCreateImage(_w->display, DefaultVisual(_w->display, _w->screen),
//                       DefaultDepth(_w->display, _w->screen), ZPixmap, NULL,
//                       &_w->data->shminfo, width, height);
//
//   _w->data->shminfo.shmid =
//       shmget(IPC_PRIVATE, width * height * 4, IPC_CREAT | 0777);
//   if (_w->data->shminfo.shmid == -1) {
//     perror("shmget failed");
//     exit(1);
//   }
//
//   _w->data->image->data = (char*)shmat(_w->data->shminfo.shmid, NULL, 0);
//   _w->data->shminfo.shmaddr = _w->data->image->data;
//
//   if ((long)_w->data->shminfo.shmaddr == -1) {
//     perror("shmat failed");
//     exit(1);
//   }
//
//   _w->data->shminfo.readOnly = False;
//   if (!XShmAttach(_w->display, &_w->data->shminfo)) {
//     fprintf(stderr, "XShmAttach failed\n");
//     exit(1);
//   }
//
//   return _w;
// }

void SSGLDestroyWindow(SSGLWindow* window) {
  XShmDetach(window->display, &window->data->shminfo);
  shmdt(window->data->image->data);
  XDestroyWindow(window->display, window->window);
  XCloseDisplay(window->display);
  free(window);
}
