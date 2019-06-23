#ifndef HGX11DAMAGE_H
#define HGX11DAMAGE_H

#include <QThread>
#include <QDebug>
#include <X11/extensions/Xdamage.h>
#include <X11/Xlib.h>

class hgx11damage : public QThread
{
    Q_OBJECT
public:
    hgx11damage();
    ~hgx11damage();

private:
    Display *_display_p;
    XEvent _event_m;
    Damage _damage_m;
    bool _loop_m = true;

    void _monitor();

protected:
    void run() {
        _monitor();
    }

signals:
    void damageDetected();

};

#endif // HGX11DAMAGE_H
