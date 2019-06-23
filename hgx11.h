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
    hgx11(QHash<QString, QString>);
    ~hgx11();

private:
    hgx11damage *_damage_p;
    hgx11grab *_grabber_p;
    hgx11net *_hclient_p;
    QTimer *_timer_p;

    QString _addr_m = "localhost";
    QString _port_m = "19444";
    QString _scale_m = "8";
    QString _frameskip_m = "0";
    QString _inactiveTime_m = "0";
    QString _redAdjust_m = "";
    QString _greenAdjust_m = "";
    QString _blueAdjust_m = "";
    QString _temperature_m = "";
    QString _threshold_m = "";
    QString _transform_m = "";
    QString _destWidth_m;
    QString _destHeight_m;

    QString _parseColorArr(QString, bool);

private slots:
    void _sendImage();
    void _inActivity();
    void _activity();
    void _setImgSize();

};

#endif // HGX11_H
