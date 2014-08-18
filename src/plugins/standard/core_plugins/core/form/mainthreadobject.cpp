#include "mainthreadobject.h"
#include "reportcore.h"

#include <QUiLoader>
#include <QApplication>
#include <QDesktopWidget>
#include <QBuffer>

using namespace CuteReport;

MainThreadObject::MainThreadObject(Form * form) :
    QObject(0),
    m_form(form),
    m_dialog(0)
{
    connect(this, SIGNAL(get(Parameters*)), SLOT(getCurrentThread(Parameters*)), Qt::DirectConnection);
    connect(this, SIGNAL(getFromAnotherThread(Parameters*)), SLOT(invokeWidgetMethod(Parameters*)), Qt::BlockingQueuedConnection);
}


MainThreadObject::~MainThreadObject()
{
    clear();

//    disconnect(this, SIGNAL(get(Parameters*)), this, SLOT(getCurrentThread(Parameters*)));
//    disconnect(this, SIGNAL(getFromAnotherThread(Parameters*)), this, SLOT(invokeWidgetMethod(Parameters*)));
}


QVariant MainThreadObject::invoke(const QString &objectName, const QString &method, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6)
{
//    qDebug() << (qint64) QThread::currentThread();
//    qDebug() << (qint64) this->thread();

    Parameters value;
    value.objectName = objectName;
    value.method = method;
    value.arg1 = arg1;
    value.arg2 = arg2;
    value.arg3 = arg3;
    value.arg4 = arg4;
    value.arg5 = arg5;
    value.arg6 = arg6;

    emit get(&value);

    if (!value.error.isEmpty())
        ReportCore::log(LogError, "Form", QString("Invoke method error"), QString("Error: %1").arg(value.error));

    return value.result;
}


void MainThreadObject::clear()
{
    if (m_dialog) {
//        metaObject()->invokeMethod(m_dialog, "deleteLater", Qt::QueuedConnection);
//        m_dialog->hide();
        delete m_dialog;
//        m_dialog->deleteLater();
        m_dialog = 0;
    }
}


void MainThreadObject::getCurrentThread(Parameters *value)
{
    QThread *thread = QThread::currentThread();
    QThread *mainthread = this->thread();

    qDebug() << (qint64)thread;
    qDebug() << (qint64)mainthread;

    if(thread == mainthread)
        invokeWidgetMethod(value);
    else
        emit getFromAnotherThread(value);
}


void MainThreadObject::invokeWidgetMethod(Parameters *value)
{
    initWidget();

    QWidget * w = (m_dialog->objectName() == value->objectName) ? m_dialog : m_dialog->findChild<QWidget*>(value->objectName);
    if (!w) {
        value->error = QString("object with name \'%1\' is not found").arg(value->objectName);
        return;
    }

    const QMetaObject * metaObject = w->metaObject();
    int index = metaObject->indexOfMethod(value->method.toLatin1());

    if (index == -1) {
        value->error = QString("method \'%1\' is not found").arg(value->method);
        return;
    }

    QMetaMethod method = metaObject->method(index);
    qDebug() << method.typeName();
    qDebug() << method.parameterTypes();
    //    qDebug() << method.parameterNames();


    qDebug() << (qint64) QThread::currentThread();
    qDebug() << (qint64) w->thread();

    bool invokable;

    QVariant::Type variantType = QVariant::nameToType(method.typeName());
    int argCount = method.parameterTypes().size();

    QGenericArgument gArg1 = argCount >=1 ? makeGenericArgument(method.parameterTypes().at(0), value->arg1) : QGenericArgument();
    QGenericArgument gArg2 = argCount >=2 ? makeGenericArgument(method.parameterTypes().at(1), value->arg2) : QGenericArgument();
    QGenericArgument gArg3 = argCount >=3 ? makeGenericArgument(method.parameterTypes().at(2), value->arg3) : QGenericArgument();
    QGenericArgument gArg4 = argCount >=4 ? makeGenericArgument(method.parameterTypes().at(3), value->arg4) : QGenericArgument();
    QGenericArgument gArg5 = argCount >=5 ? makeGenericArgument(method.parameterTypes().at(4), value->arg5) : QGenericArgument();
    QGenericArgument gArg6 = argCount >=6 ? makeGenericArgument(method.parameterTypes().at(5), value->arg6) : QGenericArgument();

    if(variantType == QVariant::Invalid) {
        invokable = method.invoke(w, gArg1, gArg2, gArg3, gArg4, gArg5, gArg6) ;
    } else {
        QVariant val(variantType, (const void *) 0);
        QGenericReturnArgument genericvalue = QGenericReturnArgument(method.typeName(), val.data());
        invokable = method.invoke(w, genericvalue, gArg1, gArg2, gArg3, gArg4, gArg5, gArg6) ;
        value->result = val;
    }

    if (!invokable) {
        value->error = QString("method \'%1\' is not invocable").arg(value->method);
        return;
    }
}


