/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PainterWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      24,   14,   14,   14, 0x0a,
      43,   14,   14,   14, 0x0a,
      56,   14,   14,   14, 0x0a,
      72,   14,   68,   14, 0x0a,
     115,   95,   85,   14, 0x0a,
     194,  177,   14,   14, 0x0a,
     235,   14,   14,   14, 0x0a,
     252,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PainterWidget[] = {
    "PainterWidget\0\0detect()\0frame_per_second()\0"
    "game_start()\0game_exit()\0int\0MusicSheet()\0"
    "IplImage*\0img,cascade,storage\0"
    "face_detect(IplImage*,CvHaarClassifierCascade*,CvMemStorage*)\0"
    "pIplImage,qImage\0"
    "cvxCopyIplImage(const IplImage*,QImage&)\0"
    "musicnote_load()\0counting_down_to_start()\0"
};

void PainterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PainterWidget *_t = static_cast<PainterWidget *>(_o);
        switch (_id) {
        case 0: _t->detect(); break;
        case 1: _t->frame_per_second(); break;
        case 2: _t->game_start(); break;
        case 3: _t->game_exit(); break;
        case 4: { int _r = _t->MusicSheet();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { IplImage* _r = _t->face_detect((*reinterpret_cast< IplImage*(*)>(_a[1])),(*reinterpret_cast< CvHaarClassifierCascade*(*)>(_a[2])),(*reinterpret_cast< CvMemStorage*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< IplImage**>(_a[0]) = _r; }  break;
        case 6: _t->cvxCopyIplImage((*reinterpret_cast< const IplImage*(*)>(_a[1])),(*reinterpret_cast< QImage(*)>(_a[2]))); break;
        case 7: _t->musicnote_load(); break;
        case 8: _t->counting_down_to_start(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PainterWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PainterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PainterWidget,
      qt_meta_data_PainterWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PainterWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PainterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PainterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PainterWidget))
        return static_cast<void*>(const_cast< PainterWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PainterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
