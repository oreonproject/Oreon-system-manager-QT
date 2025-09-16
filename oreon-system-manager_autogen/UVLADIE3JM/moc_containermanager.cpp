/****************************************************************************
** Meta object code from reading C++ file 'containermanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/containermanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'containermanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_ContainerSearchWorker_t {
    uint offsetsAndSizes[18];
    char stringdata0[22];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[11];
    char stringdata5[10];
    char stringdata6[15];
    char stringdata7[14];
    char stringdata8[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ContainerSearchWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ContainerSearchWorker_t qt_meta_stringdata_ContainerSearchWorker = {
    {
        QT_MOC_LITERAL(0, 21),  // "ContainerSearchWorker"
        QT_MOC_LITERAL(22, 14),  // "containerFound"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 13),  // "containerInfo"
        QT_MOC_LITERAL(52, 10),  // "imageFound"
        QT_MOC_LITERAL(63, 9),  // "imageInfo"
        QT_MOC_LITERAL(73, 14),  // "searchFinished"
        QT_MOC_LITERAL(88, 13),  // "errorOccurred"
        QT_MOC_LITERAL(102, 5)   // "error"
    },
    "ContainerSearchWorker",
    "containerFound",
    "",
    "containerInfo",
    "imageFound",
    "imageInfo",
    "searchFinished",
    "errorOccurred",
    "error"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ContainerSearchWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       4,    1,   41,    2, 0x06,    3 /* Public */,
       6,    0,   44,    2, 0x06,    5 /* Public */,
       7,    1,   45,    2, 0x06,    6 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject ContainerSearchWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_ContainerSearchWorker.offsetsAndSizes,
    qt_meta_data_ContainerSearchWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ContainerSearchWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ContainerSearchWorker, std::true_type>,
        // method 'containerFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'imageFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'searchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ContainerSearchWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContainerSearchWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->containerFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 1: _t->imageFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 2: _t->searchFinished(); break;
        case 3: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ContainerSearchWorker::*)(const QJsonObject & );
            if (_t _q_method = &ContainerSearchWorker::containerFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ContainerSearchWorker::*)(const QJsonObject & );
            if (_t _q_method = &ContainerSearchWorker::imageFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ContainerSearchWorker::*)();
            if (_t _q_method = &ContainerSearchWorker::searchFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ContainerSearchWorker::*)(const QString & );
            if (_t _q_method = &ContainerSearchWorker::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ContainerSearchWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContainerSearchWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContainerSearchWorker.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ContainerSearchWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ContainerSearchWorker::containerFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ContainerSearchWorker::imageFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ContainerSearchWorker::searchFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ContainerSearchWorker::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
namespace {
struct qt_meta_stringdata_ContainerManager_t {
    uint offsetsAndSizes[100];
    char stringdata0[17];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[27];
    char stringdata5[23];
    char stringdata6[27];
    char stringdata7[17];
    char stringdata8[14];
    char stringdata9[13];
    char stringdata10[10];
    char stringdata11[17];
    char stringdata12[14];
    char stringdata13[6];
    char stringdata14[28];
    char stringdata15[4];
    char stringdata16[24];
    char stringdata17[28];
    char stringdata18[28];
    char stringdata19[24];
    char stringdata20[28];
    char stringdata21[18];
    char stringdata22[7];
    char stringdata23[9];
    char stringdata24[8];
    char stringdata25[16];
    char stringdata26[8];
    char stringdata27[15];
    char stringdata28[9];
    char stringdata29[7];
    char stringdata30[15];
    char stringdata31[14];
    char stringdata32[12];
    char stringdata33[12];
    char stringdata34[15];
    char stringdata35[26];
    char stringdata36[26];
    char stringdata37[18];
    char stringdata38[21];
    char stringdata39[17];
    char stringdata40[16];
    char stringdata41[16];
    char stringdata42[16];
    char stringdata43[12];
    char stringdata44[10];
    char stringdata45[11];
    char stringdata46[9];
    char stringdata47[10];
    char stringdata48[10];
    char stringdata49[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ContainerManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ContainerManager_t qt_meta_stringdata_ContainerManager = {
    {
        QT_MOC_LITERAL(0, 16),  // "ContainerManager"
        QT_MOC_LITERAL(17, 16),  // "searchContainers"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 12),  // "searchImages"
        QT_MOC_LITERAL(48, 26),  // "onContainerActionTriggered"
        QT_MOC_LITERAL(75, 22),  // "onImageActionTriggered"
        QT_MOC_LITERAL(98, 26),  // "onDistroboxActionTriggered"
        QT_MOC_LITERAL(125, 16),  // "onContainerFound"
        QT_MOC_LITERAL(142, 13),  // "containerInfo"
        QT_MOC_LITERAL(156, 12),  // "onImageFound"
        QT_MOC_LITERAL(169, 9),  // "imageInfo"
        QT_MOC_LITERAL(179, 16),  // "onSearchFinished"
        QT_MOC_LITERAL(196, 13),  // "onSearchError"
        QT_MOC_LITERAL(210, 5),  // "error"
        QT_MOC_LITERAL(216, 27),  // "onContainerTableContextMenu"
        QT_MOC_LITERAL(244, 3),  // "pos"
        QT_MOC_LITERAL(248, 23),  // "onImageTableContextMenu"
        QT_MOC_LITERAL(272, 27),  // "onDistroboxTableContextMenu"
        QT_MOC_LITERAL(300, 27),  // "onContainerSelectionChanged"
        QT_MOC_LITERAL(328, 23),  // "onImageSelectionChanged"
        QT_MOC_LITERAL(352, 27),  // "onDistroboxSelectionChanged"
        QT_MOC_LITERAL(380, 17),  // "onProgressUpdated"
        QT_MOC_LITERAL(398, 6),  // "taskId"
        QT_MOC_LITERAL(405, 8),  // "progress"
        QT_MOC_LITERAL(414, 7),  // "message"
        QT_MOC_LITERAL(422, 15),  // "onTaskCompleted"
        QT_MOC_LITERAL(438, 7),  // "success"
        QT_MOC_LITERAL(446, 14),  // "onTaskFinished"
        QT_MOC_LITERAL(461, 8),  // "exitCode"
        QT_MOC_LITERAL(470, 6),  // "output"
        QT_MOC_LITERAL(477, 14),  // "onTaskProgress"
        QT_MOC_LITERAL(492, 13),  // "onTaskStarted"
        QT_MOC_LITERAL(506, 11),  // "description"
        QT_MOC_LITERAL(518, 11),  // "onTaskError"
        QT_MOC_LITERAL(530, 14),  // "onRefreshTimer"
        QT_MOC_LITERAL(545, 25),  // "showCreateContainerDialog"
        QT_MOC_LITERAL(571, 25),  // "showCreateDistroboxDialog"
        QT_MOC_LITERAL(597, 17),  // "showContainerLogs"
        QT_MOC_LITERAL(615, 20),  // "showContainerInspect"
        QT_MOC_LITERAL(636, 16),  // "showImageInspect"
        QT_MOC_LITERAL(653, 15),  // "exportContainer"
        QT_MOC_LITERAL(669, 15),  // "importContainer"
        QT_MOC_LITERAL(685, 15),  // "pruneContainers"
        QT_MOC_LITERAL(701, 11),  // "pruneImages"
        QT_MOC_LITERAL(713, 9),  // "pullImage"
        QT_MOC_LITERAL(723, 10),  // "buildImage"
        QT_MOC_LITERAL(734, 8),  // "tagImage"
        QT_MOC_LITERAL(743, 9),  // "pushImage"
        QT_MOC_LITERAL(753, 9),  // "saveImage"
        QT_MOC_LITERAL(763, 9)   // "loadImage"
    },
    "ContainerManager",
    "searchContainers",
    "",
    "searchImages",
    "onContainerActionTriggered",
    "onImageActionTriggered",
    "onDistroboxActionTriggered",
    "onContainerFound",
    "containerInfo",
    "onImageFound",
    "imageInfo",
    "onSearchFinished",
    "onSearchError",
    "error",
    "onContainerTableContextMenu",
    "pos",
    "onImageTableContextMenu",
    "onDistroboxTableContextMenu",
    "onContainerSelectionChanged",
    "onImageSelectionChanged",
    "onDistroboxSelectionChanged",
    "onProgressUpdated",
    "taskId",
    "progress",
    "message",
    "onTaskCompleted",
    "success",
    "onTaskFinished",
    "exitCode",
    "output",
    "onTaskProgress",
    "onTaskStarted",
    "description",
    "onTaskError",
    "onRefreshTimer",
    "showCreateContainerDialog",
    "showCreateDistroboxDialog",
    "showContainerLogs",
    "showContainerInspect",
    "showImageInspect",
    "exportContainer",
    "importContainer",
    "pruneContainers",
    "pruneImages",
    "pullImage",
    "buildImage",
    "tagImage",
    "pushImage",
    "saveImage",
    "loadImage"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ContainerManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  236,    2, 0x0a,    1 /* Public */,
       3,    0,  237,    2, 0x0a,    2 /* Public */,
       4,    0,  238,    2, 0x0a,    3 /* Public */,
       5,    0,  239,    2, 0x0a,    4 /* Public */,
       6,    0,  240,    2, 0x0a,    5 /* Public */,
       7,    1,  241,    2, 0x08,    6 /* Private */,
       9,    1,  244,    2, 0x08,    8 /* Private */,
      11,    0,  247,    2, 0x08,   10 /* Private */,
      12,    1,  248,    2, 0x08,   11 /* Private */,
      14,    1,  251,    2, 0x08,   13 /* Private */,
      16,    1,  254,    2, 0x08,   15 /* Private */,
      17,    1,  257,    2, 0x08,   17 /* Private */,
      18,    0,  260,    2, 0x08,   19 /* Private */,
      19,    0,  261,    2, 0x08,   20 /* Private */,
      20,    0,  262,    2, 0x08,   21 /* Private */,
      21,    3,  263,    2, 0x08,   22 /* Private */,
      25,    3,  270,    2, 0x08,   26 /* Private */,
      27,    3,  277,    2, 0x08,   30 /* Private */,
      30,    2,  284,    2, 0x08,   34 /* Private */,
      31,    2,  289,    2, 0x08,   37 /* Private */,
      33,    2,  294,    2, 0x08,   40 /* Private */,
      34,    0,  299,    2, 0x08,   43 /* Private */,
      35,    0,  300,    2, 0x08,   44 /* Private */,
      36,    0,  301,    2, 0x08,   45 /* Private */,
      37,    0,  302,    2, 0x08,   46 /* Private */,
      38,    0,  303,    2, 0x08,   47 /* Private */,
      39,    0,  304,    2, 0x08,   48 /* Private */,
      40,    0,  305,    2, 0x08,   49 /* Private */,
      41,    0,  306,    2, 0x08,   50 /* Private */,
      42,    0,  307,    2, 0x08,   51 /* Private */,
      43,    0,  308,    2, 0x08,   52 /* Private */,
      44,    0,  309,    2, 0x08,   53 /* Private */,
      45,    0,  310,    2, 0x08,   54 /* Private */,
      46,    0,  311,    2, 0x08,   55 /* Private */,
      47,    0,  312,    2, 0x08,   56 /* Private */,
      48,    0,  313,    2, 0x08,   57 /* Private */,
      49,    0,  314,    2, 0x08,   58 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QPoint,   15,
    QMetaType::Void, QMetaType::QPoint,   15,
    QMetaType::Void, QMetaType::QPoint,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   22,   23,   24,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   22,   26,   24,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   22,   28,   29,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   22,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   22,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   22,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ContainerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ContainerManager.offsetsAndSizes,
    qt_meta_data_ContainerManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ContainerManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ContainerManager, std::true_type>,
        // method 'searchContainers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchImages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onContainerActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onImageActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDistroboxActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onContainerFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onImageFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onSearchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onContainerTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onImageTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onDistroboxTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onContainerSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onImageSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDistroboxSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProgressUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRefreshTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showCreateContainerDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showCreateDistroboxDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showContainerLogs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showContainerInspect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showImageInspect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportContainer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'importContainer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pruneContainers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pruneImages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pullImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buildImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'tagImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pushImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ContainerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContainerManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->searchContainers(); break;
        case 1: _t->searchImages(); break;
        case 2: _t->onContainerActionTriggered(); break;
        case 3: _t->onImageActionTriggered(); break;
        case 4: _t->onDistroboxActionTriggered(); break;
        case 5: _t->onContainerFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->onImageFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 7: _t->onSearchFinished(); break;
        case 8: _t->onSearchError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->onContainerTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 10: _t->onImageTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 11: _t->onDistroboxTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 12: _t->onContainerSelectionChanged(); break;
        case 13: _t->onImageSelectionChanged(); break;
        case 14: _t->onDistroboxSelectionChanged(); break;
        case 15: _t->onProgressUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 16: _t->onTaskCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 17: _t->onTaskFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 18: _t->onTaskProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 19: _t->onTaskStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 20: _t->onTaskError((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->onRefreshTimer(); break;
        case 22: _t->showCreateContainerDialog(); break;
        case 23: _t->showCreateDistroboxDialog(); break;
        case 24: _t->showContainerLogs(); break;
        case 25: _t->showContainerInspect(); break;
        case 26: _t->showImageInspect(); break;
        case 27: _t->exportContainer(); break;
        case 28: _t->importContainer(); break;
        case 29: _t->pruneContainers(); break;
        case 30: _t->pruneImages(); break;
        case 31: _t->pullImage(); break;
        case 32: _t->buildImage(); break;
        case 33: _t->tagImage(); break;
        case 34: _t->pushImage(); break;
        case 35: _t->saveImage(); break;
        case 36: _t->loadImage(); break;
        default: ;
        }
    }
}

const QMetaObject *ContainerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContainerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContainerManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ContainerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 37;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