void MainThreadObject::initWidget()
{
    if (m_dialog)
        return;

    qDebug() << (qint64) QThread::currentThread();
    qDebug() << (qint64) this->thread();

    QUiLoader loader;
    QBuffer buffer(this);
    buffer.setData(m_form->data().toUtf8());
    buffer.open(QBuffer::ReadOnly);
    m_dialog = dynamic_cast<QDialog*>(loader.load(&buffer));
    if (m_dialog) {
        m_dialog->setParent(m_form->reportCore()->rootWidget());
        m_dialog->setObjectName("widget");
        m_dialog->setModal(false);
        m_dialog->setGeometry( QRect(QApplication::desktop()->screen()->geometry().bottomRight()/2 - m_dialog->geometry().bottomRight()/2,
                                     QSize(m_dialog->width(), m_dialog->height()) ) );
        //        connect(m_dialog, SIGNAL(accepted()), this, SIGNAL(accepted()));
        //        connect(m_dialog, SIGNAL(rejected()), this, SIGNAL(rejected()));
    } else {
        ReportCore::log(LogError, "Form", QString("Form with name \'%1\' can't be initialized").arg(objectName()),
                        QString("Form data: %1").arg(m_form->data()));
    }
}


QGenericArgument MainThreadObject::makeGenericArgument(const QByteArray &parameterType, const QVariant &arg)
{
    // argument type should be the same as parameter type, castable type or QObject
    //    QVariant::Type varType = QVariant::nameToType(parameterType);
    //        QVariant arg(varType, value->arg1.data());
    qDebug() << "================================================";
    qDebug() << QVariant::nameToType(parameterType);
    qDebug() << arg.userType();
    qDebug() << arg.typeName();
    qDebug() << QMetaType::type(arg.typeName());
    qDebug() << QMetaType::type(parameterType);
    qDebug() << QMetaType::typeName(QMetaType::type(arg.typeName()));
    qDebug() << QMetaType::typeName(QMetaType::type(parameterType));
    qDebug() << arg.canConvert((QVariant::Type)QMetaType::type(parameterType));
    //    qDebug() << arg.canConvert<CuteReport::DatasetInterface*>();
    int parType = QMetaType::type(parameterType);
    int argType = QMetaType::type(arg.typeName());
    QString parTypeName = QMetaType::typeName(QMetaType::type(parameterType));

    if (parType != argType) {
        if (arg.canConvert((QVariant::Type)parType)) {
//            QVariant val = arg.convert((QVariant::Type)parType);
            return QGenericArgument(arg.typeName(), arg.data());
        } else if (arg.canConvert<QObject*>()) {
            QObject * o = arg.value<QObject*>();
            bool compatible = o->inherits(parTypeName.remove(QRegExp("\\*")).toLatin1());
            if (!compatible) {
                ReportCore::log(LogError, "Form::MainThreadObject", QString("Not appropriate QObject argument type for \'%1\'").arg(parTypeName));
                return QGenericArgument();
            }
        } else {
            ReportCore::log(LogError, "Form::MainThreadObject", QString("Not appropriate argument type for \'%1\'").arg(parTypeName));
            return QGenericArgument();
        }

    }
    return  QGenericArgument(arg.typeName(), arg.data());
}
