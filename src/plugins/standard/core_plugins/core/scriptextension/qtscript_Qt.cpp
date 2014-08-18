/***************************************************************************
 *   This file is part of the CuteReport project                           *
 *   Copyright (C) 2012-2014 by Alexander Mikhalov                         *
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
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>

#include <qnamespace.h>
#include <QGesture>
#include <QTextDocument>
#include <QVariant>

#include <qnamespace.h>
#include <qtextdocument.h>

static const char * const qtscript_Qt_function_names[] = {
    "Qt"
    // static
    // prototype
    , "toString"
};

static const char * const qtscript_Qt_function_signatures[] = {
    ""
    // static
    // prototype
""
};

static const int qtscript_Qt_function_lengths[] = {
    0
    // static
    // prototype
    , 0
};

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

struct qtscript_Qt_metaObject_helper : private QObject
{
    static const QMetaObject *get()
    { return &static_cast<qtscript_Qt_metaObject_helper*>(0)->staticQtMetaObject; }
};

static const QMetaObject *qtscript_Qt_metaObject()
{
    return qtscript_Qt_metaObject_helper::get();
}

Q_DECLARE_METATYPE(Qt::CheckState)
Q_DECLARE_METATYPE(Qt::DateFormat)
Q_DECLARE_METATYPE(Qt::TextFormat)
Q_DECLARE_METATYPE(Qt::PenJoinStyle)
Q_DECLARE_METATYPE(Qt::CaseSensitivity)
Q_DECLARE_METATYPE(Qt::ImageConversionFlag)
Q_DECLARE_METATYPE(QFlags<Qt::ImageConversionFlag>)
Q_DECLARE_METATYPE(Qt::BGMode)
Q_DECLARE_METATYPE(Qt::ConnectionType)
Q_DECLARE_METATYPE(Qt::CoordinateSystem)
Q_DECLARE_METATYPE(Qt::SizeMode)
Q_DECLARE_METATYPE(Qt::TileRule)
Q_DECLARE_METATYPE(Qt::WhiteSpaceMode)
Q_DECLARE_METATYPE(Qt::AspectRatioMode)
Q_DECLARE_METATYPE(Qt::AlignmentFlag)
Q_DECLARE_METATYPE(QFlags<Qt::AlignmentFlag>)
Q_DECLARE_METATYPE(Qt::GlobalColor)
Q_DECLARE_METATYPE(Qt::SortOrder)
Q_DECLARE_METATYPE(Qt::PenStyle)
Q_DECLARE_METATYPE(Qt::Axis)
Q_DECLARE_METATYPE(Qt::TransformationMode)
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(QFlags<Qt::Orientation>)
Q_DECLARE_METATYPE(Qt::TimeSpec)
Q_DECLARE_METATYPE(Qt::DayOfWeek)
Q_DECLARE_METATYPE(Qt::BrushStyle)
Q_DECLARE_METATYPE(Qt::MatchFlag)
Q_DECLARE_METATYPE(QFlags<Qt::MatchFlag>)
Q_DECLARE_METATYPE(Qt::PenCapStyle)
Q_DECLARE_METATYPE(Qt::TextElideMode)

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

static QScriptValue qtscript_create_flags_class_helper(
    QScriptEngine *engine,
    QScriptEngine::FunctionSignature construct,
    QScriptEngine::FunctionSignature valueOf,
    QScriptEngine::FunctionSignature toString,
    QScriptEngine::FunctionSignature equals)
{
    QScriptValue proto = engine->newObject();
    proto.setProperty(QString::fromLatin1("valueOf"),
        engine->newFunction(valueOf), QScriptValue::SkipInEnumeration);
    proto.setProperty(QString::fromLatin1("toString"),
        engine->newFunction(toString), QScriptValue::SkipInEnumeration);
    proto.setProperty(QString::fromLatin1("equals"),
        engine->newFunction(equals), QScriptValue::SkipInEnumeration);
    return engine->newFunction(construct, proto);
}


//
// Qt::CheckState
//

static const Qt::CheckState qtscript_Qt_CheckState_values[] = {
    Qt::Unchecked
    , Qt::PartiallyChecked
    , Qt::Checked
};

static const char * const qtscript_Qt_CheckState_keys[] = {
    "Unchecked"
    , "PartiallyChecked"
    , "Checked"
};

static QString qtscript_Qt_CheckState_toStringHelper(Qt::CheckState value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("CheckState");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_CheckState_toScriptValue(QScriptEngine *engine, const Qt::CheckState &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_CheckState_toStringHelper(value));
}

static void qtscript_Qt_CheckState_fromScriptValue(const QScriptValue &value, Qt::CheckState &out)
{
    out = qvariant_cast<Qt::CheckState>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_CheckState(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("CheckState");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::CheckState>(arg));
    return context->throwError(QString::fromLatin1("CheckState(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_CheckState_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CheckState value = qscriptvalue_cast<Qt::CheckState>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_CheckState_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CheckState value = qscriptvalue_cast<Qt::CheckState>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_CheckState_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_CheckState_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_CheckState,
        qtscript_Qt_CheckState_valueOf, qtscript_Qt_CheckState_toString);
    qScriptRegisterMetaType<Qt::CheckState>(engine, qtscript_Qt_CheckState_toScriptValue,
        qtscript_Qt_CheckState_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_CheckState_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_CheckState_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}



//
// Qt::DateFormat
//

static const Qt::DateFormat qtscript_Qt_DateFormat_values[] = {
    Qt::TextDate
    , Qt::ISODate
    , Qt::SystemLocaleDate
    , Qt::LocaleDate
    , Qt::SystemLocaleShortDate
    , Qt::SystemLocaleLongDate
    , Qt::DefaultLocaleShortDate
    , Qt::DefaultLocaleLongDate
};

static const char * const qtscript_Qt_DateFormat_keys[] = {
    "TextDate"
    , "ISODate"
    , "SystemLocaleDate"
    , "LocaleDate"
    , "SystemLocaleShortDate"
    , "SystemLocaleLongDate"
    , "DefaultLocaleShortDate"
    , "DefaultLocaleLongDate"
};

static QString qtscript_Qt_DateFormat_toStringHelper(Qt::DateFormat value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("DateFormat");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_DateFormat_toScriptValue(QScriptEngine *engine, const Qt::DateFormat &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_DateFormat_toStringHelper(value));
}

static void qtscript_Qt_DateFormat_fromScriptValue(const QScriptValue &value, Qt::DateFormat &out)
{
    out = qvariant_cast<Qt::DateFormat>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_DateFormat(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("DateFormat");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::DateFormat>(arg));
    return context->throwError(QString::fromLatin1("DateFormat(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_DateFormat_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::DateFormat value = qscriptvalue_cast<Qt::DateFormat>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_DateFormat_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::DateFormat value = qscriptvalue_cast<Qt::DateFormat>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_DateFormat_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_DateFormat_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_DateFormat,
        qtscript_Qt_DateFormat_valueOf, qtscript_Qt_DateFormat_toString);
    qScriptRegisterMetaType<Qt::DateFormat>(engine, qtscript_Qt_DateFormat_toScriptValue,
        qtscript_Qt_DateFormat_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 8; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_DateFormat_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_DateFormat_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::TextFormat
//

static const Qt::TextFormat qtscript_Qt_TextFormat_values[] = {
    Qt::PlainText
    , Qt::RichText
    , Qt::AutoText
};

static const char * const qtscript_Qt_TextFormat_keys[] = {
    "PlainText"
    , "RichText"
    , "AutoText"
};

static QString qtscript_Qt_TextFormat_toStringHelper(Qt::TextFormat value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TextFormat");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_TextFormat_toScriptValue(QScriptEngine *engine, const Qt::TextFormat &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_TextFormat_toStringHelper(value));
}

static void qtscript_Qt_TextFormat_fromScriptValue(const QScriptValue &value, Qt::TextFormat &out)
{
    out = qvariant_cast<Qt::TextFormat>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_TextFormat(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TextFormat");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::TextFormat>(arg));
    return context->throwError(QString::fromLatin1("TextFormat(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_TextFormat_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TextFormat value = qscriptvalue_cast<Qt::TextFormat>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_TextFormat_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TextFormat value = qscriptvalue_cast<Qt::TextFormat>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_TextFormat_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_TextFormat_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_TextFormat,
        qtscript_Qt_TextFormat_valueOf, qtscript_Qt_TextFormat_toString);
    qScriptRegisterMetaType<Qt::TextFormat>(engine, qtscript_Qt_TextFormat_toScriptValue,
        qtscript_Qt_TextFormat_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_TextFormat_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_TextFormat_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::PenJoinStyle
//

static const Qt::PenJoinStyle qtscript_Qt_PenJoinStyle_values[] = {
    Qt::MiterJoin
    , Qt::BevelJoin
    , Qt::RoundJoin
    , Qt::SvgMiterJoin
    , Qt::MPenJoinStyle
};

static const char * const qtscript_Qt_PenJoinStyle_keys[] = {
    "MiterJoin"
    , "BevelJoin"
    , "RoundJoin"
    , "SvgMiterJoin"
    , "MPenJoinStyle"
};

static QString qtscript_Qt_PenJoinStyle_toStringHelper(Qt::PenJoinStyle value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenJoinStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_PenJoinStyle_toScriptValue(QScriptEngine *engine, const Qt::PenJoinStyle &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_PenJoinStyle_toStringHelper(value));
}

static void qtscript_Qt_PenJoinStyle_fromScriptValue(const QScriptValue &value, Qt::PenJoinStyle &out)
{
    out = qvariant_cast<Qt::PenJoinStyle>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_PenJoinStyle(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenJoinStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::PenJoinStyle>(arg));
    return context->throwError(QString::fromLatin1("PenJoinStyle(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_PenJoinStyle_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenJoinStyle value = qscriptvalue_cast<Qt::PenJoinStyle>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_PenJoinStyle_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenJoinStyle value = qscriptvalue_cast<Qt::PenJoinStyle>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_PenJoinStyle_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_PenJoinStyle_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_PenJoinStyle,
        qtscript_Qt_PenJoinStyle_valueOf, qtscript_Qt_PenJoinStyle_toString);
    qScriptRegisterMetaType<Qt::PenJoinStyle>(engine, qtscript_Qt_PenJoinStyle_toScriptValue,
        qtscript_Qt_PenJoinStyle_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 5; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_PenJoinStyle_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_PenJoinStyle_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::CaseSensitivity
//

static const Qt::CaseSensitivity qtscript_Qt_CaseSensitivity_values[] = {
    Qt::CaseInsensitive
    , Qt::CaseSensitive
};

static const char * const qtscript_Qt_CaseSensitivity_keys[] = {
    "CaseInsensitive"
    , "CaseSensitive"
};

static QString qtscript_Qt_CaseSensitivity_toStringHelper(Qt::CaseSensitivity value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("CaseSensitivity");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_CaseSensitivity_toScriptValue(QScriptEngine *engine, const Qt::CaseSensitivity &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_CaseSensitivity_toStringHelper(value));
}

static void qtscript_Qt_CaseSensitivity_fromScriptValue(const QScriptValue &value, Qt::CaseSensitivity &out)
{
    out = qvariant_cast<Qt::CaseSensitivity>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_CaseSensitivity(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("CaseSensitivity");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::CaseSensitivity>(arg));
    return context->throwError(QString::fromLatin1("CaseSensitivity(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_CaseSensitivity_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CaseSensitivity value = qscriptvalue_cast<Qt::CaseSensitivity>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_CaseSensitivity_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CaseSensitivity value = qscriptvalue_cast<Qt::CaseSensitivity>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_CaseSensitivity_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_CaseSensitivity_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_CaseSensitivity,
        qtscript_Qt_CaseSensitivity_valueOf, qtscript_Qt_CaseSensitivity_toString);
    qScriptRegisterMetaType<Qt::CaseSensitivity>(engine, qtscript_Qt_CaseSensitivity_toScriptValue,
        qtscript_Qt_CaseSensitivity_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_CaseSensitivity_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_CaseSensitivity_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::ImageConversionFlag
//

static const Qt::ImageConversionFlag qtscript_Qt_ImageConversionFlag_values[] = {
    Qt::AutoColor
    , Qt::MonoOnly
    , Qt::ColorMode_Mask
    , Qt::OrderedAlphaDither
    , Qt::DiffuseAlphaDither
    , Qt::AlphaDither_Mask
    , Qt::OrderedDither
    , Qt::ThresholdDither
    , Qt::Dither_Mask
    , Qt::PreferDither
    , Qt::AvoidDither
    , Qt::DitherMode_Mask
    , Qt::NoOpaqueDetection
    , Qt::NoFormatConversion
};

static const char * const qtscript_Qt_ImageConversionFlag_keys[] = {
    "AutoColor"
    , "MonoOnly"
    , "ColorMode_Mask"
    , "OrderedAlphaDither"
    , "DiffuseAlphaDither"
    , "AlphaDither_Mask"
    , "OrderedDither"
    , "ThresholdDither"
    , "Dither_Mask"
    , "PreferDither"
    , "AvoidDither"
    , "DitherMode_Mask"
    , "NoOpaqueDetection"
    , "NoFormatConversion"
};

static QString qtscript_Qt_ImageConversionFlag_toStringHelper(Qt::ImageConversionFlag value)
{
    for (int i = 0; i < 14; ++i) {
        if (qtscript_Qt_ImageConversionFlag_values[i] == value)
            return QString::fromLatin1(qtscript_Qt_ImageConversionFlag_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_Qt_ImageConversionFlag_toScriptValue(QScriptEngine *engine, const Qt::ImageConversionFlag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_ImageConversionFlag_toStringHelper(value));
}

static void qtscript_Qt_ImageConversionFlag_fromScriptValue(const QScriptValue &value, Qt::ImageConversionFlag &out)
{
    out = qvariant_cast<Qt::ImageConversionFlag>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_ImageConversionFlag(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    for (int i = 0; i < 14; ++i) {
        if (qtscript_Qt_ImageConversionFlag_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<Qt::ImageConversionFlag>(arg));
    }
    return context->throwError(QString::fromLatin1("ImageConversionFlag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_ImageConversionFlag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ImageConversionFlag value = qscriptvalue_cast<Qt::ImageConversionFlag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_ImageConversionFlag_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ImageConversionFlag value = qscriptvalue_cast<Qt::ImageConversionFlag>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_ImageConversionFlag_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_ImageConversionFlag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_ImageConversionFlag,
        qtscript_Qt_ImageConversionFlag_valueOf, qtscript_Qt_ImageConversionFlag_toString);
    qScriptRegisterMetaType<Qt::ImageConversionFlag>(engine, qtscript_Qt_ImageConversionFlag_toScriptValue,
        qtscript_Qt_ImageConversionFlag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 14; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_ImageConversionFlag_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_ImageConversionFlag_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::ImageConversionFlags
//

static QScriptValue qtscript_Qt_ImageConversionFlags_toScriptValue(QScriptEngine *engine, const Qt::ImageConversionFlags &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_Qt_ImageConversionFlags_fromScriptValue(const QScriptValue &value, Qt::ImageConversionFlags &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<Qt::ImageConversionFlags>())
        out = qvariant_cast<Qt::ImageConversionFlags>(var);
    else if (var.userType() == qMetaTypeId<Qt::ImageConversionFlag>())
        out = qvariant_cast<Qt::ImageConversionFlag>(var);
    else
        out = 0;
}

static QScriptValue qtscript_construct_Qt_ImageConversionFlags(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ImageConversionFlags result = 0;
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<Qt::ImageConversionFlags>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<Qt::ImageConversionFlag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("ImageConversionFlags(): argument %0 is not of type ImageConversionFlag").arg(i));
            }
            result |= qvariant_cast<Qt::ImageConversionFlag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_Qt_ImageConversionFlags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ImageConversionFlags value = qscriptvalue_cast<Qt::ImageConversionFlags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_ImageConversionFlags_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ImageConversionFlags value = qscriptvalue_cast<Qt::ImageConversionFlags>(context->thisObject());
    QString result;
    for (int i = 0; i < 14; ++i) {
        if ((value & qtscript_Qt_ImageConversionFlag_values[i]) == qtscript_Qt_ImageConversionFlag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_Qt_ImageConversionFlag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_Qt_ImageConversionFlags_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<Qt::ImageConversionFlags>() == otherObj.value<Qt::ImageConversionFlags>())));
}

static QScriptValue qtscript_create_Qt_ImageConversionFlags_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_Qt_ImageConversionFlags, qtscript_Qt_ImageConversionFlags_valueOf,
        qtscript_Qt_ImageConversionFlags_toString, qtscript_Qt_ImageConversionFlags_equals);
    qScriptRegisterMetaType<Qt::ImageConversionFlags>(engine, qtscript_Qt_ImageConversionFlags_toScriptValue,
        qtscript_Qt_ImageConversionFlags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}


//
// Qt::BGMode
//

static const Qt::BGMode qtscript_Qt_BGMode_values[] = {
    Qt::TransparentMode
    , Qt::OpaqueMode
};

static const char * const qtscript_Qt_BGMode_keys[] = {
    "TransparentMode"
    , "OpaqueMode"
};

static QString qtscript_Qt_BGMode_toStringHelper(Qt::BGMode value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("BGMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_BGMode_toScriptValue(QScriptEngine *engine, const Qt::BGMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_BGMode_toStringHelper(value));
}

static void qtscript_Qt_BGMode_fromScriptValue(const QScriptValue &value, Qt::BGMode &out)
{
    out = qvariant_cast<Qt::BGMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_BGMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("BGMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::BGMode>(arg));
    return context->throwError(QString::fromLatin1("BGMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_BGMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::BGMode value = qscriptvalue_cast<Qt::BGMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_BGMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::BGMode value = qscriptvalue_cast<Qt::BGMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_BGMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_BGMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_BGMode,
        qtscript_Qt_BGMode_valueOf, qtscript_Qt_BGMode_toString);
    qScriptRegisterMetaType<Qt::BGMode>(engine, qtscript_Qt_BGMode_toScriptValue,
        qtscript_Qt_BGMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_BGMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_BGMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::ConnectionType
//

static const Qt::ConnectionType qtscript_Qt_ConnectionType_values[] = {
    Qt::AutoConnection
    , Qt::DirectConnection
    , Qt::QueuedConnection
    , Qt::BlockingQueuedConnection
    , Qt::UniqueConnection
};

static const char * const qtscript_Qt_ConnectionType_keys[] = {
    "AutoConnection"
    , "DirectConnection"
    , "QueuedConnection"
    , "BlockingQueuedConnection"
    , "UniqueConnection"
};

static QString qtscript_Qt_ConnectionType_toStringHelper(Qt::ConnectionType value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("ConnectionType");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_ConnectionType_toScriptValue(QScriptEngine *engine, const Qt::ConnectionType &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_ConnectionType_toStringHelper(value));
}

static void qtscript_Qt_ConnectionType_fromScriptValue(const QScriptValue &value, Qt::ConnectionType &out)
{
    out = qvariant_cast<Qt::ConnectionType>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_ConnectionType(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("ConnectionType");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::ConnectionType>(arg));
    return context->throwError(QString::fromLatin1("ConnectionType(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_ConnectionType_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ConnectionType value = qscriptvalue_cast<Qt::ConnectionType>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_ConnectionType_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::ConnectionType value = qscriptvalue_cast<Qt::ConnectionType>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_ConnectionType_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_ConnectionType_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_ConnectionType,
        qtscript_Qt_ConnectionType_valueOf, qtscript_Qt_ConnectionType_toString);
    qScriptRegisterMetaType<Qt::ConnectionType>(engine, qtscript_Qt_ConnectionType_toScriptValue,
        qtscript_Qt_ConnectionType_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 5; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_ConnectionType_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_ConnectionType_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::CoordinateSystem
//

static const Qt::CoordinateSystem qtscript_Qt_CoordinateSystem_values[] = {
    Qt::DeviceCoordinates
    , Qt::LogicalCoordinates
};

static const char * const qtscript_Qt_CoordinateSystem_keys[] = {
    "DeviceCoordinates"
    , "LogicalCoordinates"
};

static QString qtscript_Qt_CoordinateSystem_toStringHelper(Qt::CoordinateSystem value)
{
    if ((value >= Qt::DeviceCoordinates) && (value <= Qt::LogicalCoordinates))
        return qtscript_Qt_CoordinateSystem_keys[static_cast<int>(value)-static_cast<int>(Qt::DeviceCoordinates)];
    return QString();
}

static QScriptValue qtscript_Qt_CoordinateSystem_toScriptValue(QScriptEngine *engine, const Qt::CoordinateSystem &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_CoordinateSystem_toStringHelper(value));
}

static void qtscript_Qt_CoordinateSystem_fromScriptValue(const QScriptValue &value, Qt::CoordinateSystem &out)
{
    out = qvariant_cast<Qt::CoordinateSystem>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_CoordinateSystem(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    if ((arg >= Qt::DeviceCoordinates) && (arg <= Qt::LogicalCoordinates))
        return qScriptValueFromValue(engine,  static_cast<Qt::CoordinateSystem>(arg));
    return context->throwError(QString::fromLatin1("CoordinateSystem(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_CoordinateSystem_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CoordinateSystem value = qscriptvalue_cast<Qt::CoordinateSystem>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_CoordinateSystem_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::CoordinateSystem value = qscriptvalue_cast<Qt::CoordinateSystem>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_CoordinateSystem_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_CoordinateSystem_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_CoordinateSystem,
        qtscript_Qt_CoordinateSystem_valueOf, qtscript_Qt_CoordinateSystem_toString);
    qScriptRegisterMetaType<Qt::CoordinateSystem>(engine, qtscript_Qt_CoordinateSystem_toScriptValue,
        qtscript_Qt_CoordinateSystem_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_CoordinateSystem_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_CoordinateSystem_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::SizeMode
//

static const Qt::SizeMode qtscript_Qt_SizeMode_values[] = {
    Qt::AbsoluteSize
    , Qt::RelativeSize
};

static const char * const qtscript_Qt_SizeMode_keys[] = {
    "AbsoluteSize"
    , "RelativeSize"
};

static QString qtscript_Qt_SizeMode_toStringHelper(Qt::SizeMode value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("SizeMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_SizeMode_toScriptValue(QScriptEngine *engine, const Qt::SizeMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_SizeMode_toStringHelper(value));
}

static void qtscript_Qt_SizeMode_fromScriptValue(const QScriptValue &value, Qt::SizeMode &out)
{
    out = qvariant_cast<Qt::SizeMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_SizeMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("SizeMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::SizeMode>(arg));
    return context->throwError(QString::fromLatin1("SizeMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_SizeMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::SizeMode value = qscriptvalue_cast<Qt::SizeMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_SizeMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::SizeMode value = qscriptvalue_cast<Qt::SizeMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_SizeMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_SizeMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_SizeMode,
        qtscript_Qt_SizeMode_valueOf, qtscript_Qt_SizeMode_toString);
    qScriptRegisterMetaType<Qt::SizeMode>(engine, qtscript_Qt_SizeMode_toScriptValue,
        qtscript_Qt_SizeMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_SizeMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_SizeMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::TileRule
//

static const Qt::TileRule qtscript_Qt_TileRule_values[] = {
    Qt::StretchTile
    , Qt::RepeatTile
    , Qt::RoundTile
};

static const char * const qtscript_Qt_TileRule_keys[] = {
    "StretchTile"
    , "RepeatTile"
    , "RoundTile"
};

static QString qtscript_Qt_TileRule_toStringHelper(Qt::TileRule value)
{
    if ((value >= Qt::StretchTile) && (value <= Qt::RoundTile))
        return qtscript_Qt_TileRule_keys[static_cast<int>(value)-static_cast<int>(Qt::StretchTile)];
    return QString();
}

static QScriptValue qtscript_Qt_TileRule_toScriptValue(QScriptEngine *engine, const Qt::TileRule &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_TileRule_toStringHelper(value));
}

static void qtscript_Qt_TileRule_fromScriptValue(const QScriptValue &value, Qt::TileRule &out)
{
    out = qvariant_cast<Qt::TileRule>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_TileRule(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    if ((arg >= Qt::StretchTile) && (arg <= Qt::RoundTile))
        return qScriptValueFromValue(engine,  static_cast<Qt::TileRule>(arg));
    return context->throwError(QString::fromLatin1("TileRule(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_TileRule_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TileRule value = qscriptvalue_cast<Qt::TileRule>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_TileRule_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TileRule value = qscriptvalue_cast<Qt::TileRule>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_TileRule_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_TileRule_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_TileRule,
        qtscript_Qt_TileRule_valueOf, qtscript_Qt_TileRule_toString);
    qScriptRegisterMetaType<Qt::TileRule>(engine, qtscript_Qt_TileRule_toScriptValue,
        qtscript_Qt_TileRule_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_TileRule_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_TileRule_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::WhiteSpaceMode
//

static const Qt::WhiteSpaceMode qtscript_Qt_WhiteSpaceMode_values[] = {
    Qt::WhiteSpaceModeUndefined
    , Qt::WhiteSpaceNormal
    , Qt::WhiteSpacePre
    , Qt::WhiteSpaceNoWrap
};

static const char * const qtscript_Qt_WhiteSpaceMode_keys[] = {
    "WhiteSpaceModeUndefined"
    , "WhiteSpaceNormal"
    , "WhiteSpacePre"
    , "WhiteSpaceNoWrap"
};

static QString qtscript_Qt_WhiteSpaceMode_toStringHelper(Qt::WhiteSpaceMode value)
{
    if ((value >= Qt::WhiteSpaceModeUndefined) && (value <= Qt::WhiteSpaceNoWrap))
        return qtscript_Qt_WhiteSpaceMode_keys[static_cast<int>(value)-static_cast<int>(Qt::WhiteSpaceModeUndefined)];
    return QString();
}

static QScriptValue qtscript_Qt_WhiteSpaceMode_toScriptValue(QScriptEngine *engine, const Qt::WhiteSpaceMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_WhiteSpaceMode_toStringHelper(value));
}

static void qtscript_Qt_WhiteSpaceMode_fromScriptValue(const QScriptValue &value, Qt::WhiteSpaceMode &out)
{
    out = qvariant_cast<Qt::WhiteSpaceMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_WhiteSpaceMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    if ((arg >= Qt::WhiteSpaceModeUndefined) && (arg <= Qt::WhiteSpaceNoWrap))
        return qScriptValueFromValue(engine,  static_cast<Qt::WhiteSpaceMode>(arg));
    return context->throwError(QString::fromLatin1("WhiteSpaceMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_WhiteSpaceMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::WhiteSpaceMode value = qscriptvalue_cast<Qt::WhiteSpaceMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_WhiteSpaceMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::WhiteSpaceMode value = qscriptvalue_cast<Qt::WhiteSpaceMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_WhiteSpaceMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_WhiteSpaceMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_WhiteSpaceMode,
        qtscript_Qt_WhiteSpaceMode_valueOf, qtscript_Qt_WhiteSpaceMode_toString);
    qScriptRegisterMetaType<Qt::WhiteSpaceMode>(engine, qtscript_Qt_WhiteSpaceMode_toScriptValue,
        qtscript_Qt_WhiteSpaceMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_WhiteSpaceMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_WhiteSpaceMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::AspectRatioMode
//

static const Qt::AspectRatioMode qtscript_Qt_AspectRatioMode_values[] = {
    Qt::IgnoreAspectRatio
    , Qt::KeepAspectRatio
    , Qt::KeepAspectRatioByExpanding
};

static const char * const qtscript_Qt_AspectRatioMode_keys[] = {
    "IgnoreAspectRatio"
    , "KeepAspectRatio"
    , "KeepAspectRatioByExpanding"
};

static QString qtscript_Qt_AspectRatioMode_toStringHelper(Qt::AspectRatioMode value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("AspectRatioMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_AspectRatioMode_toScriptValue(QScriptEngine *engine, const Qt::AspectRatioMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_AspectRatioMode_toStringHelper(value));
}

static void qtscript_Qt_AspectRatioMode_fromScriptValue(const QScriptValue &value, Qt::AspectRatioMode &out)
{
    out = qvariant_cast<Qt::AspectRatioMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_AspectRatioMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("AspectRatioMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::AspectRatioMode>(arg));
    return context->throwError(QString::fromLatin1("AspectRatioMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_AspectRatioMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::AspectRatioMode value = qscriptvalue_cast<Qt::AspectRatioMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_AspectRatioMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::AspectRatioMode value = qscriptvalue_cast<Qt::AspectRatioMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_AspectRatioMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_AspectRatioMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_AspectRatioMode,
        qtscript_Qt_AspectRatioMode_valueOf, qtscript_Qt_AspectRatioMode_toString);
    qScriptRegisterMetaType<Qt::AspectRatioMode>(engine, qtscript_Qt_AspectRatioMode_toScriptValue,
        qtscript_Qt_AspectRatioMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_AspectRatioMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_AspectRatioMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::AlignmentFlag
//

static const Qt::AlignmentFlag qtscript_Qt_AlignmentFlag_values[] = {
    Qt::AlignLeft
    , Qt::AlignRight
    , Qt::AlignHCenter
    , Qt::AlignJustify
    , Qt::AlignAbsolute
    , Qt::AlignHorizontal_Mask
    , Qt::AlignTop
    , Qt::AlignBottom
    , Qt::AlignVCenter
    , Qt::AlignCenter
    , Qt::AlignVertical_Mask
};

static const char * const qtscript_Qt_AlignmentFlag_keys[] = {
    "AlignLeft"
    , "AlignRight"
    , "AlignHCenter"
    , "AlignJustify"
    , "AlignAbsolute"
    , "AlignHorizontal_Mask"
    , "AlignTop"
    , "AlignBottom"
    , "AlignVCenter"
    , "AlignCenter"
    , "AlignVertical_Mask"
};

static QString qtscript_Qt_AlignmentFlag_toStringHelper(Qt::AlignmentFlag value)
{
    for (int i = 0; i < 11; ++i) {
        if (qtscript_Qt_AlignmentFlag_values[i] == value)
            return QString::fromLatin1(qtscript_Qt_AlignmentFlag_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_Qt_AlignmentFlag_toScriptValue(QScriptEngine *engine, const Qt::AlignmentFlag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_AlignmentFlag_toStringHelper(value));
}

static void qtscript_Qt_AlignmentFlag_fromScriptValue(const QScriptValue &value, Qt::AlignmentFlag &out)
{
    out = qvariant_cast<Qt::AlignmentFlag>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_AlignmentFlag(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    for (int i = 0; i < 11; ++i) {
        if (qtscript_Qt_AlignmentFlag_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<Qt::AlignmentFlag>(arg));
    }
    return context->throwError(QString::fromLatin1("AlignmentFlag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_AlignmentFlag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::AlignmentFlag value = qscriptvalue_cast<Qt::AlignmentFlag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_AlignmentFlag_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::AlignmentFlag value = qscriptvalue_cast<Qt::AlignmentFlag>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_AlignmentFlag_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_AlignmentFlag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_AlignmentFlag,
        qtscript_Qt_AlignmentFlag_valueOf, qtscript_Qt_AlignmentFlag_toString);
    qScriptRegisterMetaType<Qt::AlignmentFlag>(engine, qtscript_Qt_AlignmentFlag_toScriptValue,
        qtscript_Qt_AlignmentFlag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 11; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_AlignmentFlag_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_AlignmentFlag_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::Alignment
//

static QScriptValue qtscript_Qt_Alignment_toScriptValue(QScriptEngine *engine, const Qt::Alignment &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_Qt_Alignment_fromScriptValue(const QScriptValue &value, Qt::Alignment &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<Qt::Alignment>())
        out = qvariant_cast<Qt::Alignment>(var);
    else if (var.userType() == qMetaTypeId<Qt::AlignmentFlag>())
        out = qvariant_cast<Qt::AlignmentFlag>(var);
    else
        out = 0;
}

static QScriptValue qtscript_construct_Qt_Alignment(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Alignment result = 0;
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<Qt::Alignment>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<Qt::AlignmentFlag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("Alignment(): argument %0 is not of type AlignmentFlag").arg(i));
            }
            result |= qvariant_cast<Qt::AlignmentFlag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_Qt_Alignment_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Alignment value = qscriptvalue_cast<Qt::Alignment>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_Alignment_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Alignment value = qscriptvalue_cast<Qt::Alignment>(context->thisObject());
    QString result;
    for (int i = 0; i < 11; ++i) {
        if ((value & qtscript_Qt_AlignmentFlag_values[i]) == qtscript_Qt_AlignmentFlag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_Qt_AlignmentFlag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_Qt_Alignment_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<Qt::Alignment>() == otherObj.value<Qt::Alignment>())));
}

static QScriptValue qtscript_create_Qt_Alignment_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_Qt_Alignment, qtscript_Qt_Alignment_valueOf,
        qtscript_Qt_Alignment_toString, qtscript_Qt_Alignment_equals);
    qScriptRegisterMetaType<Qt::Alignment>(engine, qtscript_Qt_Alignment_toScriptValue,
        qtscript_Qt_Alignment_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}



//
// Qt::GlobalColor
//

static const Qt::GlobalColor qtscript_Qt_GlobalColor_values[] = {
    Qt::color0
    , Qt::color1
    , Qt::black
    , Qt::white
    , Qt::darkGray
    , Qt::gray
    , Qt::lightGray
    , Qt::red
    , Qt::green
    , Qt::blue
    , Qt::cyan
    , Qt::magenta
    , Qt::yellow
    , Qt::darkRed
    , Qt::darkGreen
    , Qt::darkBlue
    , Qt::darkCyan
    , Qt::darkMagenta
    , Qt::darkYellow
    , Qt::transparent
};

static const char * const qtscript_Qt_GlobalColor_keys[] = {
    "color0"
    , "color1"
    , "black"
    , "white"
    , "darkGray"
    , "gray"
    , "lightGray"
    , "red"
    , "green"
    , "blue"
    , "cyan"
    , "magenta"
    , "yellow"
    , "darkRed"
    , "darkGreen"
    , "darkBlue"
    , "darkCyan"
    , "darkMagenta"
    , "darkYellow"
    , "transparent"
};

static QString qtscript_Qt_GlobalColor_toStringHelper(Qt::GlobalColor value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("GlobalColor");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_GlobalColor_toScriptValue(QScriptEngine *engine, const Qt::GlobalColor &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_GlobalColor_toStringHelper(value));
}

static void qtscript_Qt_GlobalColor_fromScriptValue(const QScriptValue &value, Qt::GlobalColor &out)
{
    out = qvariant_cast<Qt::GlobalColor>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_GlobalColor(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("GlobalColor");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::GlobalColor>(arg));
    return context->throwError(QString::fromLatin1("GlobalColor(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_GlobalColor_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::GlobalColor value = qscriptvalue_cast<Qt::GlobalColor>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_GlobalColor_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::GlobalColor value = qscriptvalue_cast<Qt::GlobalColor>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_GlobalColor_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_GlobalColor_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_GlobalColor,
        qtscript_Qt_GlobalColor_valueOf, qtscript_Qt_GlobalColor_toString);
    qScriptRegisterMetaType<Qt::GlobalColor>(engine, qtscript_Qt_GlobalColor_toScriptValue,
        qtscript_Qt_GlobalColor_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 20; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_GlobalColor_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_GlobalColor_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::SortOrder
//

static const Qt::SortOrder qtscript_Qt_SortOrder_values[] = {
    Qt::AscendingOrder
    , Qt::DescendingOrder
};

static const char * const qtscript_Qt_SortOrder_keys[] = {
    "AscendingOrder"
    , "DescendingOrder"
};

static QString qtscript_Qt_SortOrder_toStringHelper(Qt::SortOrder value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("SortOrder");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_SortOrder_toScriptValue(QScriptEngine *engine, const Qt::SortOrder &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_SortOrder_toStringHelper(value));
}

static void qtscript_Qt_SortOrder_fromScriptValue(const QScriptValue &value, Qt::SortOrder &out)
{
    out = qvariant_cast<Qt::SortOrder>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_SortOrder(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("SortOrder");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::SortOrder>(arg));
    return context->throwError(QString::fromLatin1("SortOrder(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_SortOrder_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::SortOrder value = qscriptvalue_cast<Qt::SortOrder>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_SortOrder_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::SortOrder value = qscriptvalue_cast<Qt::SortOrder>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_SortOrder_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_SortOrder_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_SortOrder,
        qtscript_Qt_SortOrder_valueOf, qtscript_Qt_SortOrder_toString);
    qScriptRegisterMetaType<Qt::SortOrder>(engine, qtscript_Qt_SortOrder_toScriptValue,
        qtscript_Qt_SortOrder_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_SortOrder_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_SortOrder_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::PenStyle
//

static const Qt::PenStyle qtscript_Qt_PenStyle_values[] = {
    Qt::NoPen
    , Qt::SolidLine
    , Qt::DashLine
    , Qt::DotLine
    , Qt::DashDotLine
    , Qt::DashDotDotLine
    , Qt::CustomDashLine
    , Qt::MPenStyle
};

static const char * const qtscript_Qt_PenStyle_keys[] = {
    "NoPen"
    , "SolidLine"
    , "DashLine"
    , "DotLine"
    , "DashDotLine"
    , "DashDotDotLine"
    , "CustomDashLine"
    , "MPenStyle"
};

static QString qtscript_Qt_PenStyle_toStringHelper(Qt::PenStyle value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_PenStyle_toScriptValue(QScriptEngine *engine, const Qt::PenStyle &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_PenStyle_toStringHelper(value));
}

static void qtscript_Qt_PenStyle_fromScriptValue(const QScriptValue &value, Qt::PenStyle &out)
{
    out = qvariant_cast<Qt::PenStyle>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_PenStyle(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::PenStyle>(arg));
    return context->throwError(QString::fromLatin1("PenStyle(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_PenStyle_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenStyle value = qscriptvalue_cast<Qt::PenStyle>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_PenStyle_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenStyle value = qscriptvalue_cast<Qt::PenStyle>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_PenStyle_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_PenStyle_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_PenStyle,
        qtscript_Qt_PenStyle_valueOf, qtscript_Qt_PenStyle_toString);
    qScriptRegisterMetaType<Qt::PenStyle>(engine, qtscript_Qt_PenStyle_toScriptValue,
        qtscript_Qt_PenStyle_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 8; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_PenStyle_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_PenStyle_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::Axis
//

static const Qt::Axis qtscript_Qt_Axis_values[] = {
    Qt::XAxis
    , Qt::YAxis
    , Qt::ZAxis
};

static const char * const qtscript_Qt_Axis_keys[] = {
    "XAxis"
    , "YAxis"
    , "ZAxis"
};

static QString qtscript_Qt_Axis_toStringHelper(Qt::Axis value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("Axis");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_Axis_toScriptValue(QScriptEngine *engine, const Qt::Axis &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_Axis_toStringHelper(value));
}

static void qtscript_Qt_Axis_fromScriptValue(const QScriptValue &value, Qt::Axis &out)
{
    out = qvariant_cast<Qt::Axis>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_Axis(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("Axis");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::Axis>(arg));
    return context->throwError(QString::fromLatin1("Axis(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_Axis_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Axis value = qscriptvalue_cast<Qt::Axis>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_Axis_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Axis value = qscriptvalue_cast<Qt::Axis>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_Axis_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_Axis_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_Axis,
        qtscript_Qt_Axis_valueOf, qtscript_Qt_Axis_toString);
    qScriptRegisterMetaType<Qt::Axis>(engine, qtscript_Qt_Axis_toScriptValue,
        qtscript_Qt_Axis_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_Axis_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_Axis_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::TransformationMode
//

static const Qt::TransformationMode qtscript_Qt_TransformationMode_values[] = {
    Qt::FastTransformation
    , Qt::SmoothTransformation
};

static const char * const qtscript_Qt_TransformationMode_keys[] = {
    "FastTransformation"
    , "SmoothTransformation"
};

static QString qtscript_Qt_TransformationMode_toStringHelper(Qt::TransformationMode value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TransformationMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_TransformationMode_toScriptValue(QScriptEngine *engine, const Qt::TransformationMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_TransformationMode_toStringHelper(value));
}

static void qtscript_Qt_TransformationMode_fromScriptValue(const QScriptValue &value, Qt::TransformationMode &out)
{
    out = qvariant_cast<Qt::TransformationMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_TransformationMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TransformationMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::TransformationMode>(arg));
    return context->throwError(QString::fromLatin1("TransformationMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_TransformationMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TransformationMode value = qscriptvalue_cast<Qt::TransformationMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_TransformationMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TransformationMode value = qscriptvalue_cast<Qt::TransformationMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_TransformationMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_TransformationMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_TransformationMode,
        qtscript_Qt_TransformationMode_valueOf, qtscript_Qt_TransformationMode_toString);
    qScriptRegisterMetaType<Qt::TransformationMode>(engine, qtscript_Qt_TransformationMode_toScriptValue,
        qtscript_Qt_TransformationMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_TransformationMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_TransformationMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::Orientation
//

static const Qt::Orientation qtscript_Qt_Orientation_values[] = {
    Qt::Horizontal
    , Qt::Vertical
};

static const char * const qtscript_Qt_Orientation_keys[] = {
    "Horizontal"
    , "Vertical"
};

static QString qtscript_Qt_Orientation_toStringHelper(Qt::Orientation value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("Orientation");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_Orientation_toScriptValue(QScriptEngine *engine, const Qt::Orientation &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_Orientation_toStringHelper(value));
}

static void qtscript_Qt_Orientation_fromScriptValue(const QScriptValue &value, Qt::Orientation &out)
{
    out = qvariant_cast<Qt::Orientation>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_Orientation(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("Orientation");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::Orientation>(arg));
    return context->throwError(QString::fromLatin1("Orientation(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_Orientation_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Orientation value = qscriptvalue_cast<Qt::Orientation>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_Orientation_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Orientation value = qscriptvalue_cast<Qt::Orientation>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_Orientation_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_Orientation_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_Orientation,
        qtscript_Qt_Orientation_valueOf, qtscript_Qt_Orientation_toString);
    qScriptRegisterMetaType<Qt::Orientation>(engine, qtscript_Qt_Orientation_toScriptValue,
        qtscript_Qt_Orientation_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 2; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_Orientation_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_Orientation_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::Orientations
//

static QScriptValue qtscript_Qt_Orientations_toScriptValue(QScriptEngine *engine, const Qt::Orientations &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_Qt_Orientations_fromScriptValue(const QScriptValue &value, Qt::Orientations &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<Qt::Orientations>())
        out = qvariant_cast<Qt::Orientations>(var);
    else if (var.userType() == qMetaTypeId<Qt::Orientation>())
        out = qvariant_cast<Qt::Orientation>(var);
    else
        out = 0;
}

static QScriptValue qtscript_construct_Qt_Orientations(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Orientations result = 0;
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<Qt::Orientations>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<Qt::Orientation>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("Orientations(): argument %0 is not of type Orientation").arg(i));
            }
            result |= qvariant_cast<Qt::Orientation>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_Qt_Orientations_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Orientations value = qscriptvalue_cast<Qt::Orientations>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_Orientations_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::Orientations value = qscriptvalue_cast<Qt::Orientations>(context->thisObject());
    QString result;
    for (int i = 0; i < 2; ++i) {
        if ((value & qtscript_Qt_Orientation_values[i]) == qtscript_Qt_Orientation_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_Qt_Orientation_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_Qt_Orientations_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<Qt::Orientations>() == otherObj.value<Qt::Orientations>())));
}

static QScriptValue qtscript_create_Qt_Orientations_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_Qt_Orientations, qtscript_Qt_Orientations_valueOf,
        qtscript_Qt_Orientations_toString, qtscript_Qt_Orientations_equals);
    qScriptRegisterMetaType<Qt::Orientations>(engine, qtscript_Qt_Orientations_toScriptValue,
        qtscript_Qt_Orientations_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}


//
// Qt::TimeSpec
//

static const Qt::TimeSpec qtscript_Qt_TimeSpec_values[] = {
    Qt::LocalTime
    , Qt::UTC
    , Qt::OffsetFromUTC
};

static const char * const qtscript_Qt_TimeSpec_keys[] = {
    "LocalTime"
    , "UTC"
    , "OffsetFromUTC"
};

static QString qtscript_Qt_TimeSpec_toStringHelper(Qt::TimeSpec value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TimeSpec");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_TimeSpec_toScriptValue(QScriptEngine *engine, const Qt::TimeSpec &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_TimeSpec_toStringHelper(value));
}

static void qtscript_Qt_TimeSpec_fromScriptValue(const QScriptValue &value, Qt::TimeSpec &out)
{
    out = qvariant_cast<Qt::TimeSpec>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_TimeSpec(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TimeSpec");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::TimeSpec>(arg));
    return context->throwError(QString::fromLatin1("TimeSpec(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_TimeSpec_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TimeSpec value = qscriptvalue_cast<Qt::TimeSpec>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_TimeSpec_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TimeSpec value = qscriptvalue_cast<Qt::TimeSpec>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_TimeSpec_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_TimeSpec_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_TimeSpec,
        qtscript_Qt_TimeSpec_valueOf, qtscript_Qt_TimeSpec_toString);
    qScriptRegisterMetaType<Qt::TimeSpec>(engine, qtscript_Qt_TimeSpec_toScriptValue,
        qtscript_Qt_TimeSpec_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 3; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_TimeSpec_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_TimeSpec_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::DayOfWeek
//

static const Qt::DayOfWeek qtscript_Qt_DayOfWeek_values[] = {
    Qt::Monday
    , Qt::Tuesday
    , Qt::Wednesday
    , Qt::Thursday
    , Qt::Friday
    , Qt::Saturday
    , Qt::Sunday
};

static const char * const qtscript_Qt_DayOfWeek_keys[] = {
    "Monday"
    , "Tuesday"
    , "Wednesday"
    , "Thursday"
    , "Friday"
    , "Saturday"
    , "Sunday"
};

static QString qtscript_Qt_DayOfWeek_toStringHelper(Qt::DayOfWeek value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("DayOfWeek");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_DayOfWeek_toScriptValue(QScriptEngine *engine, const Qt::DayOfWeek &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_DayOfWeek_toStringHelper(value));
}

static void qtscript_Qt_DayOfWeek_fromScriptValue(const QScriptValue &value, Qt::DayOfWeek &out)
{
    out = qvariant_cast<Qt::DayOfWeek>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_DayOfWeek(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("DayOfWeek");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::DayOfWeek>(arg));
    return context->throwError(QString::fromLatin1("DayOfWeek(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_DayOfWeek_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::DayOfWeek value = qscriptvalue_cast<Qt::DayOfWeek>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_DayOfWeek_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::DayOfWeek value = qscriptvalue_cast<Qt::DayOfWeek>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_DayOfWeek_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_DayOfWeek_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_DayOfWeek,
        qtscript_Qt_DayOfWeek_valueOf, qtscript_Qt_DayOfWeek_toString);
    qScriptRegisterMetaType<Qt::DayOfWeek>(engine, qtscript_Qt_DayOfWeek_toScriptValue,
        qtscript_Qt_DayOfWeek_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 7; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_DayOfWeek_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_DayOfWeek_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::BrushStyle
//

static const Qt::BrushStyle qtscript_Qt_BrushStyle_values[] = {
    Qt::NoBrush
    , Qt::SolidPattern
    , Qt::Dense1Pattern
    , Qt::Dense2Pattern
    , Qt::Dense3Pattern
    , Qt::Dense4Pattern
    , Qt::Dense5Pattern
    , Qt::Dense6Pattern
    , Qt::Dense7Pattern
    , Qt::HorPattern
    , Qt::VerPattern
    , Qt::CrossPattern
    , Qt::BDiagPattern
    , Qt::FDiagPattern
    , Qt::DiagCrossPattern
    , Qt::LinearGradientPattern
    , Qt::RadialGradientPattern
    , Qt::ConicalGradientPattern
    , Qt::TexturePattern
};

static const char * const qtscript_Qt_BrushStyle_keys[] = {
    "NoBrush"
    , "SolidPattern"
    , "Dense1Pattern"
    , "Dense2Pattern"
    , "Dense3Pattern"
    , "Dense4Pattern"
    , "Dense5Pattern"
    , "Dense6Pattern"
    , "Dense7Pattern"
    , "HorPattern"
    , "VerPattern"
    , "CrossPattern"
    , "BDiagPattern"
    , "FDiagPattern"
    , "DiagCrossPattern"
    , "LinearGradientPattern"
    , "RadialGradientPattern"
    , "ConicalGradientPattern"
    , "TexturePattern"
};

static QString qtscript_Qt_BrushStyle_toStringHelper(Qt::BrushStyle value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("BrushStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_BrushStyle_toScriptValue(QScriptEngine *engine, const Qt::BrushStyle &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_BrushStyle_toStringHelper(value));
}

static void qtscript_Qt_BrushStyle_fromScriptValue(const QScriptValue &value, Qt::BrushStyle &out)
{
    out = qvariant_cast<Qt::BrushStyle>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_BrushStyle(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("BrushStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::BrushStyle>(arg));
    return context->throwError(QString::fromLatin1("BrushStyle(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_BrushStyle_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::BrushStyle value = qscriptvalue_cast<Qt::BrushStyle>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_BrushStyle_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::BrushStyle value = qscriptvalue_cast<Qt::BrushStyle>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_BrushStyle_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_BrushStyle_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_BrushStyle,
        qtscript_Qt_BrushStyle_valueOf, qtscript_Qt_BrushStyle_toString);
    qScriptRegisterMetaType<Qt::BrushStyle>(engine, qtscript_Qt_BrushStyle_toScriptValue,
        qtscript_Qt_BrushStyle_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 19; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_BrushStyle_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_BrushStyle_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}


//
// Qt::MatchFlag
//

static const Qt::MatchFlag qtscript_Qt_MatchFlag_values[] = {
    Qt::MatchExactly
    , Qt::MatchContains
    , Qt::MatchStartsWith
    , Qt::MatchEndsWith
    , Qt::MatchRegExp
    , Qt::MatchWildcard
    , Qt::MatchFixedString
    , Qt::MatchCaseSensitive
    , Qt::MatchWrap
    , Qt::MatchRecursive
};

static const char * const qtscript_Qt_MatchFlag_keys[] = {
    "MatchExactly"
    , "MatchContains"
    , "MatchStartsWith"
    , "MatchEndsWith"
    , "MatchRegExp"
    , "MatchWildcard"
    , "MatchFixedString"
    , "MatchCaseSensitive"
    , "MatchWrap"
    , "MatchRecursive"
};

static QString qtscript_Qt_MatchFlag_toStringHelper(Qt::MatchFlag value)
{
    for (int i = 0; i < 10; ++i) {
        if (qtscript_Qt_MatchFlag_values[i] == value)
            return QString::fromLatin1(qtscript_Qt_MatchFlag_keys[i]);
    }
    return QString();
}

static QScriptValue qtscript_Qt_MatchFlag_toScriptValue(QScriptEngine *engine, const Qt::MatchFlag &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_MatchFlag_toStringHelper(value));
}

static void qtscript_Qt_MatchFlag_fromScriptValue(const QScriptValue &value, Qt::MatchFlag &out)
{
    out = qvariant_cast<Qt::MatchFlag>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_MatchFlag(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    for (int i = 0; i < 10; ++i) {
        if (qtscript_Qt_MatchFlag_values[i] == arg)
            return qScriptValueFromValue(engine,  static_cast<Qt::MatchFlag>(arg));
    }
    return context->throwError(QString::fromLatin1("MatchFlag(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_MatchFlag_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::MatchFlag value = qscriptvalue_cast<Qt::MatchFlag>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_MatchFlag_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::MatchFlag value = qscriptvalue_cast<Qt::MatchFlag>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_MatchFlag_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_MatchFlag_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_MatchFlag,
        qtscript_Qt_MatchFlag_valueOf, qtscript_Qt_MatchFlag_toString);
    qScriptRegisterMetaType<Qt::MatchFlag>(engine, qtscript_Qt_MatchFlag_toScriptValue,
        qtscript_Qt_MatchFlag_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 10; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_MatchFlag_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_MatchFlag_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::MatchFlags
//

static QScriptValue qtscript_Qt_MatchFlags_toScriptValue(QScriptEngine *engine, const Qt::MatchFlags &value)
{
    return engine->newVariant(qVariantFromValue(value));
}

static void qtscript_Qt_MatchFlags_fromScriptValue(const QScriptValue &value, Qt::MatchFlags &out)
{
    QVariant var = value.toVariant();
    if (var.userType() == qMetaTypeId<Qt::MatchFlags>())
        out = qvariant_cast<Qt::MatchFlags>(var);
    else if (var.userType() == qMetaTypeId<Qt::MatchFlag>())
        out = qvariant_cast<Qt::MatchFlag>(var);
    else
        out = 0;
}

static QScriptValue qtscript_construct_Qt_MatchFlags(QScriptContext *context, QScriptEngine *engine)
{
    Qt::MatchFlags result = 0;
    if ((context->argumentCount() == 1) && context->argument(0).isNumber()) {
        result = static_cast<Qt::MatchFlags>(context->argument(0).toInt32());
    } else {
        for (int i = 0; i < context->argumentCount(); ++i) {
            QVariant v = context->argument(i).toVariant();
            if (v.userType() != qMetaTypeId<Qt::MatchFlag>()) {
                return context->throwError(QScriptContext::TypeError,
                    QString::fromLatin1("MatchFlags(): argument %0 is not of type MatchFlag").arg(i));
            }
            result |= qvariant_cast<Qt::MatchFlag>(v);
        }
   }
    return engine->newVariant(qVariantFromValue(result));
}

static QScriptValue qtscript_Qt_MatchFlags_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::MatchFlags value = qscriptvalue_cast<Qt::MatchFlags>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_MatchFlags_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::MatchFlags value = qscriptvalue_cast<Qt::MatchFlags>(context->thisObject());
    QString result;
    for (int i = 0; i < 10; ++i) {
        if ((value & qtscript_Qt_MatchFlag_values[i]) == qtscript_Qt_MatchFlag_values[i]) {
            if (!result.isEmpty())
                result.append(QString::fromLatin1(","));
            result.append(QString::fromLatin1(qtscript_Qt_MatchFlag_keys[i]));
        }
    }
    return QScriptValue(engine, result);
}

static QScriptValue qtscript_Qt_MatchFlags_equals(QScriptContext *context, QScriptEngine *engine)
{
    QVariant thisObj = context->thisObject().toVariant();
    QVariant otherObj = context->argument(0).toVariant();
    return QScriptValue(engine, ((thisObj.userType() == otherObj.userType()) &&
                                 (thisObj.value<Qt::MatchFlags>() == otherObj.value<Qt::MatchFlags>())));
}

static QScriptValue qtscript_create_Qt_MatchFlags_class(QScriptEngine *engine)
{
    QScriptValue ctor = qtscript_create_flags_class_helper(
        engine, qtscript_construct_Qt_MatchFlags, qtscript_Qt_MatchFlags_valueOf,
        qtscript_Qt_MatchFlags_toString, qtscript_Qt_MatchFlags_equals);
    qScriptRegisterMetaType<Qt::MatchFlags>(engine, qtscript_Qt_MatchFlags_toScriptValue,
        qtscript_Qt_MatchFlags_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    return ctor;
}

//
// Qt::PenCapStyle
//

static const Qt::PenCapStyle qtscript_Qt_PenCapStyle_values[] = {
    Qt::FlatCap
    , Qt::SquareCap
    , Qt::RoundCap
    , Qt::MPenCapStyle
};

static const char * const qtscript_Qt_PenCapStyle_keys[] = {
    "FlatCap"
    , "SquareCap"
    , "RoundCap"
    , "MPenCapStyle"
};

static QString qtscript_Qt_PenCapStyle_toStringHelper(Qt::PenCapStyle value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenCapStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_PenCapStyle_toScriptValue(QScriptEngine *engine, const Qt::PenCapStyle &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_PenCapStyle_toStringHelper(value));
}

static void qtscript_Qt_PenCapStyle_fromScriptValue(const QScriptValue &value, Qt::PenCapStyle &out)
{
    out = qvariant_cast<Qt::PenCapStyle>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_PenCapStyle(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("PenCapStyle");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::PenCapStyle>(arg));
    return context->throwError(QString::fromLatin1("PenCapStyle(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_PenCapStyle_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenCapStyle value = qscriptvalue_cast<Qt::PenCapStyle>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_PenCapStyle_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::PenCapStyle value = qscriptvalue_cast<Qt::PenCapStyle>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_PenCapStyle_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_PenCapStyle_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_PenCapStyle,
        qtscript_Qt_PenCapStyle_valueOf, qtscript_Qt_PenCapStyle_toString);
    qScriptRegisterMetaType<Qt::PenCapStyle>(engine, qtscript_Qt_PenCapStyle_toScriptValue,
        qtscript_Qt_PenCapStyle_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_PenCapStyle_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_PenCapStyle_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt::TextElideMode
//

static const Qt::TextElideMode qtscript_Qt_TextElideMode_values[] = {
    Qt::ElideLeft
    , Qt::ElideRight
    , Qt::ElideMiddle
    , Qt::ElideNone
};

static const char * const qtscript_Qt_TextElideMode_keys[] = {
    "ElideLeft"
    , "ElideRight"
    , "ElideMiddle"
    , "ElideNone"
};

static QString qtscript_Qt_TextElideMode_toStringHelper(Qt::TextElideMode value)
{
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TextElideMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    return QString::fromLatin1(menum.valueToKey(value));
}

static QScriptValue qtscript_Qt_TextElideMode_toScriptValue(QScriptEngine *engine, const Qt::TextElideMode &value)
{
    QScriptValue clazz = engine->globalObject().property(QString::fromLatin1("Qt"));
    return clazz.property(qtscript_Qt_TextElideMode_toStringHelper(value));
}

static void qtscript_Qt_TextElideMode_fromScriptValue(const QScriptValue &value, Qt::TextElideMode &out)
{
    out = qvariant_cast<Qt::TextElideMode>(value.toVariant());
}

static QScriptValue qtscript_construct_Qt_TextElideMode(QScriptContext *context, QScriptEngine *engine)
{
    int arg = context->argument(0).toInt32();
    const QMetaObject *meta = qtscript_Qt_metaObject();
    int idx = meta->indexOfEnumerator("TextElideMode");
    Q_ASSERT(idx != -1);
    QMetaEnum menum = meta->enumerator(idx);
    if (menum.valueToKey(arg) != 0)
        return qScriptValueFromValue(engine,  static_cast<Qt::TextElideMode>(arg));
    return context->throwError(QString::fromLatin1("TextElideMode(): invalid enum value (%0)").arg(arg));
}

static QScriptValue qtscript_Qt_TextElideMode_valueOf(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TextElideMode value = qscriptvalue_cast<Qt::TextElideMode>(context->thisObject());
    return QScriptValue(engine, static_cast<int>(value));
}

static QScriptValue qtscript_Qt_TextElideMode_toString(QScriptContext *context, QScriptEngine *engine)
{
    Qt::TextElideMode value = qscriptvalue_cast<Qt::TextElideMode>(context->thisObject());
    return QScriptValue(engine, qtscript_Qt_TextElideMode_toStringHelper(value));
}

static QScriptValue qtscript_create_Qt_TextElideMode_class(QScriptEngine *engine, QScriptValue &clazz)
{
    QScriptValue ctor = qtscript_create_enum_class_helper(
        engine, qtscript_construct_Qt_TextElideMode,
        qtscript_Qt_TextElideMode_valueOf, qtscript_Qt_TextElideMode_toString);
    qScriptRegisterMetaType<Qt::TextElideMode>(engine, qtscript_Qt_TextElideMode_toScriptValue,
        qtscript_Qt_TextElideMode_fromScriptValue, ctor.property(QString::fromLatin1("prototype")));
    for (int i = 0; i < 4; ++i) {
        clazz.setProperty(QString::fromLatin1(qtscript_Qt_TextElideMode_keys[i]),
            engine->newVariant(qVariantFromValue(qtscript_Qt_TextElideMode_values[i])),
            QScriptValue::ReadOnly | QScriptValue::Undeletable);
    }
    return ctor;
}

//
// Qt
//

static QScriptValue qtscript_Qt_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    return context->throwError(QScriptContext::TypeError,
        QString::fromLatin1("Qt cannot be constructed"));
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_Qt_throw_ambiguity_error_helper(context,
        qtscript_Qt_function_names[_id],
        qtscript_Qt_function_signatures[_id]);
}

QScriptValue qtscript_create_Qt_class(QScriptEngine *engine)
{
    QScriptValue proto = QScriptValue();
    QScriptValue ctor = engine->newFunction(qtscript_Qt_static_call, proto, qtscript_Qt_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    ctor.setProperty(QString::fromLatin1("CheckState"),
        qtscript_create_Qt_CheckState_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("DateFormat"),
        qtscript_create_Qt_DateFormat_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("TextFormat"),
        qtscript_create_Qt_TextFormat_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("PenJoinStyle"),
        qtscript_create_Qt_PenJoinStyle_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("CaseSensitivity"),
        qtscript_create_Qt_CaseSensitivity_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ImageConversionFlag"),
        qtscript_create_Qt_ImageConversionFlag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ImageConversionFlags"),
        qtscript_create_Qt_ImageConversionFlags_class(engine));
    ctor.setProperty(QString::fromLatin1("BGMode"),
        qtscript_create_Qt_BGMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("ConnectionType"),
        qtscript_create_Qt_ConnectionType_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("CoordinateSystem"),
        qtscript_create_Qt_CoordinateSystem_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("SizeMode"),
        qtscript_create_Qt_SizeMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("TileRule"),
        qtscript_create_Qt_TileRule_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("WhiteSpaceMode"),
        qtscript_create_Qt_WhiteSpaceMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("AspectRatioMode"),
        qtscript_create_Qt_AspectRatioMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("AlignmentFlag"),
        qtscript_create_Qt_AlignmentFlag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Alignment"),
        qtscript_create_Qt_Alignment_class(engine));
    ctor.setProperty(QString::fromLatin1("GlobalColor"),
        qtscript_create_Qt_GlobalColor_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("SortOrder"),
        qtscript_create_Qt_SortOrder_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("PenStyle"),
        qtscript_create_Qt_PenStyle_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Axis"),
        qtscript_create_Qt_Axis_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("TransformationMode"),
        qtscript_create_Qt_TransformationMode_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Orientation"),
        qtscript_create_Qt_Orientation_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("Orientations"),
        qtscript_create_Qt_Orientations_class(engine));
    ctor.setProperty(QString::fromLatin1("TimeSpec"),
        qtscript_create_Qt_TimeSpec_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("DayOfWeek"),
        qtscript_create_Qt_DayOfWeek_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("BrushStyle"),
        qtscript_create_Qt_BrushStyle_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("MatchFlag"),
        qtscript_create_Qt_MatchFlag_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("MatchFlags"),
        qtscript_create_Qt_MatchFlags_class(engine));
    ctor.setProperty(QString::fromLatin1("PenCapStyle"),
        qtscript_create_Qt_PenCapStyle_class(engine, ctor));
    ctor.setProperty(QString::fromLatin1("TextElideMode"),
        qtscript_create_Qt_TextElideMode_class(engine, ctor));
    return ctor;
}
