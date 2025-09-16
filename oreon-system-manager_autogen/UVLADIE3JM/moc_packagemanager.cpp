/****************************************************************************
** Meta object code from reading C++ file 'packagemanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/packagemanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'packagemanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_PackageManager_t {
    uint offsetsAndSizes[54];
    char stringdata0[15];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[24];
    char stringdata5[23];
    char stringdata6[23];
    char stringdata7[18];
    char stringdata8[19];
    char stringdata9[4];
    char stringdata10[7];
    char stringdata11[26];
    char stringdata12[17];
    char stringdata13[19];
    char stringdata14[9];
    char stringdata15[23];
    char stringdata16[7];
    char stringdata17[21];
    char stringdata18[6];
    char stringdata19[24];
    char stringdata20[9];
    char stringdata21[12];
    char stringdata22[18];
    char stringdata23[20];
    char stringdata24[20];
    char stringdata25[16];
    char stringdata26[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_PackageManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_PackageManager_t qt_meta_stringdata_PackageManager = {
    {
        QT_MOC_LITERAL(0, 14),  // "PackageManager"
        QT_MOC_LITERAL(15, 18),  // "refreshPackageList"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 14),  // "searchPackages"
        QT_MOC_LITERAL(50, 23),  // "installSelectedPackages"
        QT_MOC_LITERAL(74, 22),  // "removeSelectedPackages"
        QT_MOC_LITERAL(97, 22),  // "updateSelectedPackages"
        QT_MOC_LITERAL(120, 17),  // "updateAllPackages"
        QT_MOC_LITERAL(138, 18),  // "showPackageDetails"
        QT_MOC_LITERAL(157, 3),  // "row"
        QT_MOC_LITERAL(161, 6),  // "column"
        QT_MOC_LITERAL(168, 25),  // "onPackageSelectionChanged"
        QT_MOC_LITERAL(194, 16),  // "onSearchFinished"
        QT_MOC_LITERAL(211, 18),  // "QList<PackageInfo>"
        QT_MOC_LITERAL(230, 8),  // "packages"
        QT_MOC_LITERAL(239, 22),  // "onPackageActionSuccess"
        QT_MOC_LITERAL(262, 6),  // "output"
        QT_MOC_LITERAL(269, 20),  // "onPackageActionError"
        QT_MOC_LITERAL(290, 5),  // "error"
        QT_MOC_LITERAL(296, 23),  // "onPackageActionProgress"
        QT_MOC_LITERAL(320, 8),  // "progress"
        QT_MOC_LITERAL(329, 11),  // "clearSearch"
        QT_MOC_LITERAL(341, 17),  // "selectAllPackages"
        QT_MOC_LITERAL(359, 19),  // "deselectAllPackages"
        QT_MOC_LITERAL(379, 19),  // "onSearchTextChanged"
        QT_MOC_LITERAL(399, 15),  // "onSearchTimeout"
        QT_MOC_LITERAL(415, 15)   // "onFilterChanged"
    },
    "PackageManager",
    "refreshPackageList",
    "",
    "searchPackages",
    "installSelectedPackages",
    "removeSelectedPackages",
    "updateSelectedPackages",
    "updateAllPackages",
    "showPackageDetails",
    "row",
    "column",
    "onPackageSelectionChanged",
    "onSearchFinished",
    "QList<PackageInfo>",
    "packages",
    "onPackageActionSuccess",
    "output",
    "onPackageActionError",
    "error",
    "onPackageActionProgress",
    "progress",
    "clearSearch",
    "selectAllPackages",
    "deselectAllPackages",
    "onSearchTextChanged",
    "onSearchTimeout",
    "onFilterChanged"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_PackageManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  122,    2, 0x0a,    1 /* Public */,
       3,    0,  123,    2, 0x0a,    2 /* Public */,
       4,    0,  124,    2, 0x0a,    3 /* Public */,
       5,    0,  125,    2, 0x0a,    4 /* Public */,
       6,    0,  126,    2, 0x0a,    5 /* Public */,
       7,    0,  127,    2, 0x0a,    6 /* Public */,
       8,    2,  128,    2, 0x0a,    7 /* Public */,
      11,    0,  133,    2, 0x0a,   10 /* Public */,
      12,    1,  134,    2, 0x0a,   11 /* Public */,
      15,    1,  137,    2, 0x0a,   13 /* Public */,
      17,    1,  140,    2, 0x0a,   15 /* Public */,
      19,    1,  143,    2, 0x0a,   17 /* Public */,
      21,    0,  146,    2, 0x0a,   19 /* Public */,
      22,    0,  147,    2, 0x0a,   20 /* Public */,
      23,    0,  148,    2, 0x0a,   21 /* Public */,
      24,    0,  149,    2, 0x08,   22 /* Private */,
      25,    0,  150,    2, 0x08,   23 /* Private */,
      26,    0,  151,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PackageManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_PackageManager.offsetsAndSizes,
    qt_meta_data_PackageManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_PackageManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PackageManager, std::true_type>,
        // method 'refreshPackageList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installSelectedPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeSelectedPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSelectedPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateAllPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPackageDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onPackageSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<PackageInfo> &, std::false_type>,
        // method 'onPackageActionSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onPackageActionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onPackageActionProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'clearSearch'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectAllPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deselectAllPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFilterChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PackageManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PackageManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshPackageList(); break;
        case 1: _t->searchPackages(); break;
        case 2: _t->installSelectedPackages(); break;
        case 3: _t->removeSelectedPackages(); break;
        case 4: _t->updateSelectedPackages(); break;
        case 5: _t->updateAllPackages(); break;
        case 6: _t->showPackageDetails((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->onPackageSelectionChanged(); break;
        case 8: _t->onSearchFinished((*reinterpret_cast< std::add_pointer_t<QList<PackageInfo>>>(_a[1]))); break;
        case 9: _t->onPackageActionSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->onPackageActionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->onPackageActionProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->clearSearch(); break;
        case 13: _t->selectAllPackages(); break;
        case 14: _t->deselectAllPackages(); break;
        case 15: _t->onSearchTextChanged(); break;
        case 16: _t->onSearchTimeout(); break;
        case 17: _t->onFilterChanged(); break;
        default: ;
        }
    }
}

