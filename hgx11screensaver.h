#ifndef HGX11SCREENSAVER_H
#define HGX11SCREENSAVER_H

#include <QThread>
#include <QDebug>
#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

class hgx11screensaver : public QObject
{
    Q_OBJECT
public:
    hgx11screensaver(Display *);
    ~hgx11screensaver();
    unsigned long lastX11ActivitySeconds();

private:
    Display *_display_p;
    Window _window_m;
    XScreenSaverInfo *_info_p;

};

#endif // HGX11SCREENSAVER_H
