#ifndef HGX11NET_H
#define HGX11NET_H

#include <QTcpSocket>

class hgx11net : public QObject
{
    Q_OBJECT
public:
    enum {
        REDADJUST = 0x01,
        BLUEADJUST = 0x02,
        GREENADJUST = 0x04,
    };
    QString imgWidth;
    QString imgHeight;

    hgx11net(QString, ushort);
    ~hgx11net();

    void clearLeds();
    void setLedColor(quint8, quint8, quint8);
    void ledAdjustments(QString, QString, QString, QString, QString, QString);

private:
    QTcpSocket *_sock_p;
    QByteArray _cmd_m;
    QString _host_m;
    quint16 _port_m;
    QString _redAdjust_m = "";
    QString _greenAdjust_m = "";
    QString _blueAdjust_m = "";
    QString _temperature_m = "";
    QString _threshold_m = "";
    QString _transform_m = "";
    int _colorAdjustmentType_m = 0;

    void _sendCommand();
    void _connectHost();
    bool _isConnected();
    void _ledAdjustments();
    void _colorAdjustment();
    void _thresholdAdjustment();
    void _transformdAdjustment();
    void _temperatureAdjustment();

public slots:
    void sendImage(QByteArray *);
};

#endif // HGX11NET_H
