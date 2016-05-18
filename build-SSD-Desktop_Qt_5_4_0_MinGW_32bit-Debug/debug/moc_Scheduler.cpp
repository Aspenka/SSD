/****************************************************************************
** Meta object code from reading C++ file 'Scheduler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SSD/Scheduler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Scheduler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Scheduler_t {
    QByteArrayData data[10];
    char stringdata[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Scheduler_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Scheduler_t qt_meta_stringdata_Scheduler = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Scheduler"
QT_MOC_LITERAL(1, 10, 12), // "sig_callTask"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "taskIndex"
QT_MOC_LITERAL(4, 34, 7), // "sigDone"
QT_MOC_LITERAL(5, 42, 5), // "start"
QT_MOC_LITERAL(6, 48, 7), // "restart"
QT_MOC_LITERAL(7, 56, 4), // "stop"
QT_MOC_LITERAL(8, 61, 12), // "slt_Reaction"
QT_MOC_LITERAL(9, 74, 5) // "index"

    },
    "Scheduler\0sig_callTask\0\0taskIndex\0"
    "sigDone\0start\0restart\0stop\0slt_Reaction\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Scheduler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   50,    2, 0x0a /* Public */,
       6,    0,   51,    2, 0x0a /* Public */,
       7,    0,   52,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void Scheduler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Scheduler *_t = static_cast<Scheduler *>(_o);
        switch (_id) {
        case 0: _t->sig_callTask((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sigDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->start(); break;
        case 3: _t->restart(); break;
        case 4: _t->stop(); break;
        case 5: _t->slt_Reaction((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Scheduler::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::sig_callTask)) {
                *result = 0;
            }
        }
        {
            typedef void (Scheduler::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Scheduler::sigDone)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Scheduler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Scheduler.data,
      qt_meta_data_Scheduler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Scheduler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scheduler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Scheduler.stringdata))
        return static_cast<void*>(const_cast< Scheduler*>(this));
    return QObject::qt_metacast(_clname);
}

int Scheduler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Scheduler::sig_callTask(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Scheduler::sigDone(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
