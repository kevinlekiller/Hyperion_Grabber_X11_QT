#include "hgx11damage.h"

// public

hgx11damage::hgx11damage()
{
    _display_p = XOpenDisplay(nullptr);
    if(!_display_p){
        qCritical() << "failed to open x display";
        return;
    }
    Window window = DefaultRootWindow(_display_p);
    _damage_m = XDamageCreate(_display_p, window, XDamageReportNonEmpty);

}

hgx11damage::~hgx11damage()
{
    _loop_m = false;
}

// protected

void hgx11damage::_monitor()
{
    if(!_display_p){
        return;
    }
    while (_loop_m) {
        XNextEvent(_display_p, &_event_m);
        emit damageDetected();
        XDamageSubtract(_display_p ,_damage_m, None, None);
    }
}
