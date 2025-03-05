#ifndef _SHARP_EVENT_H_
#define _SHARP_EVENT_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "SharpWindow.h"

typedef enum SharpKeySym {
  SK_UNKNOWN,
  SK_ESC,
  SK_TAB,
  SK_CAPS,
  SK_LSHIFT,
  SK_RSHIFT,
  SK_LCTRL,
  SK_RCTRL,
  SK_LSUPER,
  SK_RSUPER,
  SK_LALT,
  SK_RALT,
  SK_SPACE,
  SK_RETURN,
  SK_BACKSPACE,
  SK_0,
  SK_1,
  SK_2,
  SK_3,
  SK_4,
  SK_5,
  SK_6,
  SK_7,
  SK_8,
  SK_9,
  SK_N0,
  SK_N1,
  SK_N2,
  SK_N3,
  SK_N4,
  SK_N5,
  SK_N6,
  SK_N7,
  SK_N8,
  SK_N9,
  SK_a,
  SK_b,
  SK_c,
  SK_d,
  SK_e,
  SK_f,
  SK_g,
  SK_h,
  SK_i,
  SK_j,
  SK_k,
  SK_l,
  SK_m,
  SK_n,
  SK_o,
  SK_p,
  SK_q,
  SK_r,
  SK_s,
  SK_t,
  SK_u,
  SK_v,
  SK_w,
  SK_x,
  SK_y,
  SK_z,
  SK_PERIOD,
  SK_COMMA,
  SK_FSLASH,
  SK_BSLASH,
  SK_LBRACKET,
  SK_RBRACKET,
  SK_MINUS,
  SK_EQUALS,
  SK_GRAVE,
  SK_NMINUS,
  SK_NSTAR,
  SK_NFSLASH,
  SK_NNUMLOCK,
  SK_NPLUS,
  SK_NRETURN,
  SK_NPERIOD,
  SK_F1,
  SK_F2,
  SK_F3,
  SK_F4,
  SK_F5,
  SK_F6,
  SK_F7,
  SK_F8,
  SK_F9,
  SK_F10,
  SK_F11,
  SK_F12,
  SK_UP,
  SK_DOWN,
  SK_LEFT,
  SK_RIGHT,
} SharpKey;

typedef enum SharpEventType {
  SharpKeyPress = 1,
  SharpKeyRelease,
  SharpMousePress,
  SharpMouseRelease,
  SharpWindowResize,
} SharpEventType;

typedef struct SharpEvent {
  int type;
  union {
    SharpKey keyPressed;
    SharpKey keyReleased;
  };
} SharpEvent;

#ifdef __cplusplus
extern "C" {
#endif

extern void SharpPollEvents(SharpWindow* window, SharpEvent* event);

#ifdef __cplusplus
}
#endif

#endif
