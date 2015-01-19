/****************************************************************************
** Meta object code from reading C++ file 'MeWidg.h'
**
** Created: Sun Nov 23 22:01:35 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MeWidg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeWidg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MeWidg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      19,    7,    7,    7, 0x08,
      31,    7,    7,    7, 0x08,
      44,    7,    7,    7, 0x08,
      57,    7,    7,    7, 0x08,
      78,    7,    7,    7, 0x08,
      90,    7,    7,    7, 0x08,
     103,    7,    7,    7, 0x08,
     115,    7,    7,    7, 0x08,
     126,    7,    7,    7, 0x08,
     140,    7,    7,    7, 0x08,
     154,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MeWidg[] = {
    "MeWidg\0\0myUpdate()\0loadModel()\0"
    "loadALight()\0loadDLight()\0"
    "loadInstancedModel()\0saveScene()\0"
    "exportGame()\0loadScene()\0addScene()\0"
    "selectScene()\0saveProject()\0loadProject()\0"
};

void MeWidg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MeWidg *_t = static_cast<MeWidg *>(_o);
        switch (_id) {
        case 0: _t->myUpdate(); break;
        case 1: _t->loadModel(); break;
        case 2: _t->loadALight(); break;
        case 3: _t->loadDLight(); break;
        case 4: _t->loadInstancedModel(); break;
        case 5: _t->saveScene(); break;
        case 6: _t->exportGame(); break;
        case 7: _t->loadScene(); break;
        case 8: _t->addScene(); break;
        case 9: _t->selectScene(); break;
        case 10: _t->saveProject(); break;
        case 11: _t->loadProject(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MeWidg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MeWidg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MeWidg,
      qt_meta_data_MeWidg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MeWidg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MeWidg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MeWidg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeWidg))
        return static_cast<void*>(const_cast< MeWidg*>(this));
    return QWidget::qt_metacast(_clname);
}

int MeWidg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
