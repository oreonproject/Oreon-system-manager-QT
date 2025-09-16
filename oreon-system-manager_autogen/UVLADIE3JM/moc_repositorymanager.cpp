/****************************************************************************
** Meta object code from reading C++ file 'repositorymanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/repositorymanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'repositorymanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_RepositoryManager_t {
    uint offsetsAndSizes[76];
    char stringdata0[18];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[17];
    char stringdata5[7];
    char stringdata6[18];
    char stringdata7[14];
    char stringdata8[17];
    char stringdata9[15];
    char stringdata10[17];
    char stringdata11[20];
    char stringdata12[20];
    char stringdata13[11];
    char stringdata14[21];
    char stringdata15[15];
    char stringdata16[27];
    char stringdata17[4];
    char stringdata18[16];
    char stringdata19[26];
    char stringdata20[7];
    char stringdata21[24];
    char stringdata22[6];
    char stringdata23[27];
    char stringdata24[9];
    char stringdata25[13];
    char stringdata26[8];
    char stringdata27[13];
    char stringdata28[23];
    char stringdata29[18];
    char stringdata30[5];
    char stringdata31[23];
    char stringdata32[26];
    char stringdata33[26];
    char stringdata34[22];
    char stringdata35[4];
    char stringdata36[7];
    char stringdata37[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RepositoryManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RepositoryManager_t qt_meta_stringdata_RepositoryManager = {
    {
        QT_MOC_LITERAL(0, 17),  // "RepositoryManager"
        QT_MOC_LITERAL(18, 19),  // "refreshRepositories"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 21),  // "refreshFlatpakRemotes"
        QT_MOC_LITERAL(61, 16),  // "enableRepository"
        QT_MOC_LITERAL(78, 6),  // "repoId"
        QT_MOC_LITERAL(85, 17),  // "disableRepository"
        QT_MOC_LITERAL(103, 13),  // "addRepository"
        QT_MOC_LITERAL(117, 16),  // "removeRepository"
        QT_MOC_LITERAL(134, 14),  // "editRepository"
        QT_MOC_LITERAL(149, 16),  // "addFlatpakRemote"
        QT_MOC_LITERAL(166, 19),  // "removeFlatpakRemote"
        QT_MOC_LITERAL(186, 19),  // "enableFlatpakRemote"
        QT_MOC_LITERAL(206, 10),  // "remoteName"
        QT_MOC_LITERAL(217, 20),  // "disableFlatpakRemote"
        QT_MOC_LITERAL(238, 14),  // "installFlatpak"
        QT_MOC_LITERAL(253, 26),  // "addPredefinedFlatpakRemote"
        QT_MOC_LITERAL(280, 3),  // "url"
        QT_MOC_LITERAL(284, 15),  // "quickAddFlathub"
        QT_MOC_LITERAL(300, 25),  // "onRepositoryActionSuccess"
        QT_MOC_LITERAL(326, 6),  // "output"
        QT_MOC_LITERAL(333, 23),  // "onRepositoryActionError"
        QT_MOC_LITERAL(357, 5),  // "error"
        QT_MOC_LITERAL(363, 26),  // "onRepositoryActionProgress"
        QT_MOC_LITERAL(390, 8),  // "progress"
        QT_MOC_LITERAL(399, 12),  // "showProgress"
        QT_MOC_LITERAL(412, 7),  // "message"
        QT_MOC_LITERAL(420, 12),  // "hideProgress"
        QT_MOC_LITERAL(433, 22),  // "onRepoTableItemChanged"
        QT_MOC_LITERAL(456, 17),  // "QTableWidgetItem*"
        QT_MOC_LITERAL(474, 4),  // "item"
        QT_MOC_LITERAL(479, 22),  // "onRepoSelectionChanged"
        QT_MOC_LITERAL(502, 25),  // "onFlatpakTableItemChanged"
        QT_MOC_LITERAL(528, 25),  // "onFlatpakSelectionChanged"
        QT_MOC_LITERAL(554, 21),  // "showRepositoryDetails"
        QT_MOC_LITERAL(576, 3),  // "row"
        QT_MOC_LITERAL(580, 6),  // "column"
        QT_MOC_LITERAL(587, 18)   // "showFlatpakDetails"
    },
    "RepositoryManager",
    "refreshRepositories",
    "",
    "refreshFlatpakRemotes",
    "enableRepository",
    "repoId",
    "disableRepository",
    "addRepository",
    "removeRepository",
    "editRepository",
    "addFlatpakRemote",
    "removeFlatpakRemote",
    "enableFlatpakRemote",
    "remoteName",
    "disableFlatpakRemote",
    "installFlatpak",
    "addPredefinedFlatpakRemote",
    "url",
    "quickAddFlathub",
    "onRepositoryActionSuccess",
    "output",
    "onRepositoryActionError",
    "error",
    "onRepositoryActionProgress",
    "progress",
    "showProgress",
    "message",
    "hideProgress",
    "onRepoTableItemChanged",
    "QTableWidgetItem*",
    "item",
    "onRepoSelectionChanged",
    "onFlatpakTableItemChanged",
    "onFlatpakSelectionChanged",
    "showRepositoryDetails",
    "row",
    "column",
    "showFlatpakDetails"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RepositoryManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  164,    2, 0x0a,    1 /* Public */,
       3,    0,  165,    2, 0x0a,    2 /* Public */,
       4,    1,  166,    2, 0x0a,    3 /* Public */,
       6,    1,  169,    2, 0x0a,    5 /* Public */,
       7,    0,  172,    2, 0x0a,    7 /* Public */,
       8,    0,  173,    2, 0x0a,    8 /* Public */,
       9,    0,  174,    2, 0x0a,    9 /* Public */,
      10,    0,  175,    2, 0x0a,   10 /* Public */,
      11,    0,  176,    2, 0x0a,   11 /* Public */,
      12,    1,  177,    2, 0x0a,   12 /* Public */,
      14,    1,  180,    2, 0x0a,   14 /* Public */,
      15,    0,  183,    2, 0x0a,   16 /* Public */,
      16,    2,  184,    2, 0x0a,   17 /* Public */,
      18,    0,  189,    2, 0x0a,   20 /* Public */,
      19,    1,  190,    2, 0x0a,   21 /* Public */,
      21,    1,  193,    2, 0x0a,   23 /* Public */,
      23,    1,  196,    2, 0x0a,   25 /* Public */,
      25,    1,  199,    2, 0x0a,   27 /* Public */,
      27,    0,  202,    2, 0x0a,   29 /* Public */,
      28,    1,  203,    2, 0x08,   30 /* Private */,
      31,    0,  206,    2, 0x08,   32 /* Private */,
      32,    1,  207,    2, 0x08,   33 /* Private */,
      33,    0,  210,    2, 0x08,   35 /* Private */,
      34,    2,  211,    2, 0x08,   36 /* Private */,
      37,    2,  216,    2, 0x08,   39 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   35,   36,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   35,   36,

       0        // eod
};

