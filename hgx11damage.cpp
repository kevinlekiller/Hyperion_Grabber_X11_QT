#include "hgx11damage.h"

// public

hgx11damage::hgx11damage(QWaitCondition * grabbed, unsigned short frameSkip)
{
    _grabbed_p = grabbed;
    _frameSkip_m = frameSkip;
    _display_p = XOpenDisplay(nullptr);
    if(_display_p == nullptr){
        qCritical() << "Failed to open X11 display";
        return;
    }
    _damage_m = XDamageCreate(_display_p, DefaultRootWindow(_display_p), XDamageReportNonEmpty);
}

hgx11damage::~hgx11damage()
{
    _loop_m = false;
    if (_damage_m) {
        XDamageDestroy(_display_p, _damage_m);
    }
}

// protected

void hgx11damage::_monitor()
{
    if(_display_p == nullptr){
        return;
    }
    while (_loop_m) {
        XNextEvent(_display_p, &_event_m);
        if (_frameCount_m++ < _frameSkip_m) {
            XDamageSubtract(_display_p ,_damage_m, None, None);
            continue;
        }
        _mutex_m.lock();
        emit damageDetected();
        _grabbed_p->wait(&_mutex_m);
        _mutex_m.unlock();
        XDamageSubtract(_display_p ,_damage_m, None, None);
        _frameCount_m = 0;
    }
}
