/****************************************************************************
** Meta object code from reading C++ file 'audiomanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/audiomanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiomanager.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_AudioDeviceWorker_t {
    uint offsetsAndSizes[22];
    char stringdata0[18];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[13];
    char stringdata5[12];
    char stringdata6[10];
    char stringdata7[9];
    char stringdata8[13];
    char stringdata9[14];
    char stringdata10[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AudioDeviceWorker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AudioDeviceWorker_t qt_meta_stringdata_AudioDeviceWorker = {
    {
        QT_MOC_LITERAL(0, 17),  // "AudioDeviceWorker"
        QT_MOC_LITERAL(18, 11),  // "deviceFound"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 10),  // "deviceInfo"
        QT_MOC_LITERAL(42, 12),  // "profileFound"
        QT_MOC_LITERAL(55, 11),  // "profileInfo"
        QT_MOC_LITERAL(67, 9),  // "cardFound"
        QT_MOC_LITERAL(77, 8),  // "cardInfo"
        QT_MOC_LITERAL(86, 12),  // "scanFinished"
        QT_MOC_LITERAL(99, 13),  // "errorOccurred"
        QT_MOC_LITERAL(113, 5)   // "error"
    },
    "AudioDeviceWorker",
    "deviceFound",
    "",
    "deviceInfo",
    "profileFound",
    "profileInfo",
    "cardFound",
    "cardInfo",
    "scanFinished",
    "errorOccurred",
    "error"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AudioDeviceWorker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       4,    1,   47,    2, 0x06,    3 /* Public */,
       6,    1,   50,    2, 0x06,    5 /* Public */,
       8,    0,   53,    2, 0x06,    7 /* Public */,
       9,    1,   54,    2, 0x06,    8 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

Q_CONSTINIT const QMetaObject AudioDeviceWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_AudioDeviceWorker.offsetsAndSizes,
    qt_meta_data_AudioDeviceWorker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AudioDeviceWorker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AudioDeviceWorker, std::true_type>,
        // method 'deviceFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'profileFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'cardFound'
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

void AudioDeviceWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudioDeviceWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->deviceFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 1: _t->profileFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 2: _t->cardFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->scanFinished(); break;
        case 4: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AudioDeviceWorker::*)(const QJsonObject & );
            if (_t _q_method = &AudioDeviceWorker::deviceFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AudioDeviceWorker::*)(const QJsonObject & );
            if (_t _q_method = &AudioDeviceWorker::profileFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AudioDeviceWorker::*)(const QJsonObject & );
            if (_t _q_method = &AudioDeviceWorker::cardFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (AudioDeviceWorker::*)();
            if (_t _q_method = &AudioDeviceWorker::scanFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (AudioDeviceWorker::*)(const QString & );
            if (_t _q_method = &AudioDeviceWorker::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *AudioDeviceWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioDeviceWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioDeviceWorker.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int AudioDeviceWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AudioDeviceWorker::deviceFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AudioDeviceWorker::profileFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AudioDeviceWorker::cardFound(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AudioDeviceWorker::scanFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AudioDeviceWorker::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
namespace {
struct qt_meta_stringdata_AudioManager_t {
    uint offsetsAndSizes[200];
    char stringdata0[13];
    char stringdata1[24];
    char stringdata2[1];
    char stringdata3[25];
    char stringdata4[24];
    char stringdata5[14];
    char stringdata6[18];
    char stringdata7[19];
    char stringdata8[14];
    char stringdata9[11];
    char stringdata10[15];
    char stringdata11[12];
    char stringdata12[12];
    char stringdata13[9];
    char stringdata14[15];
    char stringdata15[12];
    char stringdata16[6];
    char stringdata17[25];
    char stringdata18[4];
    char stringdata19[26];
    char stringdata20[25];
    char stringdata21[25];
    char stringdata22[26];
    char stringdata23[25];
    char stringdata24[18];
    char stringdata25[7];
    char stringdata26[9];
    char stringdata27[8];
    char stringdata28[16];
    char stringdata29[8];
    char stringdata30[15];
    char stringdata31[16];
    char stringdata32[6];
    char stringdata33[14];
    char stringdata34[6];
    char stringdata35[16];
    char stringdata36[8];
    char stringdata37[17];
    char stringdata38[12];
    char stringdata39[16];
    char stringdata40[11];
    char stringdata41[20];
    char stringdata42[11];
    char stringdata43[20];
    char stringdata44[11];
    char stringdata45[17];
    char stringdata46[8];
    char stringdata47[21];
    char stringdata48[18];
    char stringdata49[17];
    char stringdata50[23];
    char stringdata51[21];
    char stringdata52[21];
    char stringdata53[14];
    char stringdata54[14];
    char stringdata55[14];
    char stringdata56[14];
    char stringdata57[19];
    char stringdata58[17];
    char stringdata59[16];
    char stringdata60[27];
    char stringdata61[28];
    char stringdata62[22];
    char stringdata63[22];
    char stringdata64[24];
    char stringdata65[23];
    char stringdata66[16];
    char stringdata67[14];
    char stringdata68[13];
    char stringdata69[16];
    char stringdata70[24];
    char stringdata71[25];
    char stringdata72[18];
    char stringdata73[19];
    char stringdata74[19];
    char stringdata75[23];
    char stringdata76[17];
    char stringdata77[21];
    char stringdata78[18];
    char stringdata79[22];
    char stringdata80[23];
    char stringdata81[19];
    char stringdata82[18];
    char stringdata83[19];
    char stringdata84[23];
    char stringdata85[19];
    char stringdata86[22];
    char stringdata87[20];
    char stringdata88[22];
    char stringdata89[29];
    char stringdata90[16];
    char stringdata91[7];
    char stringdata92[14];
    char stringdata93[15];
    char stringdata94[13];
    char stringdata95[14];
    char stringdata96[14];
    char stringdata97[19];
    char stringdata98[21];
    char stringdata99[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AudioManager_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_AudioManager_t qt_meta_stringdata_AudioManager = {
    {
        QT_MOC_LITERAL(0, 12),  // "AudioManager"
        QT_MOC_LITERAL(13, 23),  // "onDeviceActionTriggered"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 24),  // "onProfileActionTriggered"
        QT_MOC_LITERAL(63, 23),  // "onEffectActionTriggered"
        QT_MOC_LITERAL(87, 13),  // "searchDevices"
        QT_MOC_LITERAL(101, 17),  // "applyAudioProfile"
        QT_MOC_LITERAL(119, 18),  // "resetAudioSettings"
        QT_MOC_LITERAL(138, 13),  // "onDeviceFound"
        QT_MOC_LITERAL(152, 10),  // "deviceInfo"
        QT_MOC_LITERAL(163, 14),  // "onProfileFound"
        QT_MOC_LITERAL(178, 11),  // "profileInfo"
        QT_MOC_LITERAL(190, 11),  // "onCardFound"
        QT_MOC_LITERAL(202, 8),  // "cardInfo"
        QT_MOC_LITERAL(211, 14),  // "onScanFinished"
        QT_MOC_LITERAL(226, 11),  // "onScanError"
        QT_MOC_LITERAL(238, 5),  // "error"
        QT_MOC_LITERAL(244, 24),  // "onDeviceTableContextMenu"
        QT_MOC_LITERAL(269, 3),  // "pos"
        QT_MOC_LITERAL(273, 25),  // "onProfileTableContextMenu"
        QT_MOC_LITERAL(299, 24),  // "onEffectTableContextMenu"
        QT_MOC_LITERAL(324, 24),  // "onDeviceSelectionChanged"
        QT_MOC_LITERAL(349, 25),  // "onProfileSelectionChanged"
        QT_MOC_LITERAL(375, 24),  // "onEffectSelectionChanged"
        QT_MOC_LITERAL(400, 17),  // "onProgressUpdated"
        QT_MOC_LITERAL(418, 6),  // "taskId"
        QT_MOC_LITERAL(425, 8),  // "progress"
        QT_MOC_LITERAL(434, 7),  // "message"
        QT_MOC_LITERAL(442, 15),  // "onTaskCompleted"
        QT_MOC_LITERAL(458, 7),  // "success"
        QT_MOC_LITERAL(466, 14),  // "onRefreshTimer"
        QT_MOC_LITERAL(481, 15),  // "onVolumeChanged"
        QT_MOC_LITERAL(497, 5),  // "value"
        QT_MOC_LITERAL(503, 13),  // "onMuteToggled"
        QT_MOC_LITERAL(517, 5),  // "muted"
        QT_MOC_LITERAL(523, 15),  // "onEffectToggled"
        QT_MOC_LITERAL(539, 7),  // "enabled"
        QT_MOC_LITERAL(547, 16),  // "onProfileChanged"
        QT_MOC_LITERAL(564, 11),  // "profileName"
        QT_MOC_LITERAL(576, 15),  // "onDeviceChanged"
        QT_MOC_LITERAL(592, 10),  // "deviceName"
        QT_MOC_LITERAL(603, 19),  // "onSampleRateChanged"
        QT_MOC_LITERAL(623, 10),  // "sampleRate"
        QT_MOC_LITERAL(634, 19),  // "onBufferSizeChanged"
        QT_MOC_LITERAL(654, 10),  // "bufferSize"
        QT_MOC_LITERAL(665, 16),  // "onLatencyChanged"
        QT_MOC_LITERAL(682, 7),  // "latency"
        QT_MOC_LITERAL(690, 20),  // "showDeviceProperties"
        QT_MOC_LITERAL(711, 17),  // "showProfileEditor"
        QT_MOC_LITERAL(729, 16),  // "showEffectEditor"
        QT_MOC_LITERAL(746, 22),  // "showEasyEffectsPresets"
        QT_MOC_LITERAL(769, 20),  // "showPipeWireSettings"
        QT_MOC_LITERAL(790, 20),  // "showAdvancedSettings"
        QT_MOC_LITERAL(811, 13),  // "importProfile"
        QT_MOC_LITERAL(825, 13),  // "exportProfile"
        QT_MOC_LITERAL(839, 13),  // "createProfile"
        QT_MOC_LITERAL(853, 13),  // "deleteProfile"
        QT_MOC_LITERAL(867, 18),  // "installEasyEffects"
        QT_MOC_LITERAL(886, 16),  // "startEasyEffects"
        QT_MOC_LITERAL(903, 15),  // "stopEasyEffects"
        QT_MOC_LITERAL(919, 26),  // "enableEasyEffectsAutostart"
        QT_MOC_LITERAL(946, 27),  // "disableEasyEffectsAutostart"
        QT_MOC_LITERAL(974, 21),  // "loadEasyEffectsPreset"
        QT_MOC_LITERAL(996, 21),  // "saveEasyEffectsPreset"
        QT_MOC_LITERAL(1018, 23),  // "deleteEasyEffectsPreset"
        QT_MOC_LITERAL(1042, 22),  // "resetEasyEffectsPreset"
        QT_MOC_LITERAL(1065, 15),  // "installPipeWire"
        QT_MOC_LITERAL(1081, 13),  // "startPipeWire"
        QT_MOC_LITERAL(1095, 12),  // "stopPipeWire"
        QT_MOC_LITERAL(1108, 15),  // "restartPipeWire"
        QT_MOC_LITERAL(1124, 23),  // "enablePipeWireAutostart"
        QT_MOC_LITERAL(1148, 24),  // "disablePipeWireAutostart"
        QT_MOC_LITERAL(1173, 17),  // "showPipeWireGraph"
        QT_MOC_LITERAL(1191, 18),  // "optimizeForLatency"
        QT_MOC_LITERAL(1210, 18),  // "optimizeForQuality"
        QT_MOC_LITERAL(1229, 22),  // "optimizeForPowerSaving"
        QT_MOC_LITERAL(1252, 16),  // "testAudioDevices"
        QT_MOC_LITERAL(1269, 20),  // "calibrateAudioLevels"
        QT_MOC_LITERAL(1290, 17),  // "showAudioAnalyzer"
        QT_MOC_LITERAL(1308, 21),  // "showEqualizerSettings"
        QT_MOC_LITERAL(1330, 22),  // "showCompressorSettings"
        QT_MOC_LITERAL(1353, 18),  // "showReverbSettings"
        QT_MOC_LITERAL(1372, 17),  // "showDelaySettings"
        QT_MOC_LITERAL(1390, 18),  // "showChorusSettings"
        QT_MOC_LITERAL(1409, 22),  // "showDistortionSettings"
        QT_MOC_LITERAL(1432, 18),  // "showFilterSettings"
        QT_MOC_LITERAL(1451, 21),  // "showNoiseGateSettings"
        QT_MOC_LITERAL(1473, 19),  // "showLimiterSettings"
        QT_MOC_LITERAL(1493, 21),  // "showMasteringSettings"
        QT_MOC_LITERAL(1515, 28),  // "setupSimplifiedAudioControls"
        QT_MOC_LITERAL(1544, 15),  // "setMasterVolume"
        QT_MOC_LITERAL(1560, 6),  // "volume"
        QT_MOC_LITERAL(1567, 13),  // "setMasterMute"
        QT_MOC_LITERAL(1581, 14),  // "setInputVolume"
        QT_MOC_LITERAL(1596, 12),  // "setInputMute"
        QT_MOC_LITERAL(1609, 13),  // "setSampleRate"
        QT_MOC_LITERAL(1623, 13),  // "setBufferSize"
        QT_MOC_LITERAL(1637, 18),  // "editPipeWireConfig"
        QT_MOC_LITERAL(1656, 20),  // "reloadPipeWireConfig"
        QT_MOC_LITERAL(1677, 17)   // "launchEasyEffects"
    },
    "AudioManager",
    "onDeviceActionTriggered",
    "",
    "onProfileActionTriggered",
    "onEffectActionTriggered",
    "searchDevices",
    "applyAudioProfile",
    "resetAudioSettings",
    "onDeviceFound",
    "deviceInfo",
    "onProfileFound",
    "profileInfo",
    "onCardFound",
    "cardInfo",
    "onScanFinished",
    "onScanError",
    "error",
    "onDeviceTableContextMenu",
    "pos",
    "onProfileTableContextMenu",
    "onEffectTableContextMenu",
    "onDeviceSelectionChanged",
    "onProfileSelectionChanged",
    "onEffectSelectionChanged",
    "onProgressUpdated",
    "taskId",
    "progress",
    "message",
    "onTaskCompleted",
    "success",
    "onRefreshTimer",
    "onVolumeChanged",
    "value",
    "onMuteToggled",
    "muted",
    "onEffectToggled",
    "enabled",
    "onProfileChanged",
    "profileName",
    "onDeviceChanged",
    "deviceName",
    "onSampleRateChanged",
    "sampleRate",
    "onBufferSizeChanged",
    "bufferSize",
    "onLatencyChanged",
    "latency",
    "showDeviceProperties",
    "showProfileEditor",
    "showEffectEditor",
    "showEasyEffectsPresets",
    "showPipeWireSettings",
    "showAdvancedSettings",
    "importProfile",
    "exportProfile",
    "createProfile",
    "deleteProfile",
    "installEasyEffects",
    "startEasyEffects",
    "stopEasyEffects",
    "enableEasyEffectsAutostart",
    "disableEasyEffectsAutostart",
    "loadEasyEffectsPreset",
    "saveEasyEffectsPreset",
    "deleteEasyEffectsPreset",
    "resetEasyEffectsPreset",
    "installPipeWire",
    "startPipeWire",
    "stopPipeWire",
    "restartPipeWire",
    "enablePipeWireAutostart",
    "disablePipeWireAutostart",
    "showPipeWireGraph",
    "optimizeForLatency",
    "optimizeForQuality",
    "optimizeForPowerSaving",
    "testAudioDevices",
    "calibrateAudioLevels",
    "showAudioAnalyzer",
    "showEqualizerSettings",
    "showCompressorSettings",
    "showReverbSettings",
    "showDelaySettings",
    "showChorusSettings",
    "showDistortionSettings",
    "showFilterSettings",
    "showNoiseGateSettings",
    "showLimiterSettings",
    "showMasteringSettings",
    "setupSimplifiedAudioControls",
    "setMasterVolume",
    "volume",
    "setMasterMute",
    "setInputVolume",
    "setInputMute",
    "setSampleRate",
    "setBufferSize",
    "editPipeWireConfig",
    "reloadPipeWireConfig",
    "launchEasyEffects"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_AudioManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      80,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  494,    2, 0x0a,    1 /* Public */,
       3,    0,  495,    2, 0x0a,    2 /* Public */,
       4,    0,  496,    2, 0x0a,    3 /* Public */,
       5,    0,  497,    2, 0x0a,    4 /* Public */,
       6,    0,  498,    2, 0x0a,    5 /* Public */,
       7,    0,  499,    2, 0x0a,    6 /* Public */,
       8,    1,  500,    2, 0x08,    7 /* Private */,
      10,    1,  503,    2, 0x08,    9 /* Private */,
      12,    1,  506,    2, 0x08,   11 /* Private */,
      14,    0,  509,    2, 0x08,   13 /* Private */,
      15,    1,  510,    2, 0x08,   14 /* Private */,
      17,    1,  513,    2, 0x08,   16 /* Private */,
      19,    1,  516,    2, 0x08,   18 /* Private */,
      20,    1,  519,    2, 0x08,   20 /* Private */,
      21,    0,  522,    2, 0x08,   22 /* Private */,
      22,    0,  523,    2, 0x08,   23 /* Private */,
      23,    0,  524,    2, 0x08,   24 /* Private */,
      24,    3,  525,    2, 0x08,   25 /* Private */,
      28,    3,  532,    2, 0x08,   29 /* Private */,
      30,    0,  539,    2, 0x08,   33 /* Private */,
      31,    1,  540,    2, 0x08,   34 /* Private */,
      33,    1,  543,    2, 0x08,   36 /* Private */,
      35,    1,  546,    2, 0x08,   38 /* Private */,
      37,    1,  549,    2, 0x08,   40 /* Private */,
      39,    1,  552,    2, 0x08,   42 /* Private */,
      41,    1,  555,    2, 0x08,   44 /* Private */,
      43,    1,  558,    2, 0x08,   46 /* Private */,
      45,    1,  561,    2, 0x08,   48 /* Private */,
      47,    0,  564,    2, 0x08,   50 /* Private */,
      48,    0,  565,    2, 0x08,   51 /* Private */,
      49,    0,  566,    2, 0x08,   52 /* Private */,
      50,    0,  567,    2, 0x08,   53 /* Private */,
      51,    0,  568,    2, 0x08,   54 /* Private */,
      52,    0,  569,    2, 0x08,   55 /* Private */,
      53,    0,  570,    2, 0x08,   56 /* Private */,
      54,    0,  571,    2, 0x08,   57 /* Private */,
      55,    0,  572,    2, 0x08,   58 /* Private */,
      56,    0,  573,    2, 0x08,   59 /* Private */,
      57,    0,  574,    2, 0x08,   60 /* Private */,
      58,    0,  575,    2, 0x08,   61 /* Private */,
      59,    0,  576,    2, 0x08,   62 /* Private */,
      60,    0,  577,    2, 0x08,   63 /* Private */,
      61,    0,  578,    2, 0x08,   64 /* Private */,
      62,    0,  579,    2, 0x08,   65 /* Private */,
      63,    0,  580,    2, 0x08,   66 /* Private */,
      64,    0,  581,    2, 0x08,   67 /* Private */,
      65,    0,  582,    2, 0x08,   68 /* Private */,
      66,    0,  583,    2, 0x08,   69 /* Private */,
      67,    0,  584,    2, 0x08,   70 /* Private */,
      68,    0,  585,    2, 0x08,   71 /* Private */,
      69,    0,  586,    2, 0x08,   72 /* Private */,
      70,    0,  587,    2, 0x08,   73 /* Private */,
      71,    0,  588,    2, 0x08,   74 /* Private */,
      72,    0,  589,    2, 0x08,   75 /* Private */,
      73,    0,  590,    2, 0x08,   76 /* Private */,
      74,    0,  591,    2, 0x08,   77 /* Private */,
      75,    0,  592,    2, 0x08,   78 /* Private */,
      76,    0,  593,    2, 0x08,   79 /* Private */,
      77,    0,  594,    2, 0x08,   80 /* Private */,
      78,    0,  595,    2, 0x08,   81 /* Private */,
      79,    0,  596,    2, 0x08,   82 /* Private */,
      80,    0,  597,    2, 0x08,   83 /* Private */,
      81,    0,  598,    2, 0x08,   84 /* Private */,
      82,    0,  599,    2, 0x08,   85 /* Private */,
      83,    0,  600,    2, 0x08,   86 /* Private */,
      84,    0,  601,    2, 0x08,   87 /* Private */,
      85,    0,  602,    2, 0x08,   88 /* Private */,
      86,    0,  603,    2, 0x08,   89 /* Private */,
      87,    0,  604,    2, 0x08,   90 /* Private */,
      88,    0,  605,    2, 0x08,   91 /* Private */,
      89,    0,  606,    2, 0x08,   92 /* Private */,
      90,    1,  607,    2, 0x08,   93 /* Private */,
      92,    1,  610,    2, 0x08,   95 /* Private */,
      93,    1,  613,    2, 0x08,   97 /* Private */,
      94,    1,  616,    2, 0x08,   99 /* Private */,
      95,    1,  619,    2, 0x08,  101 /* Private */,
      96,    1,  622,    2, 0x08,  103 /* Private */,
      97,    0,  625,    2, 0x08,  105 /* Private */,
      98,    0,  626,    2, 0x08,  106 /* Private */,
      99,    0,  627,    2, 0x08,  107 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void, QMetaType::QJsonObject,   11,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   25,   26,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,   25,   29,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   32,
    QMetaType::Void, QMetaType::Bool,   34,
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::QString,   40,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   44,
    QMetaType::Void, QMetaType::Int,   46,
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
    QMetaType::Void, QMetaType::Int,   91,
    QMetaType::Void, QMetaType::Bool,   34,
    QMetaType::Void, QMetaType::Int,   91,
    QMetaType::Void, QMetaType::Bool,   34,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void, QMetaType::QString,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AudioManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AudioManager.offsetsAndSizes,
    qt_meta_data_AudioManager,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_AudioManager_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AudioManager, std::true_type>,
        // method 'onDeviceActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProfileActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEffectActionTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'searchDevices'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'applyAudioProfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetAudioSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeviceFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onProfileFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onCardFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'onScanFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onScanError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onDeviceTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onProfileTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onEffectTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onDeviceSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onProfileSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEffectSelectionChanged'
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
        // method 'onRefreshTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVolumeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onMuteToggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onEffectToggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onProfileChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onDeviceChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSampleRateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onBufferSizeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onLatencyChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showDeviceProperties'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showProfileEditor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showEffectEditor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showEasyEffectsPresets'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPipeWireSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showAdvancedSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'importProfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportProfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createProfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteProfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installEasyEffects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startEasyEffects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopEasyEffects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enableEasyEffectsAutostart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disableEasyEffectsAutostart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadEasyEffectsPreset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveEasyEffectsPreset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteEasyEffectsPreset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetEasyEffectsPreset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'installPipeWire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startPipeWire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopPipeWire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restartPipeWire'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'enablePipeWireAutostart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disablePipeWireAutostart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPipeWireGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'optimizeForLatency'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'optimizeForQuality'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'optimizeForPowerSaving'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'testAudioDevices'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'calibrateAudioLevels'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showAudioAnalyzer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showEqualizerSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showCompressorSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showReverbSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDelaySettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showChorusSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDistortionSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showFilterSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showNoiseGateSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showLimiterSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showMasteringSettings'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupSimplifiedAudioControls'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setMasterVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setMasterMute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setInputVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setInputMute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setSampleRate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setBufferSize'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'editPipeWireConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'reloadPipeWireConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'launchEasyEffects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AudioManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AudioManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onDeviceActionTriggered(); break;
        case 1: _t->onProfileActionTriggered(); break;
        case 2: _t->onEffectActionTriggered(); break;
        case 3: _t->searchDevices(); break;
        case 4: _t->applyAudioProfile(); break;
        case 5: _t->resetAudioSettings(); break;
        case 6: _t->onDeviceFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 7: _t->onProfileFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 8: _t->onCardFound((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 9: _t->onScanFinished(); break;
        case 10: _t->onScanError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->onDeviceTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 12: _t->onProfileTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 13: _t->onEffectTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 14: _t->onDeviceSelectionChanged(); break;
        case 15: _t->onProfileSelectionChanged(); break;
        case 16: _t->onEffectSelectionChanged(); break;
        case 17: _t->onProgressUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 18: _t->onTaskCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 19: _t->onRefreshTimer(); break;
        case 20: _t->onVolumeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->onMuteToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->onEffectToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: _t->onProfileChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->onDeviceChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->onSampleRateChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->onBufferSizeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->onLatencyChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->showDeviceProperties(); break;
        case 29: _t->showProfileEditor(); break;
        case 30: _t->showEffectEditor(); break;
        case 31: _t->showEasyEffectsPresets(); break;
        case 32: _t->showPipeWireSettings(); break;
        case 33: _t->showAdvancedSettings(); break;
        case 34: _t->importProfile(); break;
        case 35: _t->exportProfile(); break;
        case 36: _t->createProfile(); break;
        case 37: _t->deleteProfile(); break;
        case 38: _t->installEasyEffects(); break;
        case 39: _t->startEasyEffects(); break;
        case 40: _t->stopEasyEffects(); break;
        case 41: _t->enableEasyEffectsAutostart(); break;
        case 42: _t->disableEasyEffectsAutostart(); break;
        case 43: _t->loadEasyEffectsPreset(); break;
        case 44: _t->saveEasyEffectsPreset(); break;
        case 45: _t->deleteEasyEffectsPreset(); break;
        case 46: _t->resetEasyEffectsPreset(); break;
        case 47: _t->installPipeWire(); break;
        case 48: _t->startPipeWire(); break;
        case 49: _t->stopPipeWire(); break;
        case 50: _t->restartPipeWire(); break;
        case 51: _t->enablePipeWireAutostart(); break;
        case 52: _t->disablePipeWireAutostart(); break;
        case 53: _t->showPipeWireGraph(); break;
        case 54: _t->optimizeForLatency(); break;
        case 55: _t->optimizeForQuality(); break;
        case 56: _t->optimizeForPowerSaving(); break;
        case 57: _t->testAudioDevices(); break;
        case 58: _t->calibrateAudioLevels(); break;
        case 59: _t->showAudioAnalyzer(); break;
        case 60: _t->showEqualizerSettings(); break;
        case 61: _t->showCompressorSettings(); break;
        case 62: _t->showReverbSettings(); break;
        case 63: _t->showDelaySettings(); break;
        case 64: _t->showChorusSettings(); break;
        case 65: _t->showDistortionSettings(); break;
        case 66: _t->showFilterSettings(); break;
        case 67: _t->showNoiseGateSettings(); break;
        case 68: _t->showLimiterSettings(); break;
        case 69: _t->showMasteringSettings(); break;
        case 70: _t->setupSimplifiedAudioControls(); break;
        case 71: _t->setMasterVolume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 72: _t->setMasterMute((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 73: _t->setInputVolume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 74: _t->setInputMute((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 75: _t->setSampleRate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 76: _t->setBufferSize((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 77: _t->editPipeWireConfig(); break;
        case 78: _t->reloadPipeWireConfig(); break;
        case 79: _t->launchEasyEffects(); break;
        default: ;
        }
    }
}

const QMetaObject *AudioManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioManager.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AudioManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 80)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 80;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 80)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 80;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
