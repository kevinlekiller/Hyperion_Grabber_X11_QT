#include "hgx11net.h"

// public

hgx11net::hgx11net(QString host, ushort port, QString priority)
{
    _sock_p = new QTcpSocket(this);
    _sock_p->setReadBufferSize(1);
    _host_m = host;
    _port_m = port;
    _priority_m = priority;
    _connectHost();
}

hgx11net::~hgx11net()
{
    clearLeds();
    if (_sock_p->state() == QAbstractSocket::ConnectedState) {
        while(_sock_p->waitForBytesWritten()) {}
        _sock_p->disconnectFromHost();
    }
    delete _sock_p;
}

void hgx11net::clearLeds()
{
    _cmd_m.clear();
    _cmd_m.append("{\"command\": \"clearall\",").append(_priority_m.toUtf8()).append("}\n");
   _sendCommand();
}

void hgx11net::setLedColor(quint8 R, quint8 G, quint8 B)
{
    _cmd_m.clear();
    _cmd_m.append("{\"color\":[");
    _cmd_m.append(QString::number(R).toUtf8());
    _cmd_m.append(",");
    _cmd_m.append(QString::number(G).toUtf8());
    _cmd_m.append(",");
    _cmd_m.append(QString::number(B).toUtf8());
    _cmd_m.append("],\"command\":\"color\",").append(_priority_m.toUtf8()).append("}\n");
    _sendCommand();
}

void hgx11net::ledAdjustments(QString red, QString green, QString blue, QString temp, QString thres, QString trans)
{
    _redAdjust_m = red;
    _greenAdjust_m = green;
    _blueAdjust_m = blue;
    _temperature_m = temp;
    _threshold_m = thres;
    _transform_m = trans;
    _ledAdjustments();
}

// private

bool hgx11net::_isConnected()
{
    bool connected = (_sock_p->state() == QAbstractSocket::ConnectedState);
    if (!connected) {
        _sock_p->reset();
        _connectHost();
        connected = (_sock_p->state() == QAbstractSocket::ConnectedState);
        _ledAdjustments();
    }
    return connected;
}

void hgx11net::_connectHost()
{
    _sock_p->connectToHost(_host_m, _port_m);
    if(!_sock_p->waitForConnected(5000)) {
        qWarning() << "hxgfnet Error: " << _sock_p->errorString();
        return;
    }
}

void hgx11net::_sendCommand()
{
    if (!_isConnected()) {
        return;
    }
    _sock_p->write(_cmd_m);
    while (_sock_p->bytesToWrite()) {
        _sock_p->waitForBytesWritten();
    }
}

void hgx11net::_ledAdjustments()
{
    _colorAdjustmentType_m = 0;
    if (_redAdjust_m != "") {
        _colorAdjustmentType_m |= REDADJUST;
    }
    if (_greenAdjust_m != "") {
        _colorAdjustmentType_m |= GREENADJUST;
    }
    if (_blueAdjust_m != "") {
        _colorAdjustmentType_m |= BLUEADJUST;
    }
    _colorAdjustment();
    _thresholdAdjustment();
    _transformdAdjustment();
}

void hgx11net::_colorAdjustment()
{
    if (!_colorAdjustmentType_m) {
        return;
    }
    _cmd_m.clear();
    _cmd_m.append("{\"adjustment\":{");
    if (_colorAdjustmentType_m & REDADJUST) {
        _cmd_m.append("\"redAdjust\":");
        _cmd_m.append(_redAdjust_m.toUtf8());
        _cmd_m.append(",");
    }
    if (_colorAdjustmentType_m & GREENADJUST) {
        _cmd_m.append("\"greenAdjust\":");
        _cmd_m.append(_greenAdjust_m.toUtf8());
        _cmd_m.append(",");
    }
    if (_colorAdjustmentType_m & BLUEADJUST) {
        _cmd_m.append("\"blueAdjust\":");
        _cmd_m.append(_blueAdjust_m.toUtf8());
        _cmd_m.append(",");
    }
    _cmd_m.chop(1); // remove trailing ,
    _cmd_m.append("},\"command\":\"adjustment\",").append(_priority_m.toUtf8()).append("}\n");
    _sendCommand();
}

void hgx11net::_thresholdAdjustment()
{
    if (_threshold_m == "") {
        return;
    }
    _cmd_m.clear();
    _cmd_m.append("{\"command\":\"transform\",").append(_priority_m.toUtf8()).append(",\"transform\":{\"threshold\":");
    _cmd_m.append(_threshold_m.toUtf8());
    _cmd_m.append("}}\n");
    _sendCommand();
}

void hgx11net::_transformdAdjustment()
{
    if (_transform_m == "") {
        return;
    }
    QStringList values = _transform_m.split(',');
    _cmd_m.clear();
    _cmd_m.append("{\"command\":\"transform\",").append(_priority_m.toUtf8()).append(",\"transform\":{\"luminanceGain\":");
    _cmd_m.append(values.at(0).toUtf8());
    _cmd_m.append(",\"luminanceMinimum\":");
    _cmd_m.append(values.at(1).toUtf8());
    _cmd_m.append(",\"saturationGain\":");
    _cmd_m.append(values.at(2).toUtf8());
    _cmd_m.append("}}\n");
    _sendCommand();
}

void hgx11net::_temperatureAdjustment()
{
    if (_temperature_m == "") {
        return;
    }
    _cmd_m.clear();
    _cmd_m.append("{\"command\":\"temperature\",").append(_priority_m.toUtf8()).append(",\"temperature\":{\"correctionValues\":");
    _cmd_m.append(_temperature_m.toUtf8());
    _cmd_m.append("}}\n");
    _sendCommand();
}

// public slots

void hgx11net::sendImage(QString imgdata)
{
    _cmd_m = imgCmdBuf;
    _cmd_m.append(imgdata.toUtf8());
    _cmd_m.append("\"}\n");
    _sendCommand();
}
