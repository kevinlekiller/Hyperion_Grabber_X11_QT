#ifndef HGX11NET_H
#define HGX11NET_H

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QString>
#include <QObject>
#include <QByteArray>

class hgx11net : public QObject
{
    Q_OBJECT
public:
    QString imgWidth;
    QString imgHeight;

    hgx11net(QString, ushort);
    ~hgx11net();

    void clearLeds();
    void setLedColor(quint8, quint8, quint8);

private:
    QTcpSocket *_sock_p;
    QByteArray _cmd_m;
    QString _host_m;
    quint16 _port_m;

    void _connectHost();
    bool _isConnected();

public slots:
    void sendImage(QByteArray *);
};

#endif // HGX11NET_H