Q_CONSTINIT const QMetaObject RepositoryManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_RepositoryManager.offsetsAndSizes,
    qt_meta_data_RepositoryManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RepositoryManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RepositoryManager, std::true_type>,
        // method 'refreshRepositories'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshFlatpakRemotes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableRepository'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'disableRepository'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addRepository'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeRepository'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'editRepository'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addFlatpakRemote'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeFlatpakRemote'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableFlatpakRemote'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'disableFlatpakRemote'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'installFlatpak'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addPredefinedFlatpakRemote'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'quickAddFlathub'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRepositoryActionSuccess'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRepositoryActionError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRepositoryActionProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'hideProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRepoTableItemChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidgetItem *, std::false_type>,
        // method 'onRepoSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFlatpakTableItemChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidgetItem *, std::false_type>,
        // method 'onFlatpakSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showRepositoryDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showFlatpakDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void RepositoryManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RepositoryManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshRepositories(); break;
        case 1: _t->refreshFlatpakRemotes(); break;
        case 2: _t->enableRepository((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->disableRepository((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->addRepository(); break;
        case 5: _t->removeRepository(); break;
        case 6: _t->editRepository(); break;
        case 7: _t->addFlatpakRemote(); break;
        case 8: _t->removeFlatpakRemote(); break;
        case 9: _t->enableFlatpakRemote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->disableFlatpakRemote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->installFlatpak(); break;
        case 12: _t->addPredefinedFlatpakRemote((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->quickAddFlathub(); break;
        case 14: _t->onRepositoryActionSuccess((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->onRepositoryActionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->onRepositoryActionProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->showProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->hideProgress(); break;
        case 19: _t->onRepoTableItemChanged((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 20: _t->onRepoSelectionChanged(); break;
        case 21: _t->onFlatpakTableItemChanged((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 22: _t->onFlatpakSelectionChanged(); break;
        case 23: _t->showRepositoryDetails((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 24: _t->showFlatpakDetails((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *RepositoryManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RepositoryManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RepositoryManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RepositoryManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
