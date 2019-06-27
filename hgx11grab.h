#ifndef HGX11GRAB_H
#define HGX11GRAB_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>

class hgx11grab : public QObject
{
    Q_OBJECT
public:
    QString imgdata_m;

    hgx11grab(Display *, unsigned short);
    ~hgx11grab();

    int getDest_width() const;
    int getDest_height() const;

private:
    Display *_display_p;
    Window _window_m;
    Picture _srcPicture_m;
    Picture _dstPicture_m;
    Pixmap _pixmap_m;
    XImage *_xImage_p;
    XRenderPictFormat *_srcFormat_p;
    XRenderPictFormat *_dstFormat_p;
    XRenderPictureAttributes _pictAttr_m;
    XShmSegmentInfo _shminfo_m;
    XTransform _mTransform_m;
    XWindowAttributes _windowAttr_m;
    int _srcWidth_m = 0;
    int _srcHeight_m = 0;
    int _destWidth_m;
    int _destHeight_m;
    int _imgSize_m;
    double _scale_m;
    bool _freed_m = 0;

    bool _getWinAttr();
    void _freeResources();
    void _setScale();

signals:
    void imageCreated();
    void scaleChanged();

public slots:
    void grabFrame();

private slots:
    void _changeScale();
};

#endif // HGX11GRAB_H
