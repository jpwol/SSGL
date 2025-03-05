#include "SharpEvent.h"

SharpKey SharpGetKey(XKeyEvent* event) {
  KeySym keysym = XLookupKeysym(event, 0);
  KeySym keysympad = XLookupKeysym(event, 1);

  if (keysym >= XK_0 && keysym <= XK_9)
    return (SharpKey)(keysym - XK_0 + (int)(SK_0));

  if (keysym >= XK_a && keysym <= XK_z)
    return (SharpKey)(keysym - XK_a + (int)(SK_a));

  if (keysympad >= XK_KP_0 && keysympad <= XK_KP_9)
    return (SharpKey)(keysympad - XK_KP_0 + (int)(SK_N0));

  switch (keysym) {
    case XK_Escape:
      return SK_ESC;
    case XK_Tab:
      return SK_TAB;
    case XK_Caps_Lock:
      return SK_CAPS;
    case XK_Shift_L:
      return SK_LSHIFT;
    case XK_Shift_R:
      return SK_RSHIFT;
    case XK_Control_L:
      return SK_LCTRL;
    case XK_Control_R:
      return SK_RCTRL;
    case XK_Super_L:
      return SK_LSUPER;
    case XK_Super_R:
      return SK_RSUPER;
    case XK_Alt_L:
      return SK_LALT;
    case XK_Alt_R:
      return SK_RALT;
    case XK_space:
      return SK_SPACE;
    case XK_Return:
      return SK_RETURN;
    case XK_BackSpace:
      return SK_BACKSPACE;
    case XK_period:
      return SK_PERIOD;
    case XK_comma:
      return SK_COMMA;
    case XK_slash:
      return SK_FSLASH;
    case XK_backslash:
      return SK_BSLASH;
    case XK_bracketleft:
      return SK_LBRACKET;
    case XK_bracketright:
      return SK_RBRACKET;
    case XK_equal:
      return SK_EQUALS;
    case XK_grave:
      return SK_GRAVE;
    case XK_minus:
      return SK_NMINUS;  // maps to main minus
    case XK_Num_Lock:
      return SK_NNUMLOCK;
    case XK_Up:
      return SK_UP;
    case XK_Down:
      return SK_DOWN;
    case XK_Left:
      return SK_LEFT;
    case XK_Right:
      return SK_RIGHT;
    case XK_KP_Divide:
      return SK_NFSLASH;
    case XK_KP_Multiply:
      return SK_NSTAR;
    case XK_KP_Subtract:
      return SK_NMINUS;
    case XK_KP_Add:
      return SK_NPLUS;
    case XK_KP_Enter:
      return SK_NRETURN;
    case XK_KP_Decimal:
      return SK_NPERIOD;
    case XK_KP_0:
      return SK_N0;
    default:
      return SK_UNKNOWN;
  }
}

void SharpPollEvents(SharpWindow* window, SharpEvent* event) {
  XEvent x_event;
  while (XPending(window->display) > 0) {
    XNextEvent(window->display, &x_event);

    switch (x_event.type) {
      case KeyPress:
        event->type = SharpKeyPress;
        event->keyPressed = SharpGetKey(&x_event.xkey);
        break;
      case KeyRelease:
        event->type = SharpKeyRelease;
        event->keyReleased = SharpGetKey(&x_event.xkey);
        break;
      case ButtonPress:
        event->type = SharpMousePress;
        break;
      case ButtonRelease:
        event->type = SharpMouseRelease;
        break;
      default:
        break;
    }
  }
}
