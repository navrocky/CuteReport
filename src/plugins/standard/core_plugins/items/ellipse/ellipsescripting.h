/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012 by Alexander Mikhalov                              *
 *   alexander.mikhalov@gmail.com                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>

#include "reportcore.h"
#include "types.h"
#include "ellipse.h"
#include "baseiteminterface.h"
#include "iteminterface.h"
using namespace CuteReport;

static QScriptValue qtscript_Qt_throw_ambiguity_error_helper(
        QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("Qt::%0(): could not find a function match; candidates are:\n%1")
                               .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}


static QScriptValue qtscript_create_enum_class_helper(
        QScriptEngine *engine,
        QScriptEngine::FunctionSignature construct,
        QScriptEngine::FunctionSignature valueOf,
        QScriptEngine::FunctionSignature toString)
{
    QScriptValue proto = engine->newObject();
    proto.setProperty(QString::fromLatin1("valueOf"),
                      engine->newFunction(valueOf), QScriptValue::SkipInEnumeration);

    proto.setProperty(QString::fromLatin1("toString"),
                      engine->newFunction(toString), QScriptValue::SkipInEnumeration);
    return engine->newFunction(construct, proto, 1);
}


//static QScriptValue qtscript_create_flags_class_helper(
//        QScriptEngine *engine,
//        QScriptEngine::FunctionSignature construct,
//        QScriptEngine::FunctionSignature valueOf,
//        QScriptEngine::FunctionSignature toString,
//        QScriptEngine::FunctionSignature equals)
//{
//    QScriptValue proto = engine->newObject();
//    proto.setProperty(QString::fromLatin1("valueOf"),
//                      engine->newFunction(valueOf), QScriptValue::SkipInEnumeration);
//    proto.setProperty(QString::fromLatin1("toString"),
//                      engine->newFunction(toString), QScriptValue::SkipInEnumeration);
//    proto.setProperty(QString::fromLatin1("equals"),
//                      engine->newFunction(equals), QScriptValue::SkipInEnumeration);
//    return engine->newFunction(construct, proto);
//}


struct qtscript_Qt_metaObject_helper : private QObject
{
    static const QMetaObject *get()
    { return &static_cast<qtscript_Qt_metaObject_helper*>(0)->staticQtMetaObject; }
};


static const QMetaObject *qtscript_Qt_metaObject()
{
    return qtscript_Qt_metaObject_helper::get();
}

/** ======================= Frame =========================== */

static const BaseItemInterface::Frame qtscript_Ellipse_Frame_values[] = {
    BaseItemInterface::DrawLeft,
    BaseItemInterface::DrawRight,
    BaseItemInterface::DrawTop,
    BaseItemInterface::DrawBottom
};

static const char * const qtscript_Ellipse_Frame_keys[] = {
    "DrawLeft",
    "DrawRight",
    "DrawTop",
    "DrawBottom"
};

static int Ellipse_Frame_num = 4;


