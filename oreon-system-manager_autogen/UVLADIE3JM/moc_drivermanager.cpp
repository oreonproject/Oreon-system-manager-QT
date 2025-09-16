/****************************************************************************
** Meta object code from reading C++ file 'drivermanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/drivermanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'drivermanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_HardwareScanner_t {
    uint offsetsAndSizes[26];
    char stringdata0[16];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[12];
    char stringdata5[11];
    char stringdata6[12];
    char stringdata7[11];
    char stringdata8[14];
    char stringdata9[13];
    char stringdata10[13];
    char stringdata11[14];
    char stringdata12[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_HardwareScanner_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_HardwareScanner_t qt_meta_stringdata_HardwareScanner = {
    {
        QT_MOC_LITERAL(0, 15),  // "HardwareScanner"
        QT_MOC_LITERAL(16, 13),  // "hardwareFound"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 12),  // "hardwareInfo"
        QT_MOC_LITERAL(44, 11),  // "driverFound"
        QT_MOC_LITERAL(56, 10),  // "driverInfo"
        QT_MOC_LITERAL(67, 11),  // "moduleFound"
        QT_MOC_LITERAL(79, 10),  // "moduleInfo"
        QT_MOC_LITERAL(90, 13),  // "firmwareFound"
        QT_MOC_LITERAL(104, 12),  // "firmwareInfo"
        QT_MOC_LITERAL(117, 12),  // "scanFinished"
        QT_MOC_LITERAL(130, 13),  // "errorOccurred"
        QT_MOC_LITERAL(144, 5)   // "error"
    },
    "HardwareScanner",
    "hardwareFound",
    "",
    "hardwareInfo",
    "driverFound",
    "driverInfo",
    "moduleFound",
    "moduleInfo",
    "firmwareFound",
    "firmwareInfo",
    "scanFinished",
    "errorOccurred",
    "error"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_HardwareScanner[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       4,    1,   53,    2, 0x06,    3 /* Public */,
       6,    1,   56,    2, 0x06,    5 /* Public */,
       8,    1,   59,    2, 0x06,    7 /* Public */,
      10,    0,   62,    2, 0x06,    9 /* Public */,
      11,    1,   63,    2, 0x06,   10 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject HardwareScanner::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_HardwareScanner.offsetsAndSizes,
    qt_meta_data_HardwareScanner,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_HardwareScanner_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HardwareScanner, std::true_type>,
        // method 'hardwareFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'driverFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'moduleFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'firmwareFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'scanFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void HardwareScanner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HardwareScanner *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->hardwareFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 1: _t->driverFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 2: _t->moduleFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->firmwareFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 4: _t->scanFinished(); break;
        case 5: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HardwareScanner::*)(const QJsonObject & );
            if (_t _q_method = &HardwareScanner::hardwareFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HardwareScanner::*)(const QJsonObject & );
            if (_t _q_method = &HardwareScanner::driverFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HardwareScanner::*)(const QJsonObject & );
            if (_t _q_method = &HardwareScanner::moduleFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HardwareScanner::*)(const QJsonObject & );
            if (_t _q_method = &HardwareScanner::firmwareFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HardwareScanner::*)();
            if (_t _q_method = &HardwareScanner::scanFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (HardwareScanner::*)(const QString & );
            if (_t _q_method = &HardwareScanner::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *HardwareScanner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HardwareScanner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HardwareScanner.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int HardwareScanner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void HardwareScanner::hardwareFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HardwareScanner::driverFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HardwareScanner::moduleFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HardwareScanner::firmwareFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HardwareScanner::scanFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void HardwareScanner::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
namespace {
struct qt_meta_stringdata_DriverManager_t {
    uint offsetsAndSizes[248];
    char stringdata0[14];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[24];
    char stringdata4[24];
    char stringdata5[26];
    char stringdata6[15];
    char stringdata7[22];
    char stringdata8[17];
    char stringdata9[17];
    char stringdata10[16];
    char stringdata11[13];
    char stringdata12[14];
    char stringdata13[11];
    char stringdata14[14];
    char stringdata15[11];
    char stringdata16[16];
    char stringdata17[13];
    char stringdata18[15];
    char stringdata19[12];
    char stringdata20[6];
    char stringdata21[27];
    char stringdata22[4];
    char stringdata23[25];
    char stringdata24[25];
    char stringdata25[27];
    char stringdata26[27];
    char stringdata27[25];
    char stringdata28[25];
    char stringdata29[27];
    char stringdata30[18];
    char stringdata31[7];
    char stringdata32[9];
    char stringdata33[8];
    char stringdata34[16];
    char stringdata35[8];
    char stringdata36[15];
    char stringdata37[15];
    char stringdata38[9];
    char stringdata39[7];
    char stringdata40[12];
    char stringdata41[14];
    char stringdata42[12];
    char stringdata43[15];
    char stringdata44[16];
    char stringdata45[14];
    char stringdata46[22];
    char stringdata47[13];
    char stringdata48[20];
    char stringdata49[18];
    char stringdata50[18];
    char stringdata51[20];
    char stringdata52[20];
    char stringdata53[18];
    char stringdata54[18];
    char stringdata55[20];
    char stringdata56[20];
    char stringdata57[18];
    char stringdata58[20];
    char stringdata59[18];
    char stringdata60[16];
    char stringdata61[14];
    char stringdata62[13];
    char stringdata63[13];
    char stringdata64[14];
    char stringdata65[16];
    char stringdata66[16];
    char stringdata67[11];
    char stringdata68[13];
    char stringdata69[13];
    char stringdata70[16];
    char stringdata71[16];
    char stringdata72[19];
    char stringdata73[22];
    char stringdata74[16];
    char stringdata75[15];
    char stringdata76[15];
    char stringdata77[17];
    char stringdata78[16];
    char stringdata79[18];
    char stringdata80[17];
    char stringdata81[16];
    char stringdata82[21];
    char stringdata83[23];
    char stringdata84[21];
    char stringdata85[23];
    char stringdata86[19];
    char stringdata87[20];
    char stringdata88[23];
    char stringdata89[17];
    char stringdata90[17];
    char stringdata91[19];
    char stringdata92[22];
    char stringdata93[26];
    char stringdata94[25];
    char stringdata95[21];
    char stringdata96[22];
    char stringdata97[18];
    char stringdata98[21];
    char stringdata99[20];
    char stringdata100[16];
    char stringdata101[13];
    char stringdata102[14];
    char stringdata103[13];
    char stringdata104[17];
    char stringdata105[15];
    char stringdata106[15];
    char stringdata107[17];
    char stringdata108[15];
    char stringdata109[14];
    char stringdata110[10];
    char stringdata111[10];
    char stringdata112[10];
    char stringdata113[9];
    char stringdata114[11];
    char stringdata115[9];
    char stringdata116[21];
    char stringdata117[23];
    char stringdata118[21];
    char stringdata119[21];
    char stringdata120[21];
    char stringdata121[13];
    char stringdata122[6];
    char stringdata123[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_DriverManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_DriverManager_t qt_meta_stringdata_DriverManager = {
    {
        QT_MOC_LITERAL(0, 13),  // "DriverManager"
        QT_MOC_LITERAL(14, 25),  // "onHardwareActionTriggered"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 23),  // "onDriverActionTriggered"
        QT_MOC_LITERAL(65, 23),  // "onModuleActionTriggered"
        QT_MOC_LITERAL(89, 25),  // "onFirmwareActionTriggered"
        QT_MOC_LITERAL(115, 14),  // "searchHardware"
        QT_MOC_LITERAL(130, 21),  // "installMissingDrivers"
        QT_MOC_LITERAL(152, 16),  // "updateAllDrivers"
        QT_MOC_LITERAL(169, 16),  // "rebuildInitramfs"
        QT_MOC_LITERAL(186, 15),  // "onHardwareFound"
        QT_MOC_LITERAL(202, 12),  // "hardwareInfo"
        QT_MOC_LITERAL(215, 13),  // "onDriverFound"
        QT_MOC_LITERAL(229, 10),  // "driverInfo"
        QT_MOC_LITERAL(240, 13),  // "onModuleFound"
        QT_MOC_LITERAL(254, 10),  // "moduleInfo"
        QT_MOC_LITERAL(265, 15),  // "onFirmwareFound"
        QT_MOC_LITERAL(281, 12),  // "firmwareInfo"
        QT_MOC_LITERAL(294, 14),  // "onScanFinished"
        QT_MOC_LITERAL(309, 11),  // "onScanError"
        QT_MOC_LITERAL(321, 5),  // "error"
        QT_MOC_LITERAL(327, 26),  // "onHardwareTableContextMenu"
        QT_MOC_LITERAL(354, 3),  // "pos"
        QT_MOC_LITERAL(358, 24),  // "onDriverTableContextMenu"
        QT_MOC_LITERAL(383, 24),  // "onModuleTableContextMenu"
        QT_MOC_LITERAL(408, 26),  // "onFirmwareTableContextMenu"
        QT_MOC_LITERAL(435, 26),  // "onHardwareSelectionChanged"
        QT_MOC_LITERAL(462, 24),  // "onDriverSelectionChanged"
        QT_MOC_LITERAL(487, 24),  // "onModuleSelectionChanged"
        QT_MOC_LITERAL(512, 26),  // "onFirmwareSelectionChanged"
        QT_MOC_LITERAL(539, 17),  // "onProgressUpdated"
        QT_MOC_LITERAL(557, 6),  // "taskId"
        QT_MOC_LITERAL(564, 8),  // "progress"
        QT_MOC_LITERAL(573, 7),  // "message"
        QT_MOC_LITERAL(581, 15),  // "onTaskCompleted"
        QT_MOC_LITERAL(597, 7),  // "success"
        QT_MOC_LITERAL(605, 14),  // "onTaskProgress"
        QT_MOC_LITERAL(620, 14),  // "onTaskFinished"
        QT_MOC_LITERAL(635, 8),  // "exitCode"
        QT_MOC_LITERAL(644, 6),  // "output"
        QT_MOC_LITERAL(651, 11),  // "onTaskError"
        QT_MOC_LITERAL(663, 13),  // "onTaskStarted"
        QT_MOC_LITERAL(677, 11),  // "description"
        QT_MOC_LITERAL(689, 14),  // "onRefreshTimer"
        QT_MOC_LITERAL(704, 15),  // "onKernelChanged"
        QT_MOC_LITERAL(720, 13),  // "kernelVersion"
        QT_MOC_LITERAL(734, 21),  // "onArchitectureChanged"
        QT_MOC_LITERAL(756, 12),  // "architecture"
        QT_MOC_LITERAL(769, 19),  // "updateHardwareTable"
        QT_MOC_LITERAL(789, 17),  // "updateDriverTable"
        QT_MOC_LITERAL(807, 17),  // "updateModuleTable"
        QT_MOC_LITERAL(825, 19),  // "updateFirmwareTable"
        QT_MOC_LITERAL(845, 19),  // "showHardwareDetails"
        QT_MOC_LITERAL(865, 17),  // "showDriverDetails"
        QT_MOC_LITERAL(883, 17),  // "showModuleDetails"
        QT_MOC_LITERAL(901, 19),  // "showFirmwareDetails"
        QT_MOC_LITERAL(921, 19),  // "showDriverInstaller"
        QT_MOC_LITERAL(941, 17),  // "showModuleManager"
        QT_MOC_LITERAL(959, 19),  // "showFirmwareUpdater"
        QT_MOC_LITERAL(979, 17),  // "showKernelManager"
        QT_MOC_LITERAL(997, 15),  // "showDKMSManager"
        QT_MOC_LITERAL(1013, 13),  // "installDriver"
        QT_MOC_LITERAL(1027, 12),  // "removeDriver"
        QT_MOC_LITERAL(1040, 12),  // "enableDriver"
        QT_MOC_LITERAL(1053, 13),  // "disableDriver"
        QT_MOC_LITERAL(1067, 15),  // "blacklistDriver"
        QT_MOC_LITERAL(1083, 15),  // "whitelistDriver"
        QT_MOC_LITERAL(1099, 10),  // "loadModule"
        QT_MOC_LITERAL(1110, 12),  // "unloadModule"
        QT_MOC_LITERAL(1123, 12),  // "reloadModule"
        QT_MOC_LITERAL(1136, 15),  // "blacklistModule"
        QT_MOC_LITERAL(1152, 15),  // "whitelistModule"
        QT_MOC_LITERAL(1168, 18),  // "addModuleParameter"
        QT_MOC_LITERAL(1187, 21),  // "removeModuleParameter"
        QT_MOC_LITERAL(1209, 15),  // "installFirmware"
        QT_MOC_LITERAL(1225, 14),  // "removeFirmware"
        QT_MOC_LITERAL(1240, 14),  // "updateFirmware"
        QT_MOC_LITERAL(1255, 16),  // "downloadFirmware"
        QT_MOC_LITERAL(1272, 15),  // "extractFirmware"
        QT_MOC_LITERAL(1288, 17),  // "installDKMSModule"
        QT_MOC_LITERAL(1306, 16),  // "removeDKMSModule"
        QT_MOC_LITERAL(1323, 15),  // "buildDKMSModule"
        QT_MOC_LITERAL(1339, 20),  // "installKernelHeaders"
        QT_MOC_LITERAL(1360, 22),  // "installBuildEssentials"
        QT_MOC_LITERAL(1383, 20),  // "checkDriverConflicts"
        QT_MOC_LITERAL(1404, 22),  // "resolveDriverConflicts"
        QT_MOC_LITERAL(1427, 18),  // "createDriverBackup"
        QT_MOC_LITERAL(1446, 19),  // "restoreDriverBackup"
        QT_MOC_LITERAL(1466, 22),  // "generateHardwareReport"
        QT_MOC_LITERAL(1489, 16),  // "exportDriverList"
        QT_MOC_LITERAL(1506, 16),  // "importDriverList"
        QT_MOC_LITERAL(1523, 18),  // "scanForNewHardware"
        QT_MOC_LITERAL(1542, 21),  // "detectMissingFirmware"
        QT_MOC_LITERAL(1564, 25),  // "installProprietaryDrivers"
        QT_MOC_LITERAL(1590, 24),  // "installOpenSourceDrivers"
        QT_MOC_LITERAL(1615, 20),  // "switchToNvidiaDriver"
        QT_MOC_LITERAL(1636, 21),  // "switchToNouveauDriver"
        QT_MOC_LITERAL(1658, 17),  // "switchToAMDDriver"
        QT_MOC_LITERAL(1676, 20),  // "switchToRadeonDriver"
        QT_MOC_LITERAL(1697, 19),  // "switchToIntelDriver"
        QT_MOC_LITERAL(1717, 15),  // "optimizeDrivers"
        QT_MOC_LITERAL(1733, 12),  // "updateKernel"
        QT_MOC_LITERAL(1746, 13),  // "installKernel"
        QT_MOC_LITERAL(1760, 12),  // "removeKernel"
        QT_MOC_LITERAL(1773, 16),  // "setDefaultKernel"
        QT_MOC_LITERAL(1790, 14),  // "showKernelLogs"
        QT_MOC_LITERAL(1805, 14),  // "showDriverLogs"
        QT_MOC_LITERAL(1820, 16),  // "showHardwareInfo"
        QT_MOC_LITERAL(1837, 14),  // "showSystemInfo"
        QT_MOC_LITERAL(1852, 13),  // "showDMIdecode"
        QT_MOC_LITERAL(1866, 9),  // "showLspci"
        QT_MOC_LITERAL(1876, 9),  // "showLsusb"
        QT_MOC_LITERAL(1886, 9),  // "showLsmod"
        QT_MOC_LITERAL(1896, 8),  // "showLshw"
        QT_MOC_LITERAL(1905, 10),  // "showHwinfo"
        QT_MOC_LITERAL(1916, 8),  // "showInxi"
        QT_MOC_LITERAL(1925, 20),  // "checkFirmwareUpdates"
        QT_MOC_LITERAL(1946, 22),  // "refreshFirmwareDevices"
        QT_MOC_LITERAL(1969, 20),  // "applyFirmwareUpdates"
        QT_MOC_LITERAL(1990, 20),  // "parseFirmwareDevices"
        QT_MOC_LITERAL(2011, 20),  // "parseFirmwareUpdates"
        QT_MOC_LITERAL(2032, 12),  // "showProgress"
        QT_MOC_LITERAL(2045, 5),  // "title"
        QT_MOC_LITERAL(2051, 12)   // "hideProgress"
    },
    "DriverManager",
    "onHardwareActionTriggered",
    "",
    "onDriverActionTriggered",
    "onModuleActionTriggered",
    "onFirmwareActionTriggered",
    "searchHardware",
    "installMissingDrivers",
    "updateAllDrivers",
    "rebuildInitramfs",
    "onHardwareFound",
    "hardwareInfo",
    "onDriverFound",
    "driverInfo",
    "onModuleFound",
    "moduleInfo",
    "onFirmwareFound",
    "firmwareInfo",
    "onScanFinished",
    "onScanError",
    "error",
    "onHardwareTableContextMenu",
    "pos",
    "onDriverTableContextMenu",
    "onModuleTableContextMenu",
    "onFirmwareTableContextMenu",
    "onHardwareSelectionChanged",
    "onDriverSelectionChanged",
    "onModuleSelectionChanged",
    "onFirmwareSelectionChanged",
    "onProgressUpdated",
    "taskId",
    "progress",
    "message",
    "onTaskCompleted",
    "success",
    "onTaskProgress",
    "onTaskFinished",
    "exitCode",
    "output",
    "onTaskError",
    "onTaskStarted",
    "description",
    "onRefreshTimer",
    "onKernelChanged",
    "kernelVersion",
    "onArchitectureChanged",
    "architecture",
    "updateHardwareTable",
    "updateDriverTable",
    "updateModuleTable",
    "updateFirmwareTable",
    "showHardwareDetails",
    "showDriverDetails",
    "showModuleDetails",
    "showFirmwareDetails",
    "showDriverInstaller",
    "showModuleManager",
    "showFirmwareUpdater",
    "showKernelManager",
    "showDKMSManager",
    "installDriver",
    "removeDriver",
    "enableDriver",
    "disableDriver",
    "blacklistDriver",
    "whitelistDriver",
    "loadModule",
    "unloadModule",
    "reloadModule",
    "blacklistModule",
    "whitelistModule",
    "addModuleParameter",
    "removeModuleParameter",
    "installFirmware",
    "removeFirmware",
    "updateFirmware",
    "downloadFirmware",
    "extractFirmware",
    "installDKMSModule",
    "removeDKMSModule",
    "buildDKMSModule",
    "installKernelHeaders",
    "installBuildEssentials",
    "checkDriverConflicts",
    "resolveDriverConflicts",
    "createDriverBackup",
    "restoreDriverBackup",
    "generateHardwareReport",
    "exportDriverList",
    "importDriverList",
    "scanForNewHardware",
    "detectMissingFirmware",
    "installProprietaryDrivers",
    "installOpenSourceDrivers",
    "switchToNvidiaDriver",
    "switchToNouveauDriver",
    "switchToAMDDriver",
    "switchToRadeonDriver",
    "switchToIntelDriver",
    "optimizeDrivers",
    "updateKernel",
    "installKernel",
    "removeKernel",
    "setDefaultKernel",
    "showKernelLogs",
    "showDriverLogs",
    "showHardwareInfo",
    "showSystemInfo",
    "showDMIdecode",
    "showLspci",
    "showLsusb",
    "showLsmod",
    "showLshw",
    "showHwinfo",
    "showInxi",
    "checkFirmwareUpdates",
    "refreshFirmwareDevices",
    "applyFirmwareUpdates",
    "parseFirmwareDevices",
    "parseFirmwareUpdates",
    "showProgress",
    "title",
    "hideProgress"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_DriverManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
     106,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  650,    2, 0x0a,    1 /* Public */,
       3,    0,  651,    2, 0x0a,    2 /* Public */,
       4,    0,  652,    2, 0x0a,    3 /* Public */,
       5,    0,  653,    2, 0x0a,    4 /* Public */,
       6,    0,  654,    2, 0x0a,    5 /* Public */,
       7,    0,  655,    2, 0x0a,    6 /* Public */,
       8,    0,  656,    2, 0x0a,    7 /* Public */,
       9,    0,  657,    2, 0x0a,    8 /* Public */,
      10,    1,  658,    2, 0x08,    9 /* Private */,
      12,    1,  661,    2, 0x08,   11 /* Private */,
      14,    1,  664,    2, 0x08,   13 /* Private */,
      16,    1,  667,    2, 0x08,   15 /* Private */,
      18,    0,  670,    2, 0x08,   17 /* Private */,
      19,    1,  671,    2, 0x08,   18 /* Private */,
      21,    1,  674,    2, 0x08,   20 /* Private */,
      23,    1,  677,    2, 0x08,   22 /* Private */,
      24,    1,  680,    2, 0x08,   24 /* Private */,
      25,    1,  683,    2, 0x08,   26 /* Private */,
      26,    0,  686,    2, 0x08,   28 /* Private */,
      27,    0,  687,    2, 0x08,   29 /* Private */,
      28,    0,  688,    2, 0x08,   30 /* Private */,
      29,    0,  689,    2, 0x08,   31 /* Private */,
      30,    3,  690,    2, 0x08,   32 /* Private */,
      34,    3,  697,    2, 0x08,   36 /* Private */,
      36,    2,  704,    2, 0x08,   40 /* Private */,
      37,    3,  709,    2, 0x08,   43 /* Private */,
      40,    2,  716,    2, 0x08,   47 /* Private */,
      41,    2,  721,    2, 0x08,   50 /* Private */,
      43,    0,  726,    2, 0x08,   53 /* Private */,
      44,    1,  727,    2, 0x08,   54 /* Private */,
      46,    1,  730,    2, 0x08,   56 /* Private */,
      48,    0,  733,    2, 0x08,   58 /* Private */,
      49,    0,  734,    2, 0x08,   59 /* Private */,
      50,    0,  735,    2, 0x08,   60 /* Private */,
      51,    0,  736,    2, 0x08,   61 /* Private */,
      52,    0,  737,    2, 0x08,   62 /* Private */,
      53,    0,  738,    2, 0x08,   63 /* Private */,
      54,    0,  739,    2, 0x08,   64 /* Private */,
      55,    0,  740,    2, 0x08,   65 /* Private */,
      56,    0,  741,    2, 0x08,   66 /* Private */,
      57,    0,  742,    2, 0x08,   67 /* Private */,
      58,    0,  743,    2, 0x08,   68 /* Private */,
      59,    0,  744,    2, 0x08,   69 /* Private */,
      60,    0,  745,    2, 0x08,   70 /* Private */,
      61,    0,  746,    2, 0x08,   71 /* Private */,
      62,    0,  747,    2, 0x08,   72 /* Private */,
      63,    0,  748,    2, 0x08,   73 /* Private */,
      64,    0,  749,    2, 0x08,   74 /* Private */,
      65,    0,  750,    2, 0x08,   75 /* Private */,
      66,    0,  751,    2, 0x08,   76 /* Private */,
      67,    0,  752,    2, 0x08,   77 /* Private */,
      68,    0,  753,    2, 0x08,   78 /* Private */,
      69,    0,  754,    2, 0x08,   79 /* Private */,
      70,    0,  755,    2, 0x08,   80 /* Private */,
      71,    0,  756,    2, 0x08,   81 /* Private */,
      72,    0,  757,    2, 0x08,   82 /* Private */,
      73,    0,  758,    2, 0x08,   83 /* Private */,
      74,    0,  759,    2, 0x08,   84 /* Private */,
      75,    0,  760,    2, 0x08,   85 /* Private */,
      76,    0,  761,    2, 0x08,   86 /* Private */,
      77,    0,  762,    2, 0x08,   87 /* Private */,
      78,    0,  763,    2, 0x08,   88 /* Private */,
      79,    0,  764,    2, 0x08,   89 /* Private */,
      80,    0,  765,    2, 0x08,   90 /* Private */,
      81,    0,  766,    2, 0x08,   91 /* Private */,
      82,    0,  767,    2, 0x08,   92 /* Private */,
      83,    0,  768,    2, 0x08,   93 /* Private */,
      84,    0,  769,    2, 0x08,   94 /* Private */,
      85,    0,  770,    2, 0x08,   95 /* Private */,
      86,    0,  771,    2, 0x08,   96 /* Private */,
      87,    0,  772,    2, 0x08,   97 /* Private */,
      88,    0,  773,    2, 0x08,   98 /* Private */,
      89,    0,  774,    2, 0x08,   99 /* Private */,
      90,    0,  775,    2, 0x08,  100 /* Private */,
      91,    0,  776,    2, 0x08,  101 /* Private */,
      92,    0,  777,    2, 0x08,  102 /* Private */,
      93,    0,  778,    2, 0x08,  103 /* Private */,
      94,    0,  779,    2, 0x08,  104 /* Private */,
      95,    0,  780,    2, 0x08,  105 /* Private */,
      96,    0,  781,    2, 0x08,  106 /* Private */,
      97,    0,  782,    2, 0x08,  107 /* Private */,
      98,    0,  783,    2, 0x08,  108 /* Private */,
      99,    0,  784,    2, 0x08,  109 /* Private */,
     100,    0,  785,    2, 0x08,  110 /* Private */,
     101,    0,  786,    2, 0x08,  111 /* Private */,
     102,    0,  787,    2, 0x08,  112 /* Private */,
     103,    0,  788,    2, 0x08,  113 /* Private */,
     104,    0,  789,    2, 0x08,  114 /* Private */,
     105,    0,  790,    2, 0x08,  115 /* Private */,
     106,    0,  791,    2, 0x08,  116 /* Private */,
     107,    0,  792,    2, 0x08,  117 /* Private */,
     108,    0,  793,    2, 0x08,  118 /* Private */,
     109,    0,  794,    2, 0x08,  119 /* Private */,
     110,    0,  795,    2, 0x08,  120 /* Private */,
     111,    0,  796,    2, 0x08,  121 /* Private */,
     112,    0,  797,    2, 0x08,  122 /* Private */,
     113,    0,  798,    2, 0x08,  123 /* Private */,
     114,    0,  799,    2, 0x08,  124 /* Private */,
     115,    0,  800,    2, 0x08,  125 /* Private */,
     116,    0,  801,    2, 0x08,  126 /* Private */,
     117,    0,  802,    2, 0x08,  127 /* Private */,
     118,    0,  803,    2, 0x08,  128 /* Private */,
     119,    1,  804,    2, 0x08,  129 /* Private */,
     120,    1,  807,    2, 0x08,  131 /* Private */,
     121,    2,  810,    2, 0x08,  133 /* Private */,
     123,    0,  815,    2, 0x08,  136 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   11,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void, QMetaType::QPoint,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   31,   32,   33,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   31,   35,   33,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   31,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   31,   38,   39,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   31,   20,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   31,   42,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   45,
    QMetaType::Void, QMetaType::QString,   47,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString,   39,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,  122,   33,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject DriverManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_DriverManager.offsetsAndSizes,
    qt_meta_data_DriverManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_DriverManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DriverManager, std::true_type>,
        // method 'onHardwareActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDriverActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onModuleActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFirmwareActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchHardware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installMissingDrivers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateAllDrivers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rebuildInitramfs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onHardwareFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onDriverFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onModuleFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onFirmwareFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onScanFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onScanError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onHardwareTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onDriverTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onModuleTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onFirmwareTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onHardwareSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDriverSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onModuleSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFirmwareSelectionChanged'
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
        // method 'onTaskProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onTaskStarted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onRefreshTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onKernelChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onArchitectureChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateHardwareTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDriverTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateModuleTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFirmwareTable'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHardwareDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDriverDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showModuleDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showFirmwareDetails'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDriverInstaller'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showModuleManager'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showFirmwareUpdater'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showKernelManager'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDKMSManager'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disableDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'blacklistDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'whitelistDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'unloadModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reloadModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'blacklistModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'whitelistModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addModuleParameter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeModuleParameter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'extractFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installDKMSModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeDKMSModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buildDKMSModule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installKernelHeaders'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installBuildEssentials'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkDriverConflicts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resolveDriverConflicts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createDriverBackup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restoreDriverBackup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'generateHardwareReport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportDriverList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'importDriverList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scanForNewHardware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'detectMissingFirmware'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installProprietaryDrivers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installOpenSourceDrivers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToNvidiaDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToNouveauDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToAMDDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToRadeonDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'switchToIntelDriver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'optimizeDrivers'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateKernel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installKernel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeKernel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setDefaultKernel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showKernelLogs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDriverLogs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHardwareInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showSystemInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDMIdecode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showLspci'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showLsusb'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showLsmod'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showLshw'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHwinfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showInxi'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkFirmwareUpdates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshFirmwareDevices'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'applyFirmwareUpdates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'parseFirmwareDevices'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'parseFirmwareUpdates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'hideProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void DriverManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DriverManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onHardwareActionTriggered(); break;
        case 1: _t->onDriverActionTriggered(); break;
        case 2: _t->onModuleActionTriggered(); break;
        case 3: _t->onFirmwareActionTriggered(); break;
        case 4: _t->searchHardware(); break;
        case 5: _t->installMissingDrivers(); break;
        case 6: _t->updateAllDrivers(); break;
        case 7: _t->rebuildInitramfs(); break;
        case 8: _t->onHardwareFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 9: _t->onDriverFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 10: _t->onModuleFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 11: _t->onFirmwareFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 12: _t->onScanFinished(); break;
        case 13: _t->onScanError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->onHardwareTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 15: _t->onDriverTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 16: _t->onModuleTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 17: _t->onFirmwareTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 18: _t->onHardwareSelectionChanged(); break;
        case 19: _t->onDriverSelectionChanged(); break;
        case 20: _t->onModuleSelectionChanged(); break;
        case 21: _t->onFirmwareSelectionChanged(); break;
        case 22: _t->onProgressUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 23: _t->onTaskCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 24: _t->onTaskProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 25: _t->onTaskFinished((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 26: _t->onTaskError((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 27: _t->onTaskStarted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 28: _t->onRefreshTimer(); break;
        case 29: _t->onKernelChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 30: _t->onArchitectureChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 31: _t->updateHardwareTable(); break;
        case 32: _t->updateDriverTable(); break;
        case 33: _t->updateModuleTable(); break;
        case 34: _t->updateFirmwareTable(); break;
        case 35: _t->showHardwareDetails(); break;
        case 36: _t->showDriverDetails(); break;
        case 37: _t->showModuleDetails(); break;
        case 38: _t->showFirmwareDetails(); break;
        case 39: _t->showDriverInstaller(); break;
        case 40: _t->showModuleManager(); break;
        case 41: _t->showFirmwareUpdater(); break;
        case 42: _t->showKernelManager(); break;
        case 43: _t->showDKMSManager(); break;
        case 44: _t->installDriver(); break;
        case 45: _t->removeDriver(); break;
        case 46: _t->enableDriver(); break;
        case 47: _t->disableDriver(); break;
        case 48: _t->blacklistDriver(); break;
        case 49: _t->whitelistDriver(); break;
        case 50: _t->loadModule(); break;
        case 51: _t->unloadModule(); break;
        case 52: _t->reloadModule(); break;
        case 53: _t->blacklistModule(); break;
        case 54: _t->whitelistModule(); break;
        case 55: _t->addModuleParameter(); break;
        case 56: _t->removeModuleParameter(); break;
        case 57: _t->installFirmware(); break;
        case 58: _t->removeFirmware(); break;
        case 59: _t->updateFirmware(); break;
        case 60: _t->downloadFirmware(); break;
        case 61: _t->extractFirmware(); break;
        case 62: _t->installDKMSModule(); break;
        case 63: _t->removeDKMSModule(); break;
        case 64: _t->buildDKMSModule(); break;
        case 65: _t->installKernelHeaders(); break;
        case 66: _t->installBuildEssentials(); break;
        case 67: _t->checkDriverConflicts(); break;
        case 68: _t->resolveDriverConflicts(); break;
        case 69: _t->createDriverBackup(); break;
        case 70: _t->restoreDriverBackup(); break;
        case 71: _t->generateHardwareReport(); break;
        case 72: _t->exportDriverList(); break;
        case 73: _t->importDriverList(); break;
        case 74: _t->scanForNewHardware(); break;
        case 75: _t->detectMissingFirmware(); break;
        case 76: _t->installProprietaryDrivers(); break;
        case 77: _t->installOpenSourceDrivers(); break;
        case 78: _t->switchToNvidiaDriver(); break;
        case 79: _t->switchToNouveauDriver(); break;
        case 80: _t->switchToAMDDriver(); break;
        case 81: _t->switchToRadeonDriver(); break;
        case 82: _t->switchToIntelDriver(); break;
        case 83: _t->optimizeDrivers(); break;
        case 84: _t->updateKernel(); break;
        case 85: _t->installKernel(); break;
        case 86: _t->removeKernel(); break;
        case 87: _t->setDefaultKernel(); break;
        case 88: _t->showKernelLogs(); break;
        case 89: _t->showDriverLogs(); break;
        case 90: _t->showHardwareInfo(); break;
        case 91: _t->showSystemInfo(); break;
        case 92: _t->showDMIdecode(); break;
        case 93: _t->showLspci(); break;
        case 94: _t->showLsusb(); break;
        case 95: _t->showLsmod(); break;
        case 96: _t->showLshw(); break;
        case 97: _t->showHwinfo(); break;
        case 98: _t->showInxi(); break;
        case 99: _t->checkFirmwareUpdates(); break;
        case 100: _t->refreshFirmwareDevices(); break;
        case 101: _t->applyFirmwareUpdates(); break;
        case 102: _t->parseFirmwareDevices((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 103: _t->parseFirmwareUpdates((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 104: _t->showProgress((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 105: _t->hideProgress(); break;
        default: ;
        }
    }
}

const QMetaObject *DriverManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DriverManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DriverManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DriverManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 106)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 106;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 106)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 106;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
