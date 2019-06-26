#include "hgx11grab.h"

// public

hgx11grab::hgx11grab(Display *display, unsigned short scaleDivisor)
{
    _display_p = display;

    if (!XShmQueryExtension(_display_p)) {
        qCritical() << "Xshm is: not available.";
        return;
    }

    int dummy, pixmaps_supported;

    if (!XRenderQueryExtension(_display_p, &dummy, &dummy)) {
        qCritical() << "XRender is not available.";
        return;
    }

    if (!XShmQueryVersion(_display_p, &dummy, &dummy, &pixmaps_supported)) {
        qCritical("Could not get x shared memory version.");
        return;
    }
    
    bool _XShmPixmapAvailable = pixmaps_supported && XShmPixmapFormat(_display_p) == ZPixmap;
    if (!_XShmPixmapAvailable) {
        qCritical() << "XshmPixMap is: not available.";
        return;
    }

    _window_m = DefaultRootWindow(_display_p);

    if (!_getWinAttr()) {
        return;
    }

    _destWidth_m = int(_srcWidth_m / scaleDivisor);
    _destHeight_m = int(_srcHeight_m / scaleDivisor);

    _imgSize_m = _destHeight_m * _destWidth_m * 4;

    memset(&_pictAttr_m, 0, sizeof(_pictAttr_m));
    _pictAttr_m.repeat = RepeatNone;

    _mTransform_m.matrix[0][0] = XDoubleToFixed(1.0);
    _mTransform_m.matrix[0][1] = 0;
    _mTransform_m.matrix[0][2] = 0;
    _mTransform_m.matrix[1][0] = 0;
    _mTransform_m.matrix[1][1] = XDoubleToFixed(1.0);
    _mTransform_m.matrix[1][2] = 0;
    _mTransform_m.matrix[2][0] = 0;
    _mTransform_m.matrix[2][1] = 0;
    _setScale();

    connect(this, SIGNAL(scaleChanged()), this, SLOT(_changeScale()));
}

hgx11grab::~hgx11grab()
{
    _freeResources();
}

int hgx11grab::getDest_height() const
{
    return _destHeight_m;
}

int hgx11grab::getDest_width() const
{
    return _destWidth_m;
}

// private

bool hgx11grab::_getWinAttr()
{
    if (XGetWindowAttributes(_display_p, _window_m, &_windowAttr_m) == 0) {
        qWarning() << "Failed to obtain X11 window attributes.";
        return false;
    }
    if (_srcWidth_m != _windowAttr_m.width || _srcHeight_m != _windowAttr_m.height) {
        if (0 != _srcWidth_m) {
            _freeResources();
        }
        _srcWidth_m = _windowAttr_m.width;
        _srcHeight_m = _windowAttr_m.height;
        emit scaleChanged();
    }
    return true;
}

void hgx11grab::_freeResources()
{
    if (_freed_m) {
        return;
    }
    XDestroyImage(_xImage_p);
    XShmDetach(_display_p, &_shminfo_m);
    shmdt(_shminfo_m.shmaddr);
    shmctl(_shminfo_m.shmid, IPC_RMID, nullptr);
    XRenderFreePicture(_display_p, _srcPicture_m);
    XRenderFreePicture(_display_p, _dstPicture_m);
    XFreePixmap(_display_p, _pixmap_m);
    _freed_m = 1;
}

void hgx11grab::_setScale()
{
    _scale_m = (1.0 / (double(_srcWidth_m) / double(_destWidth_m)));
    _mTransform_m.matrix[2][2] = XDoubleToFixed(_scale_m);
}

// public slots

void hgx11grab::grabFrame()
{
    if (!_getWinAttr()) {
        return;
    }

    _freed_m = 0;
    _xImage_p = XShmCreateImage(
        _display_p, _windowAttr_m.visual,
        uint(_windowAttr_m.depth), ZPixmap, nullptr, &_shminfo_m,
        uint(_destWidth_m), uint(_destHeight_m)
    );

    _shminfo_m.shmid = shmget(IPC_PRIVATE, ulong(_xImage_p->bytes_per_line * _xImage_p->height), IPC_CREAT|0777);
    _xImage_p->data = reinterpret_cast<char *>(shmat(_shminfo_m.shmid, nullptr, 0));
    _shminfo_m.shmaddr = _xImage_p->data;
    _shminfo_m.readOnly = false;

    XShmAttach(_display_p, &_shminfo_m);
    _pixmap_m = XShmCreatePixmap(_display_p, _window_m, _xImage_p->data, &_shminfo_m, uint(_destWidth_m), uint(_destHeight_m), uint(_windowAttr_m.depth));

    _dstFormat_p = XRenderFindVisualFormat(_display_p, _windowAttr_m.visual);
    _srcFormat_p = XRenderFindVisualFormat(_display_p, _windowAttr_m.visual);

    _srcPicture_m = XRenderCreatePicture(_display_p, _window_m, _srcFormat_p, CPRepeat, &_pictAttr_m);
    _dstPicture_m = XRenderCreatePicture(_display_p, _pixmap_m, _dstFormat_p, CPRepeat, &_pictAttr_m);

    XRenderSetPictureFilter(_display_p, _srcPicture_m, FilterBilinear, nullptr, 0);
    XRenderSetPictureTransform(_display_p, _srcPicture_m, &_mTransform_m);

    XRenderComposite(
        _display_p, // *dpy,
        PictOpSrc,   // op,
        _srcPicture_m, // src
        None,        // mask
        _dstPicture_m, // dst
        0,           // src_x
        0,           // src_y
        0,           // mask_x
        0,           // mask_y
        0,           // dst_x
        0,           // dst_y
        uint(_destWidth_m),          // width
        uint(_destHeight_m)          // height
    );

    XSync(_display_p, false);

    XShmGetImage(_display_p, _pixmap_m, _xImage_p, 0, 0, 0xFFFFFFFF);

    if (_xImage_p == nullptr) {
        qWarning() << "Failed to get image from X11 server.";
        return;
    }

    QImage qimg(reinterpret_cast<const uchar *>(_xImage_p->data), _destWidth_m, _destHeight_m, _xImage_p->bytes_per_line, QImage::Format_ARGB32);
    qimg = qimg.convertToFormat(QImage::Format_RGB888);
    imgdata_m = QByteArray::fromRawData(reinterpret_cast<const char *>(qimg.bits()), qimg.byteCount());

    _freeResources();

    imageCreated();
}

// private slots

void hgx11grab::_changeScale()
{
    _imgSize_m = _destHeight_m * _destWidth_m * 4;
    _setScale();
}
