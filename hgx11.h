#ifndef HGX11_H
#define HGX11_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "hgx11grab.h"
#include "hgx11net.h"
#include "hgx11damage.h"
#include "hgx11screensaver.h"

class hgx11 : public QObject
{
    Q_OBJECT
public:
    hgx11(QHash<QString, QString>);
    ~hgx11();

private:
    QWaitCondition _grabbed_m;
    Display *_display_p;
    hgx11damage *_damage_p;
    hgx11grab *_grabber_p;
    hgx11net *_hclient_p;
    hgx11screensaver *_screensaver_p;
    QTimer *_timer_p;

    int _inactiveTime_m = 0;
    unsigned long _inactiveTimeXss_m = 0;
    bool _inactiveXss_m = false;
    bool _grabActive_m;

    QString _parseColorArr(QString, bool);

private slots:
    void _grabImage();
    void _sendImage();
    void _inActivity();
    void _activity();
    void _setImgSize();
    void _checkXss();
};

#endif // HGX11_H
