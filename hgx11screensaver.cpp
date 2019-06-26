#include "hgx11screensaver.h"

// public

hgx11screensaver::hgx11screensaver(Display *display)
{
    _display_p = display;
    _window_m =  DefaultRootWindow(_display_p);
    _info_p = XScreenSaverAllocInfo();
}

hgx11screensaver::~hgx11screensaver()
{
    XFree(_info_p);
}

unsigned long hgx11screensaver::lastX11ActivitySeconds()
{
    if (!_info_p) {
        return 0;
    }
    XScreenSaverQueryInfo(_display_p, _window_m, _info_p);
    return _info_p->idle;
}

