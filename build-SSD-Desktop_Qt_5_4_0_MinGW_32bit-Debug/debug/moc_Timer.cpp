/****************************************************************************
** Meta object code from reading C++ file 'Timer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SSD/Timer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Timer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Timer_t {
    QByteArrayData data[10];
    char stringdata[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Timer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Timer_t qt_meta_stringdata_Timer = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Timer"
QT_MOC_LITERAL(1, 6, 7), // "timeout"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 5), // "index"
QT_MOC_LITERAL(4, 21, 4), // "done"
QT_MOC_LITERAL(5, 26, 5), // "start"
QT_MOC_LITERAL(6, 32, 4), // "cron"
QT_MOC_LITERAL(7, 37, 4), // "stop"
QT_MOC_LITERAL(8, 42, 13), // "setSingleShot"
QT_MOC_LITERAL(9, 56, 10) // "singleShot"

    },
    "Timer\0timeout\0\0index\0done\0start\0cron\0"
    "stop\0setSingleShot\0singleShot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Timer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   45,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       8,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void Timer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Timer *_t = static_cast<Timer *>(_o);
        switch (_id) {
        case 0: _t->timeout((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->done((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->start((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->stop(); break;
        case 4: _t->setSingleShot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Timer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Timer::timeout)) {
                *result = 0;
            }
        }
        {
            typedef void (Timer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Timer::done)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Timer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Timer.data,
      qt_meta_data_Timer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Timer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Timer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Timer.stringdata))
        return static_cast<void*>(const_cast< Timer*>(this));
    return QObject::qt_metacast(_clname);
}

int Timer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Timer::timeout(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Timer::done(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
