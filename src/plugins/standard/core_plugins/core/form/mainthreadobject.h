#ifndef MAINTHREADOBJECT_H
#define MAINTHREADOBJECT_H

#include <QObject>
#include <QDialog>

#include "form.h"


class MainThreadObject : public QObject
{
    Q_OBJECT
public:
    struct Parameters {
        QString objectName;
        QString method;
        QVariant result;
        QString error;
        QVariant arg1;
        QVariant arg2;
        QVariant arg3;
        QVariant arg4;
        QVariant arg5;
        QVariant arg6;
    };

    explicit MainThreadObject(Form * form);
    ~MainThreadObject();

    QVariant invoke(const QString & objectName, const QString & method, const QVariant & arg1 = QVariant(),
                            const QVariant & arg2 = QVariant(), const QVariant & arg3 = QVariant(),
                            const QVariant & arg4 = QVariant(), const QVariant & arg5 = QVariant(), const QVariant & arg6 = QVariant());

public slots:
    void clear();

signals:
    void get(Parameters *value);
    void getFromAnotherThread(Parameters *value);

private slots:
    void getCurrentThread(Parameters *value);
    void invokeWidgetMethod(Parameters *value);

private:
    void initWidget();
    QGenericArgument makeGenericArgument(const QByteArray& parameterType, const QVariant & arg);

    Form * m_form;
    QDialog * m_dialog;
};

#endif // MAINTHREADOBJECT_H
