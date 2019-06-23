#include "hgx11net.h"

// public

hgx11net::hgx11net(QString host, ushort port)
{
    _sock_p = new QTcpSocket(this);
    _host_m = host;
    _port_m = port;
    this->_connectHost();
}

hgx11net::~hgx11net()
{
    clearLeds();
    _sock_p->disconnectFromHost();
}

void hgx11net::clearLeds()
{
    if (!_isConnected()) {
        return;
    }
   _sock_p->write("{\"command\": \"clearall\"}\n");
}

void hgx11net::setLedColor(quint8 R, quint8 G, quint8 B)
{
    if (!_isConnected()) {
        return;
    }
    _cmd_m.clear();
    _cmd_m.append("{\"color\":[");
    _cmd_m.append(QString::number(R));
    _cmd_m.append(",");
    _cmd_m.append(QString::number(G));
    _cmd_m.append(",");
    _cmd_m.append(QString::number(B));
    _cmd_m.append("],\"command\":\"color\",\"priority\":100}}\n");
    _sock_p->write(_cmd_m);
}

// private

bool hgx11net::_isConnected()
{
    bool connected = (_sock_p->state() == QAbstractSocket::ConnectedState);
    if (!connected) {
        _sock_p->reset();
        _connectHost();
        connected = (_sock_p->state() == QAbstractSocket::ConnectedState);
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

// public slots

void hgx11net::sendImage(QByteArray *imgdata)
{
    if (!_isConnected()) {
        return;
    }
    _cmd_m.clear();
    _cmd_m.append("{\"command\":\"image\",\"imagedata\":\"");
    _cmd_m.append(imgdata->toBase64());
    _cmd_m.append("\",\"imageheight\":");
    _cmd_m.append(imgHeight);
    _cmd_m.append(",\"imagewidth\":");
    _cmd_m.append(imgWidth);
    _cmd_m.append(",\"priority\":100}\n");
    _sock_p->write(_cmd_m);
}
