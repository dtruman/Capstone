/****************************************************************************
** Meta object code from reading C++ file 'ObjectHandle.h'
**
** Created: Tue Nov 25 01:52:02 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ObjectHandle.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectHandle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ObjectHandle[] = {

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
      14,   13,   13,   13, 0x08,
      27,   13,   13,   13, 0x08,
      39,   13,   13,   13, 0x08,
      50,   13,   13,   13, 0x08,
      62,   13,   13,   13, 0x08,
      72,   13,   13,   13, 0x08,
      82,   13,   13,   13, 0x08,
     100,   13,   13,   13, 0x08,
     115,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ObjectHandle[] = {
    "ObjectHandle\0\0addTexture()\0addPlayer()\0"
    "addInput()\0addScript()\0addPath()\0"
    "addNode()\0addIntelligence()\0objectDelete()\0"
    "objectClone()\0"
};

void ObjectHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ObjectHandle *_t = static_cast<ObjectHandle *>(_o);
        switch (_id) {
        case 0: _t->addTexture(); break;
        case 1: _t->addPlayer(); break;
        case 2: _t->addInput(); break;
        case 3: _t->addScript(); break;
        case 4: _t->addPath(); break;
        case 5: _t->addNode(); break;
        case 6: _t->addIntelligence(); break;
        case 7: _t->objectDelete(); break;
        case 8: _t->objectClone(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ObjectHandle::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ObjectHandle::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_ObjectHandle,
      qt_meta_data_ObjectHandle, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ObjectHandle::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ObjectHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ObjectHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectHandle))
        return static_cast<void*>(const_cast< ObjectHandle*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int ObjectHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
