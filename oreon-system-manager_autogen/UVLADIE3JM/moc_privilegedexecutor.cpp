/****************************************************************************
** Meta object code from reading C++ file 'privilegedexecutor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/privilegedexecutor.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'privilegedexecutor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_PrivilegedExecutor_t {
    uint offsetsAndSizes[42];
    char stringdata0[19];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[12];
    char stringdata5[13];
    char stringdata6[9];
    char stringdata7[7];
    char stringdata8[10];
    char stringdata9[6];
    char stringdata10[13];
    char stringdata11[9];
    char stringdata12[14];
    char stringdata13[17];
    char stringdata14[18];
    char stringdata15[21];
    char stringdata16[11];
    char stringdata17[15];
    char stringdata18[23];
    char stringdata19[19];
    char stringdata20[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_PrivilegedExecutor_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_PrivilegedExecutor_t qt_meta_stringdata_PrivilegedExecutor = {
    {
        QT_MOC_LITERAL(0, 18),  // "PrivilegedExecutor"
        QT_MOC_LITERAL(19, 11),  // "taskStarted"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 6),  // "taskId"
        QT_MOC_LITERAL(39, 11),  // "description"
        QT_MOC_LITERAL(51, 12),  // "taskFinished"
        QT_MOC_LITERAL(64, 8),  // "exitCode"
        QT_MOC_LITERAL(73, 6),  // "output"
        QT_MOC_LITERAL(80, 9),  // "taskError"
        QT_MOC_LITERAL(90, 5),  // "error"
        QT_MOC_LITERAL(96, 12),  // "taskProgress"
        QT_MOC_LITERAL(109, 8),  // "progress"
        QT_MOC_LITERAL(118, 13),  // "taskCancelled"
        QT_MOC_LITERAL(132, 16),  // "onProcessStarted"
        QT_MOC_LITERAL(149, 17),  // "onProcessFinished"
        QT_MOC_LITERAL(167, 20),  // "QProcess::ExitStatus"
        QT_MOC_LITERAL(188, 10),  // "exitStatus"
        QT_MOC_LITERAL(199, 14),  // "onProcessError"
        QT_MOC_LITERAL(214, 22),  // "QProcess::ProcessError"
        QT_MOC_LITERAL(237, 18),  // "onProcessReadyRead"
        QT_MOC_LITERAL(256, 15)   // "processNextTask"
    },
    "PrivilegedExecutor",
    "taskStarted",
    "",
    "taskId",
    "description",
    "taskFinished",
    "exitCode",
    "output",
    "taskError",
    "error",
    "taskProgress",
    "progress",
    "taskCancelled",
    "onProcessStarted",
    "onProcessFinished",
    "QProcess::ExitStatus",
    "exitStatus",
    "onProcessError",
    "QProcess::ProcessError",
    "onProcessReadyRead",
    "processNextTask"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_PrivilegedExecutor[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   74,    2, 0x06,    1 /* Public */,
       5,    3,   79,    2, 0x06,    4 /* Public */,
       8,    2,   86,    2, 0x06,    8 /* Public */,
      10,    2,   91,    2, 0x06,   11 /* Public */,
      12,    1,   96,    2, 0x06,   14 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    0,   99,    2, 0x08,   16 /* Private */,
      14,    2,  100,    2, 0x08,   17 /* Private */,
      17,    1,  105,    2, 0x08,   20 /* Private */,
      19,    0,  108,    2, 0x08,   22 /* Private */,
      20,    0,  109,    2, 0x08,   23 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,    3,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,   11,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,    6,   16,
    QMetaType::Void, 0x80000000 | 18,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PrivilegedExecutor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PrivilegedExecutor.offsetsAndSizes,
    qt_meta_data_PrivilegedExecutor,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_PrivilegedExecutor_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PrivilegedExecutor, std::true_type>,
        // method 'taskStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'taskFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'taskError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'taskProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'taskCancelled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onProcessStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProcessFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ExitStatus, std::false_type>,
        // method 'onProcessError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QProcess::ProcessError, std::false_type>,
        // method 'onProcessReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processNextTask'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PrivilegedExecutor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PrivilegedExecutor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->taskStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->taskFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 2: _t->taskError((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->taskProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->taskCancelled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onProcessStarted(); break;
        case 6: _t->onProcessFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 7: _t->onProcessError((*reinterpret_cast< std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        case 8: _t->onProcessReadyRead(); break;
        case 9: _t->processNextTask(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PrivilegedExecutor::*)(int , const QString & );
            if (_t _q_method = &PrivilegedExecutor::taskStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PrivilegedExecutor::*)(int , int , const QString & );
            if (_t _q_method = &PrivilegedExecutor::taskFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PrivilegedExecutor::*)(int , const QString & );
            if (_t _q_method = &PrivilegedExecutor::taskError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PrivilegedExecutor::*)(int , const QString & );
            if (_t _q_method = &PrivilegedExecutor::taskProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PrivilegedExecutor::*)(int );
            if (_t _q_method = &PrivilegedExecutor::taskCancelled; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *PrivilegedExecutor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PrivilegedExecutor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PrivilegedExecutor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PrivilegedExecutor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PrivilegedExecutor::taskStarted(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PrivilegedExecutor::taskFinished(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PrivilegedExecutor::taskError(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PrivilegedExecutor::taskProgress(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void PrivilegedExecutor::taskCancelled(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
