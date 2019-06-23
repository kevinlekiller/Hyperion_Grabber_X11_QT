#ifndef HGX11_H
#define HGX11_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "hgx11grab.h"
#include "hgx11net.h"
#include "hgx11damage.h"


class hgx11 : public QObject
{
    Q_OBJECT
public:
    hgx11(QString, QString, QString, QString, QString);
    ~hgx11();

private:
    hgx11damage *_damage_p;
    hgx11grab *_grabber_p;
    hgx11net *_hclient_p;
    QTimer *_timer_p;

    QString _addr_m;
    QString _port_m;
    QString _scale_m;
    QString _frameskip_m;
    QString _inactiveTime_m;
    QString _destWidth_m;
    QString _destHeight_m;

private slots:
    void _sendImage();
    void _inActivity();
    void _activity();
    void _setImgSize();

};

#endif // HGX11_H
