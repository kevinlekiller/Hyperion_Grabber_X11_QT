#ifndef HGX11DAMAGE_H
#define HGX11DAMAGE_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <X11/extensions/Xdamage.h>

class hgx11damage : public QThread
{
    Q_OBJECT
public:

    hgx11damage(QWaitCondition *, unsigned short);
    ~hgx11damage();

private:
    QWaitCondition * _grabbed_p;
    QMutex _mutex_m;
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