const QMetaObject *PackageManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PackageManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PackageManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PackageManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    return _id;
}
namespace {
struct qt_meta_stringdata_PackageSearchWorker_t {
    uint offsetsAndSizes[28];
    char stringdata0[20];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[19];
    char stringdata4[9];
    char stringdata5[12];
    char stringdata6[6];
    char stringdata7[15];
    char stringdata8[8];
    char stringdata9[15];
    char stringdata10[11];
    char stringdata11[11];
    char stringdata12[19];
    char stringdata13[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_PackageSearchWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_PackageSearchWorker_t qt_meta_stringdata_PackageSearchWorker = {
    {
        QT_MOC_LITERAL(0, 19),  // "PackageSearchWorker"
        QT_MOC_LITERAL(20, 14),  // "searchFinished"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 18),  // "QList<PackageInfo>"
        QT_MOC_LITERAL(55, 8),  // "packages"
        QT_MOC_LITERAL(64, 11),  // "searchError"
        QT_MOC_LITERAL(76, 5),  // "error"
        QT_MOC_LITERAL(82, 14),  // "searchProgress"
        QT_MOC_LITERAL(97, 7),  // "message"
        QT_MOC_LITERAL(105, 14),  // "searchPackages"
        QT_MOC_LITERAL(120, 10),  // "searchTerm"
        QT_MOC_LITERAL(131, 10),  // "searchType"
        QT_MOC_LITERAL(142, 18),  // "refreshAllPackages"
        QT_MOC_LITERAL(161, 6)   // "cancel"
    },
    "PackageSearchWorker",
    "searchFinished",
    "",
    "QList<PackageInfo>",
    "packages",
    "searchError",
    "error",
    "searchProgress",
    "message",
    "searchPackages",
    "searchTerm",
    "searchType",
    "refreshAllPackages",
    "cancel"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_PackageSearchWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    1,   53,    2, 0x06,    3 /* Public */,
       7,    1,   56,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    2,   59,    2, 0x0a,    7 /* Public */,
      12,    0,   64,    2, 0x0a,   10 /* Public */,
      13,    0,   65,    2, 0x0a,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PackageSearchWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PackageSearchWorker.offsetsAndSizes,
    qt_meta_data_PackageSearchWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_PackageSearchWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PackageSearchWorker, std::true_type>,
        // method 'searchFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<PackageInfo> &, std::false_type>,
        // method 'searchError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'searchProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'searchPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'refreshAllPackages'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cancel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PackageSearchWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PackageSearchWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->searchFinished((*reinterpret_cast< std::add_pointer_t<QList<PackageInfo>>>(_a[1]))); break;
        case 1: _t->searchError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->searchProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->searchPackages((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->refreshAllPackages(); break;
        case 5: _t->cancel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PackageSearchWorker::*)(const QList<PackageInfo> & );
            if (_t _q_method = &PackageSearchWorker::searchFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PackageSearchWorker::*)(const QString & );
            if (_t _q_method = &PackageSearchWorker::searchError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PackageSearchWorker::*)(const QString & );
            if (_t _q_method = &PackageSearchWorker::searchProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *PackageSearchWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PackageSearchWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PackageSearchWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PackageSearchWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void PackageSearchWorker::searchFinished(const QList<PackageInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PackageSearchWorker::searchError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PackageSearchWorker::searchProgress(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
