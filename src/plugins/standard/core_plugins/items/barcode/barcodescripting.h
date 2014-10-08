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
#include "barcode.h"
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

static const BaseItemInterface::Frame qtscript_Barcode_Frame_values[] = {
    BaseItemInterface::DrawLeft,
    BaseItemInterface::DrawRight,
    BaseItemInterface::DrawTop,
    BaseItemInterface::DrawBottom
};

static const char * const qtscript_Barcode_Frame_keys[] = {
    "DrawLeft",
    "DrawRight",
    "DrawTop",
    "DrawBottom"
};

static int Barcode_Frame_num = 4;


static QString qtscript_Barcode_Frame_toStringHelper(BaseItemInterface::Frame value)
{
    const QMetaObject meta = BaseItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("Frame");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Barcode_Frame_toScriptValue(QScriptEngine *engine, const BaseItemInterface::Frame &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_Barcode_Frame_toStringHelper(value));
}


static void qtscript_Barcode_Frame_fromScriptValue(const QScriptValue &value, BaseItemInterface::Frame &out)
{
    out = qvariant_cast<BaseItemInterface::Frame>(value.toVariant());
}


static QScriptValue qtscript_construct_Barcode_Frame(QScriptContext *context, QScriptEngine *engine)
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


static QScriptValue qtscript_Barcode_Frame_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::Frame value = qscriptvalue_cast<BaseItemInterface::Frame>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Barcode_Frame_toString(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::Frame value = qscriptvalue_cast<BaseItemInterface::Frame>(context->thisObject());
    return QScriptValue(engine, qtscript_Barcode_Frame_toStringHelper(value));
}


