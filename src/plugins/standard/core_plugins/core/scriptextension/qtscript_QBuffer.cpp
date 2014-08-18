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

#include <qbuffer.h>
#include <QVariant>
#include <qbuffer.h>
#include <qbytearray.h>
#include <qcoreevent.h>
#include <qlist.h>
#include <qobject.h>

#include "qtscriptshell_QBuffer.h"

static const char * const qtscript_QBuffer_function_names[] = {
    "QBuffer"
    // static
    // prototype
    , "setBuffer"
    , "setData"
    , "toString"
};

static const char * const qtscript_QBuffer_function_signatures[] = {
    "QByteArray buf, QObject parent\nQObject parent"
    // static
    // prototype
    , "QByteArray a"
    , "QByteArray data"
""
};

static const int qtscript_QBuffer_function_lengths[] = {
    2
    // static
    // prototype
    , 1
    , 1
    , 0
};

static QScriptValue qtscript_QBuffer_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QBuffer::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QBuffer*)
Q_DECLARE_METATYPE(QtScriptShell_QBuffer*)
Q_DECLARE_METATYPE(QByteArray*)
Q_DECLARE_METATYPE(QIODevice*)

//
// QBuffer
//

static QScriptValue qtscript_QBuffer_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 2;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QBuffer* _q_self = qscriptvalue_cast<QBuffer*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QBuffer.%0(): this object is not a QBuffer")
            .arg(qtscript_QBuffer_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 1) {
        QByteArray* _q_arg0 = qscriptvalue_cast<QByteArray*>(context->argument(0));
        _q_self->setBuffer(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 1:
    if (context->argumentCount() == 1) {
        QByteArray _q_arg0 = qscriptvalue_cast<QByteArray>(context->argument(0));
        _q_self->setData(_q_arg0);
        return context->engine()->undefinedValue();
    }
    break;

    case 2: {
    QString result = QString::fromLatin1("QBuffer");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QBuffer_throw_ambiguity_error_helper(context,
        qtscript_QBuffer_function_names[_id+1],
        qtscript_QBuffer_function_signatures[_id+1]);
}

static QScriptValue qtscript_QBuffer_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QBuffer(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QBuffer* _q_cpp_result = new QtScriptShell_QBuffer();
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QBuffer*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        if (qscriptvalue_cast<QByteArray*>(context->argument(0))) {
            QByteArray* _q_arg0 = qscriptvalue_cast<QByteArray*>(context->argument(0));
            QtScriptShell_QBuffer* _q_cpp_result = new QtScriptShell_QBuffer(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QBuffer*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        } else if (context->argument(0).isQObject()) {
            QObject* _q_arg0 = context->argument(0).toQObject();
            QtScriptShell_QBuffer* _q_cpp_result = new QtScriptShell_QBuffer(_q_arg0);
            QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QBuffer*)_q_cpp_result, QScriptEngine::AutoOwnership);
            _q_cpp_result->__qtscript_self = _q_result;
            return _q_result;
        }
    } else if (context->argumentCount() == 2) {
        QByteArray* _q_arg0 = qscriptvalue_cast<QByteArray*>(context->argument(0));
        QObject* _q_arg1 = context->argument(1).toQObject();
        QtScriptShell_QBuffer* _q_cpp_result = new QtScriptShell_QBuffer(_q_arg0, _q_arg1);
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QBuffer*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QBuffer_throw_ambiguity_error_helper(context,
        qtscript_QBuffer_function_names[_id],
        qtscript_QBuffer_function_signatures[_id]);
}

static QScriptValue qtscript_QBuffer_toScriptValue(QScriptEngine *engine, QBuffer* const &in)
{
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

static void qtscript_QBuffer_fromScriptValue(const QScriptValue &value, QBuffer* &out)
{
    out = qobject_cast<QBuffer*>(value.toQObject());
}

QScriptValue qtscript_create_QBuffer_class(QScriptEngine *engine)
{
    engine->setDefaultPrototype(qMetaTypeId<QBuffer*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QBuffer*)0));
    proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QIODevice*>()));
    for (int i = 0; i < 3; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QBuffer_prototype_call, qtscript_QBuffer_function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QBuffer_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    qScriptRegisterMetaType<QBuffer*>(engine, qtscript_QBuffer_toScriptValue, 
        qtscript_QBuffer_fromScriptValue, proto);

    QScriptValue ctor = engine->newFunction(qtscript_QBuffer_static_call, proto, qtscript_QBuffer_function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    return ctor;
}
