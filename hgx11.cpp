#include "hgx11.h"

// public

hgx11::hgx11(QHash<QString, QString> opts)
{
    QHashIterator<QString, QString> i(opts);
    while (i.hasNext()) {
        i.next();
        if ((i.key() == "a" || i.key() == "address") && !(i.value().isNull() && i.value().isEmpty())) {
             _addr_m = i.value();
        } else if ((i.key() == "p" || i.key() == "port") && (i.value().toInt() > 0 && i.value().toInt() < 65536)) {
            _port_m = i.value();
        } else if ((i.key() == "s" || i.key() == "scale") && (i.value().toInt() > 0 && i.value().toInt() < 1001)) {
            _scale_m = i.value();
        } else if ((i.key() == "f" || i.key() == "frameskip") && (i.value().toInt() > 0 && i.value().toInt() < 256)) {
             _frameskip_m = i.value();
        } else if ((i.key() == "i" || i.key() == "inactive") && (i.value().toInt() > 0)) {
            _inactiveTime_m = (i.value().toInt() * 1000);
        } else if (i.key() == "r" || i.key() == "redadjust") {
            _redAdjust_m = _parseColorArr(i.value(), 1);
        } else if (i.key() == "g" || i.key() == "greenadjust") {
            _greenAdjust_m = _parseColorArr(i.value(), 1);
        } else if (i.key() == "b" || i.key() == "blueadjust") {
            _blueAdjust_m = _parseColorArr(i.value(), 1);
        } else if (i.key() == "t" || i.key() == "temperature") {
            _temperature_m = _parseColorArr(i.value(), 1);
        } else if (i.key() == "d" || i.key() == "threshold") {
            _threshold_m = _parseColorArr(i.value(), 0);
        }else if ((i.key() == "l" || i.key() == "transform") && _parseColorArr(i.value(), 0) != "") {
            _transform_m = i.value();
        }
    }

    _grabber_p = new hgx11grab(_scale_m.toInt(), _frameskip_m.toUShort());
    _hclient_p = new hgx11net(_addr_m, _port_m.toUShort());
    _damage_p = new hgx11damage();

    _hclient_p->imgWidth = QString::number(_grabber_p->getDest_width());
    _hclient_p->imgHeight = QString::number(_grabber_p->getDest_height());
    _hclient_p->ledAdjustments(_redAdjust_m, _greenAdjust_m, _blueAdjust_m, _temperature_m, _threshold_m, _transform_m);

    _damage_p->start();

    connect(_damage_p, SIGNAL(damageDetected()), _grabber_p, SLOT(grabFrame()));
    connect(_grabber_p, SIGNAL(scaleChanged()), this, SLOT(_setImgSize()));
    connect(_grabber_p, SIGNAL(imageCreated()), this, SLOT(_sendImage()));

    if (_inactiveTime_m) {
        _timer_p = new QTimer(this);
        connect(_timer_p, SIGNAL(timeout()), this, SLOT(_inActivity()));
        _timer_p->start(_inactiveTime_m);
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

// private

QString hgx11::_parseColorArr(QString value, bool isInt)
{
    QStringList values = value.split(',');
    if (values.size() != 3) {
        return "";
    }
    QString retVal = "[";
    for (int i = 0; i < 3; i++) {
        if (isInt && (values.at(i).toInt() < 0 || values.at(i).toInt() > 255)) {
            return "";
        }
        if (!isInt && (values.at(i).toDouble() < 0.0 || values.at(i).toDouble() > 1.0)) {
            return "";
        }
        retVal.append(values.at(i));
        retVal.append(",");
    }
    retVal.chop(1);
    retVal.append("]");
    return retVal;
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
    _timer_p->start(_inactiveTime_m);
}

void hgx11::_setImgSize()
{
    _hclient_p->imgWidth = QString::number(_grabber_p->getDest_width());
    _hclient_p->imgHeight = QString::number(_grabber_p->getDest_height());
}