static QString qtscript_Ellipse_Frame_toStringHelper(BaseItemInterface::Frame value)
{
    const QMetaObject meta = BaseItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("Frame");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Ellipse_Frame_toScriptValue(QScriptEngine *engine, const BaseItemInterface::Frame &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Ellipse"));
    return clazz.property(qtscript_Ellipse_Frame_toStringHelper(value));
}


static void qtscript_Ellipse_Frame_fromScriptValue(const QScriptValue &value, BaseItemInterface::Frame &out)
{
    out = qvariant_cast<BaseItemInterface::Frame>(value.toVariant());
}


static QScriptValue qtscript_construct_Ellipse_Frame(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("Frame");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BaseItemInterface::Frame>(arg));
    return context->throwError(QString::fromLatin1("Frame(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_Ellipse_Frame_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::Frame value = qscriptvalue_cast<BaseItemInterface::Frame>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Ellipse_Frame_toString(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::Frame value = qscriptvalue_cast<BaseItemInterface::Frame>(context->thisObject());
    return QScriptValue(engine, qtscript_Ellipse_Frame_toStringHelper(value));
}


static QScriptValue qtscript_create_Ellipse_Frame_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Ellipse_Frame,
                            qtscript_Ellipse_Frame_valueOf, qtscript_Ellipse_Frame_toString);

    qScriptRegisterMetaType<BaseItemInterface::Frame>(engine, qtscript_Ellipse_Frame_toScriptValue,
                                                   qtscript_Ellipse_Frame_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Ellipse_Frame_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Ellipse_Frame_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Ellipse_Frame_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Ellipse_Frame_toString(BaseItemInterface::Frame value)
{
    int idx = -1;
    for (int i = 0; i < Ellipse_Frame_num; ++i) {
        if (qtscript_Ellipse_Frame_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Ellipse_Frame_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Ellipse_Frame_keys[idx]);
}


BaseItemInterface::Frame Ellipse_Frame_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Ellipse_Frame_num; ++i) {
        QString str(qtscript_Ellipse_Frame_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Ellipse_Frame_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BaseItemInterface::Frame(qtscript_Ellipse_Frame_values[idx]);
}


/** ======================= ResizeFlags =========================== */

static const BaseItemInterface::ResizeFlags qtscript_Ellipse_ResizeFlags_values[] = {
    BaseItemInterface::Fixed,
    BaseItemInterface::ResizeLeft,
    BaseItemInterface::ResizeRight,
    BaseItemInterface::ResizeTop,
    BaseItemInterface::ResizeBottom,
    BaseItemInterface::FixedPos
};

static const char * const qtscript_Ellipse_ResizeFlags_keys[] = {
    "Fixed",
    "ResizeLeft",
    "ResizeRight",
    "ResizeTop",
    "ResizeBottom",
    "FixedPos"
};

static int Ellipse_ResizeFlags_num = 6;


static QString qtscript_Ellipse_ResizeFlags_toStringHelper(BaseItemInterface::ResizeFlags value)
{
    const QMetaObject meta = BaseItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("ResizeFlags");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Ellipse_ResizeFlags_toScriptValue(QScriptEngine *engine, const BaseItemInterface::ResizeFlags &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Ellipse"));
    return clazz.property(qtscript_Ellipse_ResizeFlags_toStringHelper(value));
}


static void qtscript_Ellipse_ResizeFlags_fromScriptValue(const QScriptValue &value, BaseItemInterface::ResizeFlags &out)
{
    out = qvariant_cast<BaseItemInterface::ResizeFlags>(value.toVariant());
}


static QScriptValue qtscript_construct_Ellipse_ResizeFlags(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("ResizeFlags");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BaseItemInterface::ResizeFlags>(arg));
    return context->throwError(QString::fromLatin1("ResizeFlags(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_Ellipse_ResizeFlags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::ResizeFlags value = qscriptvalue_cast<BaseItemInterface::ResizeFlags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Ellipse_ResizeFlags_toString(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::ResizeFlags value = qscriptvalue_cast<BaseItemInterface::ResizeFlags>(context->thisObject());
    return QScriptValue(engine, qtscript_Ellipse_ResizeFlags_toStringHelper(value));
}


static QScriptValue qtscript_create_Ellipse_ResizeFlags_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Ellipse_ResizeFlags,
                            qtscript_Ellipse_ResizeFlags_valueOf, qtscript_Ellipse_ResizeFlags_toString);

    qScriptRegisterMetaType<BaseItemInterface::ResizeFlags>(engine, qtscript_Ellipse_ResizeFlags_toScriptValue,
                                                   qtscript_Ellipse_ResizeFlags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Ellipse_ResizeFlags_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Ellipse_ResizeFlags_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Ellipse_ResizeFlags_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Ellipse_ResizeFlags_toString(BaseItemInterface::ResizeFlags value)
{
    int idx = -1;
    for (int i = 0; i < Ellipse_ResizeFlags_num; ++i) {
        if (qtscript_Ellipse_ResizeFlags_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Ellipse_ResizeFlags_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Ellipse_ResizeFlags_keys[idx]);
}


BaseItemInterface::ResizeFlags Ellipse_ResizeFlags_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Ellipse_ResizeFlags_num; ++i) {
        QString str(qtscript_Ellipse_ResizeFlags_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Ellipse_ResizeFlags_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BaseItemInterface::ResizeFlags(qtscript_Ellipse_ResizeFlags_values[idx]);
}


/** ======================= ShiftMode =========================== */

static const ItemInterface::ShiftMode qtscript_Ellipse_ShiftMode_values[] = {
    ItemInterface::AlwaysShift,
    ItemInterface::DontShift,
    ItemInterface::ShiftWhenOverlapped
};

static const char * const qtscript_Ellipse_ShiftMode_keys[] = {
    "AlwaysShift",
    "DontShift",
    "ShiftWhenOverlapped"
};

static int Ellipse_ShiftMode_num = 3;


static QString qtscript_Ellipse_ShiftMode_toStringHelper(ItemInterface::ShiftMode value)
{
    const QMetaObject meta = ItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("ShiftMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Ellipse_ShiftMode_toScriptValue(QScriptEngine *engine, const ItemInterface::ShiftMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Ellipse"));
    return clazz.property(qtscript_Ellipse_ShiftMode_toStringHelper(value));
}


static void qtscript_Ellipse_ShiftMode_fromScriptValue(const QScriptValue &value, ItemInterface::ShiftMode &out)
{
    out = qvariant_cast<ItemInterface::ShiftMode>(value.toVariant());
}


static QScriptValue qtscript_construct_Ellipse_ShiftMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("ShiftMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<ItemInterface::ShiftMode>(arg));
    return context->throwError(QString::fromLatin1("ShiftMode(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_Ellipse_ShiftMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    ItemInterface::ShiftMode value = qscriptvalue_cast<ItemInterface::ShiftMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Ellipse_ShiftMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    ItemInterface::ShiftMode value = qscriptvalue_cast<ItemInterface::ShiftMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Ellipse_ShiftMode_toStringHelper(value));
}


static QScriptValue qtscript_create_Ellipse_ShiftMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Ellipse_ShiftMode,
                            qtscript_Ellipse_ShiftMode_valueOf, qtscript_Ellipse_ShiftMode_toString);

    qScriptRegisterMetaType<ItemInterface::ShiftMode>(engine, qtscript_Ellipse_ShiftMode_toScriptValue,
                                                   qtscript_Ellipse_ShiftMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Ellipse_ShiftMode_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Ellipse_ShiftMode_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Ellipse_ShiftMode_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Ellipse_ShiftMode_toString(ItemInterface::ShiftMode value)
{
    int idx = -1;
    for (int i = 0; i < Ellipse_ShiftMode_num; ++i) {
        if (qtscript_Ellipse_ShiftMode_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "ItemInterface", "Ellipse_ShiftMode_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Ellipse_ShiftMode_keys[idx]);
}


ItemInterface::ShiftMode Ellipse_ShiftMode_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Ellipse_ShiftMode_num; ++i) {
        QString str(qtscript_Ellipse_ShiftMode_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "ItemInterface", "Ellipse_ShiftMode_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return ItemInterface::ShiftMode(qtscript_Ellipse_ShiftMode_values[idx]);
}

/** ============================================================================================================= */


static const char * const qtscript_EllipseItem_function_names[] = {
    "Ellipse"
    , "toString"
};


static QScriptValue qtscript_EllipseItem_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
        case 0:
            return context->throwError(QScriptContext::TypeError,
                                       QString::fromLatin1("EllipseItem cannot be constructed"));
            break;

        default:
            Q_ASSERT(false);
    }
    return qtscript_Qt_throw_ambiguity_error_helper(context,  qtscript_EllipseItem_function_names[_id],  "" /*qtscript_Qt_function_signatures[_id]*/);
}


QScriptValue qtscript_create_EllipseItem_class(QScriptEngine *engine)
{
    QScriptValue proto = QScriptValue();
    QScriptValue ctor = engine->newFunction(qtscript_EllipseItem_static_call, proto, 0);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("Frame"), qtscript_create_Ellipse_Frame_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ResizeFlags"), qtscript_create_Ellipse_ResizeFlags_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ShiftMode"), qtscript_create_Ellipse_ShiftMode_class(engine, ctor));

    return ctor;
}


void registerEllipseItemScriptClass(QScriptEngine *engine)
{
    QScriptValue extensionObject = engine->globalObject();
    extensionObject.setProperty("Ellipse", qtscript_create_EllipseItem_class(engine), QScriptValue::SkipInEnumeration);
}

