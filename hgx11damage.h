#ifndef HGX11DAMAGE_H
#define HGX11DAMAGE_H

#include <QThread>
#include <QDebug>
#include <X11/extensions/Xdamage.h>

class hgx11damage : public QThread
{
    Q_OBJECT
public:
    hgx11damage(unsigned short);
    ~hgx11damage();

private:
    Display *_display_p;
    XEvent _event_m;
    Damage _damage_m;
    bool _loop_m = true;
    ushort _frameCount_m = 255;
    ushort _frameSkip_m = 0;

    void _monitor();

protected:
    void run() {
        _monitor();
    }

signals:
    void damageDetected();

};

#endif // HGX11DAMAGE_H
