#include "hgx11.h"

// public

hgx11::hgx11(QHash<QString, QString> opts)
{
    QString addr = "localhost";
    unsigned short port = 19444;
    unsigned short scale = 8;
    unsigned short frameskip = 0;
    QString redAdjust = "", greenAdjust = "", blueAdjust = "";
    QString temperature = "", threshold = "", transform = "";
    QHashIterator<QString, QString> i(opts);
    while (i.hasNext()) {
        i.next();
        if ((i.key() == "a" || i.key() == "address") && !(i.value().isNull() && i.value().isEmpty())) {
            addr = i.value();
        } else if ((i.key() == "p" || i.key() == "port") && i.value().toUShort()) {
            port = i.value().toUShort();
        } else if ((i.key() == "s" || i.key() == "scale") && i.value().toUShort()) {
            scale = i.value().toUShort();
        } else if ((i.key() == "f" || i.key() == "frameskip") && (i.value().toUShort() && i.value().toUShort() <= 255)) {
            frameskip = i.value().toUShort();
        } else if ((i.key() == "i" || i.key() == "inactive") && i.value().toInt()) {
            _inactiveTime_m = (i.value().toInt() * 1000);
        } else if (i.key() == "r" || i.key() == "redadjust") {
            redAdjust = _parseColorArr(i.value(), 1);
        } else if (i.key() == "g" || i.key() == "greenadjust") {
            greenAdjust = _parseColorArr(i.value(), 1);
        } else if (i.key() == "b" || i.key() == "blueadjust") {
            blueAdjust = _parseColorArr(i.value(), 1);
        } else if (i.key() == "t" || i.key() == "temperature") {
            temperature = _parseColorArr(i.value(), 1);
        } else if (i.key() == "d" || i.key() == "threshold") {
            threshold = _parseColorArr(i.value(), 0);
        }else if ((i.key() == "l" || i.key() == "transform") && _parseColorArr(i.value(), 0) != "") {
            transform = i.value();
        }
    }

    _grabber_p = new hgx11grab(scale);

    _hclient_p = new hgx11net(addr, port);

    _damage_p = new hgx11damage(frameskip);

    _setImgSize();
    _hclient_p->ledAdjustments(redAdjust, greenAdjust, blueAdjust, temperature, threshold, transform);

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
    value = "[";
    for (int i = 0; i < 3; i++) {
        if (isInt && (values.at(i).toInt() < 0 || values.at(i).toInt() > 255)) {
            return "";
        }
        if (!isInt && (values.at(i).toDouble() < 0.0 || values.at(i).toDouble() > 1.0)) {
            return "";
        }
        value.append(values.at(i));
        value.append(",");
    }
    value.chop(1);
    value.append("]");
    return value;
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
    _hclient_p->imgCmdBuf.clear();
    _hclient_p->imgCmdBuf.append("{\"command\":\"image\",\"priority\":100,\"imageheight\":");
    _hclient_p->imgCmdBuf.append(QString::number(_grabber_p->getDest_height()));
    _hclient_p->imgCmdBuf.append(",\"imagewidth\":");
    _hclient_p->imgCmdBuf.append(QString::number(_grabber_p->getDest_width()));
    _hclient_p->imgCmdBuf.append(",\"imagedata\":\"");
}