static QScriptValue qtscript_create_Barcode_Frame_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Barcode_Frame,
                            qtscript_Barcode_Frame_valueOf, qtscript_Barcode_Frame_toString);

    qScriptRegisterMetaType<BaseItemInterface::Frame>(engine, qtscript_Barcode_Frame_toScriptValue,
                                                   qtscript_Barcode_Frame_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Barcode_Frame_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Barcode_Frame_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Barcode_Frame_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Barcode_Frame_toString(BaseItemInterface::Frame value)
{
    int idx = -1;
    for (int i = 0; i < Barcode_Frame_num; ++i) {
        if (qtscript_Barcode_Frame_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Barcode_Frame_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Barcode_Frame_keys[idx]);
}


BaseItemInterface::Frame Barcode_Frame_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Barcode_Frame_num; ++i) {
        QString str(qtscript_Barcode_Frame_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Barcode_Frame_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BaseItemInterface::Frame(qtscript_Barcode_Frame_values[idx]);
}


/** ======================= ResizeFlags =========================== */

static const BaseItemInterface::ResizeFlags qtscript_Barcode_ResizeFlags_values[] = {
    BaseItemInterface::Fixed,
    BaseItemInterface::ResizeLeft,
    BaseItemInterface::ResizeRight,
    BaseItemInterface::ResizeTop,
    BaseItemInterface::ResizeBottom,
    BaseItemInterface::FixedPos
};

static const char * const qtscript_Barcode_ResizeFlags_keys[] = {
    "Fixed",
    "ResizeLeft",
    "ResizeRight",
    "ResizeTop",
    "ResizeBottom",
    "FixedPos"
};

static int Barcode_ResizeFlags_num = 6;


static QString qtscript_Barcode_ResizeFlags_toStringHelper(BaseItemInterface::ResizeFlags value)
{
    const QMetaObject meta = BaseItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("ResizeFlags");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Barcode_ResizeFlags_toScriptValue(QScriptEngine *engine, const BaseItemInterface::ResizeFlags &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_Barcode_ResizeFlags_toStringHelper(value));
}


static void qtscript_Barcode_ResizeFlags_fromScriptValue(const QScriptValue &value, BaseItemInterface::ResizeFlags &out)
{
    out = qvariant_cast<BaseItemInterface::ResizeFlags>(value.toVariant());
}


static QScriptValue qtscript_construct_Barcode_ResizeFlags(QScriptContext *context, QScriptEngine *engine)
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


static QScriptValue qtscript_Barcode_ResizeFlags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::ResizeFlags value = qscriptvalue_cast<BaseItemInterface::ResizeFlags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Barcode_ResizeFlags_toString(QScriptContext *context, QScriptEngine *engine)
{
    BaseItemInterface::ResizeFlags value = qscriptvalue_cast<BaseItemInterface::ResizeFlags>(context->thisObject());
    return QScriptValue(engine, qtscript_Barcode_ResizeFlags_toStringHelper(value));
}


static QScriptValue qtscript_create_Barcode_ResizeFlags_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Barcode_ResizeFlags,
                            qtscript_Barcode_ResizeFlags_valueOf, qtscript_Barcode_ResizeFlags_toString);

    qScriptRegisterMetaType<BaseItemInterface::ResizeFlags>(engine, qtscript_Barcode_ResizeFlags_toScriptValue,
                                                   qtscript_Barcode_ResizeFlags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Barcode_ResizeFlags_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Barcode_ResizeFlags_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Barcode_ResizeFlags_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Barcode_ResizeFlags_toString(BaseItemInterface::ResizeFlags value)
{
    int idx = -1;
    for (int i = 0; i < Barcode_ResizeFlags_num; ++i) {
        if (qtscript_Barcode_ResizeFlags_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Barcode_ResizeFlags_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Barcode_ResizeFlags_keys[idx]);
}


BaseItemInterface::ResizeFlags Barcode_ResizeFlags_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Barcode_ResizeFlags_num; ++i) {
        QString str(qtscript_Barcode_ResizeFlags_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BaseItemInterface", "Barcode_ResizeFlags_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BaseItemInterface::ResizeFlags(qtscript_Barcode_ResizeFlags_values[idx]);
}


/** ======================= ShiftMode =========================== */

static const ItemInterface::ShiftMode qtscript_Barcode_ShiftMode_values[] = {
    ItemInterface::AlwaysShift,
    ItemInterface::DontShift,
    ItemInterface::ShiftWhenOverlapped
};

static const char * const qtscript_Barcode_ShiftMode_keys[] = {
    "AlwaysShift",
    "DontShift",
    "ShiftWhenOverlapped"
};

static int Barcode_ShiftMode_num = 3;


static QString qtscript_Barcode_ShiftMode_toStringHelper(ItemInterface::ShiftMode value)
{
    const QMetaObject meta = ItemInterface::staticMetaObject;
    int idx = meta.indexOfEnumerator("ShiftMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_Barcode_ShiftMode_toScriptValue(QScriptEngine *engine, const ItemInterface::ShiftMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_Barcode_ShiftMode_toStringHelper(value));
}


static void qtscript_Barcode_ShiftMode_fromScriptValue(const QScriptValue &value, ItemInterface::ShiftMode &out)
{
    out = qvariant_cast<ItemInterface::ShiftMode>(value.toVariant());
}


static QScriptValue qtscript_construct_Barcode_ShiftMode(QScriptContext *context, QScriptEngine *engine)
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


static QScriptValue qtscript_Barcode_ShiftMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    ItemInterface::ShiftMode value = qscriptvalue_cast<ItemInterface::ShiftMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_Barcode_ShiftMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    ItemInterface::ShiftMode value = qscriptvalue_cast<ItemInterface::ShiftMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Barcode_ShiftMode_toStringHelper(value));
}


static QScriptValue qtscript_create_Barcode_ShiftMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_Barcode_ShiftMode,
                            qtscript_Barcode_ShiftMode_valueOf, qtscript_Barcode_ShiftMode_toString);

    qScriptRegisterMetaType<ItemInterface::ShiftMode>(engine, qtscript_Barcode_ShiftMode_toScriptValue,
                                                   qtscript_Barcode_ShiftMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < Barcode_ShiftMode_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Barcode_ShiftMode_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_Barcode_ShiftMode_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString Barcode_ShiftMode_toString(ItemInterface::ShiftMode value)
{
    int idx = -1;
    for (int i = 0; i < Barcode_ShiftMode_num; ++i) {
        if (qtscript_Barcode_ShiftMode_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "ItemInterface", "Barcode_ShiftMode_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_Barcode_ShiftMode_keys[idx]);
}


ItemInterface::ShiftMode Barcode_ShiftMode_fromString(const QString &value, bool *ok)
{
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < Barcode_ShiftMode_num; ++i) {
        QString str(qtscript_Barcode_ShiftMode_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "ItemInterface", "Barcode_ShiftMode_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return ItemInterface::ShiftMode(qtscript_Barcode_ShiftMode_values[idx]);
}


/** ======================= MsiPlessey =========================== */

static const BarcodeItem::MsiPlessey qtscript_BarcodeItem_MsiPlessey_values[] = {
    BarcodeItem::MSI_PLESSEY_,
    BarcodeItem::MSI_10,
    BarcodeItem::MSI_10_10,
    BarcodeItem::MSI_11,
    BarcodeItem::MSI_11_10
};

static const char * const qtscript_BarcodeItem_MsiPlessey_keys[] = {
    "MSI_PLESSEY_",
    "MSI_10",
    "MSI_10_10",
    "MSI_11",
    "MSI_11_10"
};

static int BarcodeItem_MsiPlessey_num = 5;


static QString qtscript_BarcodeItem_MsiPlessey_toStringHelper(BarcodeItem::MsiPlessey value)
{
    const QMetaObject meta = BarcodeItem::staticMetaObject;
    int idx = meta.indexOfEnumerator("MsiPlessey");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_BarcodeItem_MsiPlessey_toScriptValue(QScriptEngine *engine, const BarcodeItem::MsiPlessey &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_BarcodeItem_MsiPlessey_toStringHelper(value));
}


static void qtscript_BarcodeItem_MsiPlessey_fromScriptValue(const QScriptValue &value, BarcodeItem::MsiPlessey &out)
{
    out = qvariant_cast<BarcodeItem::MsiPlessey>(value.toVariant());
}


static QScriptValue qtscript_construct_BarcodeItem_MsiPlessey(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("MsiPlessey");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BarcodeItem::MsiPlessey>(arg));
    return context->throwError(QString::fromLatin1("MsiPlessey(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_BarcodeItem_MsiPlessey_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::MsiPlessey value = qscriptvalue_cast<BarcodeItem::MsiPlessey>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_BarcodeItem_MsiPlessey_toString(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::MsiPlessey value = qscriptvalue_cast<BarcodeItem::MsiPlessey>(context->thisObject());
    return QScriptValue(engine, qtscript_BarcodeItem_MsiPlessey_toStringHelper(value));
}


static QScriptValue qtscript_create_BarcodeItem_MsiPlessey_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_BarcodeItem_MsiPlessey,
                            qtscript_BarcodeItem_MsiPlessey_valueOf, qtscript_BarcodeItem_MsiPlessey_toString);

    qScriptRegisterMetaType<BarcodeItem::MsiPlessey>(engine, qtscript_BarcodeItem_MsiPlessey_toScriptValue,
                                                   qtscript_BarcodeItem_MsiPlessey_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < BarcodeItem_MsiPlessey_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_BarcodeItem_MsiPlessey_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_BarcodeItem_MsiPlessey_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString BarcodeItem_MsiPlessey_toString(BarcodeItem::MsiPlessey value)
{
    int idx = -1;
    for (int i = 0; i < BarcodeItem_MsiPlessey_num; ++i) {
        if (qtscript_BarcodeItem_MsiPlessey_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_MsiPlessey_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_BarcodeItem_MsiPlessey_keys[idx]);
}


BarcodeItem::MsiPlessey BarcodeItem_MsiPlessey_fromString(const QString &value, bool *ok)
{
    if (ok)
        *ok = true;
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < BarcodeItem_MsiPlessey_num; ++i) {
        QString str(qtscript_BarcodeItem_MsiPlessey_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_MsiPlessey_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BarcodeItem::MsiPlessey(qtscript_BarcodeItem_MsiPlessey_values[idx]);
}


QStringList BarcodeItem_MsiPlessey_variants()
{
    QStringList list;
    for (int i = 0; i < BarcodeItem_MsiPlessey_num; ++i) {
        list << QString(qtscript_BarcodeItem_MsiPlessey_keys[i]);
    }
    return list;
}

/** ======================= DrawTextTypes =========================== */

static const BarcodeItem::DrawTextTypes qtscript_BarcodeItem_DrawTextTypes_values[] = {
    BarcodeItem::NO_TEXT,
    BarcodeItem::DRAW_TOP,
    BarcodeItem::DRAW_BOTTOM,
    BarcodeItem::DRAW_ABOVE,
    BarcodeItem::DRAW_BELOW
};

static const char * const qtscript_BarcodeItem_DrawTextTypes_keys[] = {
    "NO_TEXT",
    "DRAW_TOP",
    "DRAW_BOTTOM",
    "DRAW_ABOVE",
    "DRAW_BELOW"
};

static int BarcodeItem_DrawTextTypes_num = 5;


static QString qtscript_BarcodeItem_DrawTextTypes_toStringHelper(BarcodeItem::DrawTextTypes value)
{
    const QMetaObject meta = BarcodeItem::staticMetaObject;
    int idx = meta.indexOfEnumerator("DrawTextTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_BarcodeItem_DrawTextTypes_toScriptValue(QScriptEngine *engine, const BarcodeItem::DrawTextTypes &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_BarcodeItem_DrawTextTypes_toStringHelper(value));
}


static void qtscript_BarcodeItem_DrawTextTypes_fromScriptValue(const QScriptValue &value, BarcodeItem::DrawTextTypes &out)
{
    out = qvariant_cast<BarcodeItem::DrawTextTypes>(value.toVariant());
}


static QScriptValue qtscript_construct_BarcodeItem_DrawTextTypes(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("DrawTextTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BarcodeItem::DrawTextTypes>(arg));
    return context->throwError(QString::fromLatin1("DrawTextTypes(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_BarcodeItem_DrawTextTypes_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::DrawTextTypes value = qscriptvalue_cast<BarcodeItem::DrawTextTypes>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_BarcodeItem_DrawTextTypes_toString(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::DrawTextTypes value = qscriptvalue_cast<BarcodeItem::DrawTextTypes>(context->thisObject());
    return QScriptValue(engine, qtscript_BarcodeItem_DrawTextTypes_toStringHelper(value));
}


static QScriptValue qtscript_create_BarcodeItem_DrawTextTypes_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_BarcodeItem_DrawTextTypes,
                            qtscript_BarcodeItem_DrawTextTypes_valueOf, qtscript_BarcodeItem_DrawTextTypes_toString);

    qScriptRegisterMetaType<BarcodeItem::DrawTextTypes>(engine, qtscript_BarcodeItem_DrawTextTypes_toScriptValue,
                                                   qtscript_BarcodeItem_DrawTextTypes_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < BarcodeItem_DrawTextTypes_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_BarcodeItem_DrawTextTypes_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_BarcodeItem_DrawTextTypes_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString BarcodeItem_DrawTextTypes_toString(BarcodeItem::DrawTextTypes value)
{
    int idx = -1;
    for (int i = 0; i < BarcodeItem_DrawTextTypes_num; ++i) {
        if (qtscript_BarcodeItem_DrawTextTypes_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_DrawTextTypes_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_BarcodeItem_DrawTextTypes_keys[idx]);
}


BarcodeItem::DrawTextTypes BarcodeItem_DrawTextTypes_fromString(const QString &value, bool *ok)
{
    if (ok)
        *ok = true;
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < BarcodeItem_DrawTextTypes_num; ++i) {
        QString str(qtscript_BarcodeItem_DrawTextTypes_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_DrawTextTypes_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BarcodeItem::DrawTextTypes(qtscript_BarcodeItem_DrawTextTypes_values[idx]);
}


QStringList BarcodeItem_DrawTextTypes_variants()
{
    QStringList list;
    for (int i = 0; i < BarcodeItem_DrawTextTypes_num; ++i) {
        list << QString(qtscript_BarcodeItem_DrawTextTypes_keys[i]);
    }
    return list;
}

/** ======================= FrameTypes =========================== */

static const BarcodeItem::FrameTypes qtscript_BarcodeItem_FrameTypes_values[] = {
    BarcodeItem::NO_FRAME,
    BarcodeItem::BarcodeItem_BIND,
    BarcodeItem::BarcodeItem_BOX
};

static const char * const qtscript_BarcodeItem_FrameTypes_keys[] = {
    "NO_FRAME",
    "BarcodeItem_BIND",
    "BarcodeItem_BOX"
};

static int BarcodeItem_FrameTypes_num = 3;


static QString qtscript_BarcodeItem_FrameTypes_toStringHelper(BarcodeItem::FrameTypes value)
{
    const QMetaObject meta = BarcodeItem::staticMetaObject;
    int idx = meta.indexOfEnumerator("FrameTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_BarcodeItem_FrameTypes_toScriptValue(QScriptEngine *engine, const BarcodeItem::FrameTypes &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_BarcodeItem_FrameTypes_toStringHelper(value));
}


static void qtscript_BarcodeItem_FrameTypes_fromScriptValue(const QScriptValue &value, BarcodeItem::FrameTypes &out)
{
    out = qvariant_cast<BarcodeItem::FrameTypes>(value.toVariant());
}


static QScriptValue qtscript_construct_BarcodeItem_FrameTypes(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("FrameTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BarcodeItem::FrameTypes>(arg));
    return context->throwError(QString::fromLatin1("FrameTypes(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_BarcodeItem_FrameTypes_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::FrameTypes value = qscriptvalue_cast<BarcodeItem::FrameTypes>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_BarcodeItem_FrameTypes_toString(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::FrameTypes value = qscriptvalue_cast<BarcodeItem::FrameTypes>(context->thisObject());
    return QScriptValue(engine, qtscript_BarcodeItem_FrameTypes_toStringHelper(value));
}


static QScriptValue qtscript_create_BarcodeItem_FrameTypes_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_BarcodeItem_FrameTypes,
                            qtscript_BarcodeItem_FrameTypes_valueOf, qtscript_BarcodeItem_FrameTypes_toString);

    qScriptRegisterMetaType<BarcodeItem::FrameTypes>(engine, qtscript_BarcodeItem_FrameTypes_toScriptValue,
                                                   qtscript_BarcodeItem_FrameTypes_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < BarcodeItem_FrameTypes_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_BarcodeItem_FrameTypes_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_BarcodeItem_FrameTypes_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString BarcodeItem_FrameTypes_toString(BarcodeItem::FrameTypes value)
{
    int idx = -1;
    for (int i = 0; i < BarcodeItem_FrameTypes_num; ++i) {
        if (qtscript_BarcodeItem_FrameTypes_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_FrameTypes_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_BarcodeItem_FrameTypes_keys[idx]);
}


BarcodeItem::FrameTypes BarcodeItem_FrameTypes_fromString(const QString &value, bool *ok)
{
    if (ok)
        *ok = true;
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < BarcodeItem_FrameTypes_num; ++i) {
        QString str(qtscript_BarcodeItem_FrameTypes_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_FrameTypes_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BarcodeItem::FrameTypes(qtscript_BarcodeItem_FrameTypes_values[idx]);
}


QStringList BarcodeItem_FrameTypes_variants()
{
    QStringList list;
    for (int i = 0; i < BarcodeItem_FrameTypes_num; ++i) {
        list << QString(qtscript_BarcodeItem_FrameTypes_keys[i]);
    }
    return list;
}

/** ======================= PaintTypes =========================== */

static const BarcodeItem::PaintTypes qtscript_BarcodeItem_PaintTypes_values[] = {
    BarcodeItem::IgnoreAspectRatio,
    BarcodeItem::KeepAspectRatio,
    BarcodeItem::CenterBarcodeItem
};

static const char * const qtscript_BarcodeItem_PaintTypes_keys[] = {
    "IgnoreAspectRatio",
    "KeepAspectRatio",
    "CenterBarcodeItem"
};

static int BarcodeItem_PaintTypes_num = 3;


static QString qtscript_BarcodeItem_PaintTypes_toStringHelper(BarcodeItem::PaintTypes value)
{
    const QMetaObject meta = BarcodeItem::staticMetaObject;
    int idx = meta.indexOfEnumerator("PaintTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_BarcodeItem_PaintTypes_toScriptValue(QScriptEngine *engine, const BarcodeItem::PaintTypes &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_BarcodeItem_PaintTypes_toStringHelper(value));
}


static void qtscript_BarcodeItem_PaintTypes_fromScriptValue(const QScriptValue &value, BarcodeItem::PaintTypes &out)
{
    out = qvariant_cast<BarcodeItem::PaintTypes>(value.toVariant());
}


static QScriptValue qtscript_construct_BarcodeItem_PaintTypes(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PaintTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BarcodeItem::PaintTypes>(arg));
    return context->throwError(QString::fromLatin1("PaintTypes(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_BarcodeItem_PaintTypes_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::PaintTypes value = qscriptvalue_cast<BarcodeItem::PaintTypes>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_BarcodeItem_PaintTypes_toString(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::PaintTypes value = qscriptvalue_cast<BarcodeItem::PaintTypes>(context->thisObject());
    return QScriptValue(engine, qtscript_BarcodeItem_PaintTypes_toStringHelper(value));
}


static QScriptValue qtscript_create_BarcodeItem_PaintTypes_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_BarcodeItem_PaintTypes,
                            qtscript_BarcodeItem_PaintTypes_valueOf, qtscript_BarcodeItem_PaintTypes_toString);

    qScriptRegisterMetaType<BarcodeItem::PaintTypes>(engine, qtscript_BarcodeItem_PaintTypes_toScriptValue,
                                                   qtscript_BarcodeItem_PaintTypes_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < BarcodeItem_PaintTypes_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_BarcodeItem_PaintTypes_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_BarcodeItem_PaintTypes_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString BarcodeItem_PaintTypes_toString(BarcodeItem::PaintTypes value)
{
    int idx = -1;
    for (int i = 0; i < BarcodeItem_PaintTypes_num; ++i) {
        if (qtscript_BarcodeItem_PaintTypes_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_PaintTypes_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_BarcodeItem_PaintTypes_keys[idx]);
}


BarcodeItem::PaintTypes BarcodeItem_PaintTypes_fromString(const QString &value, bool *ok)
{
    if (ok)
        *ok = true;
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < BarcodeItem_PaintTypes_num; ++i) {
        QString str(qtscript_BarcodeItem_PaintTypes_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_PaintTypes_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BarcodeItem::PaintTypes(qtscript_BarcodeItem_PaintTypes_values[idx]);
}


QStringList BarcodeItem_PaintTypes_variants()
{
    QStringList list;
    for (int i = 0; i < BarcodeItem_PaintTypes_num; ++i) {
        list << QString(qtscript_BarcodeItem_PaintTypes_keys[i]);
    }
    return list;
}

/** ======================= BarcodeTypes =========================== */

static const BarcodeItem::BarcodeTypes qtscript_BarcodeItem_BarcodeTypes_values[] = {
    BarcodeItem::CODE11,
    BarcodeItem::C25MATRIX,
    BarcodeItem::C25INTER,
    BarcodeItem::C25IATA,
    BarcodeItem::C25LOGIC,
    BarcodeItem::C25IND,
    BarcodeItem::CODE39,
    BarcodeItem::EXCODE39,
    BarcodeItem::EANX,
    BarcodeItem::EAN128,
    BarcodeItem::CODABAR,
    BarcodeItem::CODE128,
    BarcodeItem::DPLEIT,
    BarcodeItem::DPIDENT,
    BarcodeItem::CODE16K,
    BarcodeItem::CODE93,
    BarcodeItem::FLAT,
    BarcodeItem::RSS14,
    BarcodeItem::RSS_LTD,
    BarcodeItem::RSS_EXP,
    BarcodeItem::TELEPEN,
    BarcodeItem::UPCA,
    BarcodeItem::UPCE,
    BarcodeItem::POSTNET,
    BarcodeItem::MSI_PLESSEY,
    BarcodeItem::FIM,
    BarcodeItem::LOGMARS,
    BarcodeItem::PHARMA,
    BarcodeItem::PZN,
    BarcodeItem::PHARMA_TWO,
    BarcodeItem::PDF417,
    BarcodeItem::PDF417TRUNC,
    BarcodeItem::MAXICODE,
    BarcodeItem::QRCODE,
    BarcodeItem::CODE128B,
    BarcodeItem::AUSPOST,
    BarcodeItem::AUSREPLY,
    BarcodeItem::AUSROUTE,
    BarcodeItem::AUSREDIRECT,
    BarcodeItem::ISBNX,
    BarcodeItem::RM4SCC,
    BarcodeItem::DATAMATRIX,
    BarcodeItem::ITF14,
    BarcodeItem::CODABLOCKF,
    BarcodeItem::NVE18,
    BarcodeItem::KOREAPOST,
    BarcodeItem::RSS14STACK,
    BarcodeItem::RSS14STACK_OMNI,
    BarcodeItem::RSS_EXPSTACK,
    BarcodeItem::PLANET,
    BarcodeItem::MICROPDF417,
    BarcodeItem::ONECODE,
    BarcodeItem::PLESSEY,
    BarcodeItem::KIX,
    BarcodeItem::AZTEC,
    BarcodeItem::DAFT,
    BarcodeItem::ITALYPOST,
    BarcodeItem::DPD,
    BarcodeItem::MICROQR,
    BarcodeItem::TELEPEN_NUM,
    BarcodeItem::CODE32,
    BarcodeItem::EANX_CC,
    BarcodeItem::EAN128_CC,
    BarcodeItem::RSS14_CC,
    BarcodeItem::RSS_LTD_CC,
    BarcodeItem::RSS_EXP_CC,
    BarcodeItem::UPCA_CC,
    BarcodeItem::UPCE_CC,
    BarcodeItem::RSS14STACK_CC,
    BarcodeItem::RSS14_OMNI_CC,
    BarcodeItem::RSS_EXPSTACK_CC
};

static const char * const qtscript_BarcodeItem_BarcodeTypes_keys[] = {
    "CODE11",
    "C25MATRIX",
    "C25INTER",
    "C25IATA",
    "C25LOGIC",
    "C25IND",
    "CODE39",
    "EXCODE39",
    "EANX",
    "EAN128",
    "CODABAR",
    "CODE128",
    "DPLEIT",
    "DPIDENT",
    "CODE16K",
    "CODE93",
    "FLAT",
    "RSS14",
    "RSS_LTD",
    "RSS_EXP",
    "TELEPEN",
    "UPCA",
    "UPCE",
    "POSTNET",
    "MSI_PLESSEY",
    "FIM",
    "LOGMARS",
    "PHARMA",
    "PZN",
    "PHARMA_TWO",
    "PDF417",
    "PDF417TRUNC",
    "MAXICODE",
    "QRCODE",
    "CODE128B",
    "AUSPOST",
    "AUSREPLY",
    "AUSROUTE",
    "AUSREDIRECT",
    "ISBNX",
    "RM4SCC",
    "DATAMATRIX",
    "ITF14",
    "CODABLOCKF",
    "NVE18",
    "KOREAPOST",
    "RSS14STACK",
    "RSS14STACK_OMNI",
    "RSS_EXPSTACK",
    "PLANET",
    "MICROPDF417",
    "ONECODE",
    "PLESSEY",
    "KIX",
    "AZTEC",
    "DAFT",
    "ITALYPOST",
    "DPD",
    "MICROQR",
    "TELEPEN_NUM",
    "CODE32",
    "EANX_CC",
    "EAN128_CC",
    "RSS14_CC",
    "RSS_LTD_CC",
    "RSS_EXP_CC",
    "UPCA_CC",
    "UPCE_CC",
    "RSS14STACK_CC",
    "RSS14_OMNI_CC",
    "RSS_EXPSTACK_CC"
};

static int BarcodeItem_BarcodeTypes_num = 71;


static QString qtscript_BarcodeItem_BarcodeTypes_toStringHelper(BarcodeItem::BarcodeTypes value)
{
    const QMetaObject meta = BarcodeItem::staticMetaObject;
    int idx = meta.indexOfEnumerator("BarcodeTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta.enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}


static QScriptValue qtscript_BarcodeItem_BarcodeTypes_toScriptValue(QScriptEngine *engine, const BarcodeItem::BarcodeTypes &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Barcode"));
    return clazz.property(qtscript_BarcodeItem_BarcodeTypes_toStringHelper(value));
}


static void qtscript_BarcodeItem_BarcodeTypes_fromScriptValue(const QScriptValue &value, BarcodeItem::BarcodeTypes &out)
{
    out = qvariant_cast<BarcodeItem::BarcodeTypes>(value.toVariant());
}


static QScriptValue qtscript_construct_BarcodeItem_BarcodeTypes(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("BarcodeTypes");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<BarcodeItem::BarcodeTypes>(arg));
    return context->throwError(QString::fromLatin1("BarcodeTypes(): invalid enum value (%0)").arg(arg));
}


static QScriptValue qtscript_BarcodeItem_BarcodeTypes_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::BarcodeTypes value = qscriptvalue_cast<BarcodeItem::BarcodeTypes>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}


static QScriptValue qtscript_BarcodeItem_BarcodeTypes_toString(QScriptContext *context, QScriptEngine *engine)
{
    BarcodeItem::BarcodeTypes value = qscriptvalue_cast<BarcodeItem::BarcodeTypes>(context->thisObject());
    return QScriptValue(engine, qtscript_BarcodeItem_BarcodeTypes_toStringHelper(value));
}


static QScriptValue qtscript_create_BarcodeItem_BarcodeTypes_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
                            engine, qtscript_construct_BarcodeItem_BarcodeTypes,
                            qtscript_BarcodeItem_BarcodeTypes_valueOf, qtscript_BarcodeItem_BarcodeTypes_toString);

    qScriptRegisterMetaType<BarcodeItem::BarcodeTypes>(engine, qtscript_BarcodeItem_BarcodeTypes_toScriptValue,
                                                   qtscript_BarcodeItem_BarcodeTypes_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));

    for (int i = 0; i < BarcodeItem_BarcodeTypes_num; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_BarcodeItem_BarcodeTypes_keys[i]),
                          engine->newVariant(qVariantFromValue(qtscript_BarcodeItem_BarcodeTypes_values[i])),
                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }

    return ctor;
}


QString BarcodeItem_BarcodeTypes_toString(BarcodeItem::BarcodeTypes value)
{
    int idx = -1;
    for (int i = 0; i < BarcodeItem_BarcodeTypes_num; ++i) {
        if (qtscript_BarcodeItem_BarcodeTypes_values[i] == value) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_BarcodeTypes_toString", QString("Value \'%1\' is not within range").arg((int)value));
        return QString();
    }

    return QString(qtscript_BarcodeItem_BarcodeTypes_keys[idx]);
}


BarcodeItem::BarcodeTypes BarcodeItem_BarcodeTypes_fromString(const QString &value, bool *ok)
{
    if (ok)
        *ok = true;
    QString strIn = value.toLower();
    int idx = -1;
    for (int i = 0; i < BarcodeItem_BarcodeTypes_num; ++i) {
        QString str(qtscript_BarcodeItem_BarcodeTypes_keys[i]);
        if (strIn == str.toLower()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        CuteReport::ReportCore::log(LogError, "BarcodeItem", "BarcodeItem_BarcodeTypes_fromString", QString("Value \'%1\' is not within range").arg(value));
        if (ok)
            *ok = false;
    }

    return BarcodeItem::BarcodeTypes(qtscript_BarcodeItem_BarcodeTypes_values[idx]);
}


QStringList BarcodeItem_BarcodeTypes_variants()
{
    QStringList list;
    for (int i = 0; i < BarcodeItem_BarcodeTypes_num; ++i) {
        list << QString(qtscript_BarcodeItem_BarcodeTypes_keys[i]);
    }
    return list;
}
/** ============================================================================================================= */


static const char * const qtscript_BarcodeItem_function_names[] = {
    "Barcode"
    , "toString"
};


static QScriptValue qtscript_BarcodeItem_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
        case 0:
            return context->throwError(QScriptContext::TypeError,
                                       QString::fromLatin1("BarcodeItem cannot be constructed"));
            break;

        default:
            Q_ASSERT(false);
    }
    return qtscript_Qt_throw_ambiguity_error_helper(context,  qtscript_BarcodeItem_function_names[_id],  "" /*qtscript_Qt_function_signatures[_id]*/);
}


QScriptValue qtscript_create_BarcodeItem_class(QScriptEngine *engine)
{
    QScriptValue proto = QScriptValue();
    QScriptValue ctor = engine->newFunction(qtscript_BarcodeItem_static_call, proto, 0);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("MsiPlessey"), qtscript_create_BarcodeItem_MsiPlessey_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("DrawTextTypes"), qtscript_create_BarcodeItem_DrawTextTypes_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("FrameTypes"), qtscript_create_BarcodeItem_FrameTypes_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("PaintTypes"), qtscript_create_BarcodeItem_PaintTypes_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("BarcodeTypes"), qtscript_create_BarcodeItem_BarcodeTypes_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Frame"), qtscript_create_Barcode_Frame_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ResizeFlags"), qtscript_create_Barcode_ResizeFlags_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ShiftMode"), qtscript_create_Barcode_ShiftMode_class(engine, ctor));

    return ctor;
}


void registerBarcodeItemScriptClass(QScriptEngine *engine)
{
    QScriptValue extensionObject = engine->globalObject();
    extensionObject.setProperty("Barcode", qtscript_create_BarcodeItem_class(engine), QScriptValue::SkipInEnumeration);
}

