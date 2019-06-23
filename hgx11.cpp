#include "hgx11.h"

// public

hgx11::hgx11(QString addr, QString port, QString scale, QString frameskip, QString inactiveTime)
{
    _addr_m = (addr.isEmpty() || addr.isNull()) ? "localhost" : addr;
    _port_m = (port.toInt() > 65535 || port.toInt() < 1) ? "19444" : port;
    _scale_m = (scale.toInt() < 1 || scale.toInt() > 1000) ? "4" : scale;
    _frameskip_m = (frameskip.toInt() > 0 && frameskip.toInt() < 256) ? frameskip : "0";
    _inactiveTime_m = (inactiveTime.toInt() > 0 ? QString::number(inactiveTime.toInt() * 1000) : "0");

    _grabber_p = new hgx11grab(_scale_m.toInt(), _frameskip_m.toUShort());
    _hclient_p = new hgx11net(_addr_m, _port_m.toUShort());
    _damage_p = new hgx11damage();

    _hclient_p->imgWidth = QString::number(_grabber_p->getDest_width());
    _hclient_p->imgHeight = QString::number(_grabber_p->getDest_height());

    _damage_p->start();

    connect(_damage_p, SIGNAL(damageDetected()), _grabber_p, SLOT(grabFrame()));
    connect(_grabber_p, SIGNAL(scaleChanged()), this, SLOT(_setImgSize()));
    connect(_grabber_p, SIGNAL(imageCreated()), this, SLOT(_sendImage()));

    if (_inactiveTime_m.toInt()) {
        _timer_p = new QTimer(this);
        connect(_timer_p, SIGNAL(timeout()), this, SLOT(_inActivity()));
        _timer_p->start(_inactiveTime_m.toInt());
        connect(_damage_p, SIGNAL(damageDetected()), this, SLOT(_activity()));
    }
}

hgx11::~hgx11()
{
    delete _hclient_p;
    _hclient_p = nullptr;
    _damage_p->terminate();
    while (!_damage_p->isFinished()) {}
    delete _damage_p;
    _damage_p = nullptr;
    delete _grabber_p;
    _grabber_p = nullptr;
}

// private slots

void hgx11::_sendImage()
{
    _hclient_p->sendImage(&_grabber_p->imgdata_m);
}

void hgx11::_inActivity()
{
    _hclient_p->clearLeds();
}

void hgx11::_activity()
{
    _timer_p->stop();
    _timer_p->start(_inactiveTime_m.toInt());
}

void hgx11::_setImgSize()
{
    _hclient_p->imgWidth = QString::number(_grabber_p->getDest_width());
    _hclient_p->imgHeight = QString::number(_grabber_p->getDest_height());
}
