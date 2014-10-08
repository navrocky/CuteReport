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
#include "form.h"
#include "reportcore.h"
#include "pageinterface.h"
#include "mainthreadobject.h"

#include <QApplication>

using namespace CuteReport;


Form::Form(QObject *parent) :
    FormInterface(parent),
    m_processor(0)
{
}


Form::Form(const Form &dd, QObject *parent) :
    FormInterface(parent),
    m_processor(0),
    m_data(dd.m_data)
{

}


Form::~Form()
{
    renderReset();
}


CuteReport::FormHelperInterface * Form::helper()
{
    return 0;
}


FormInterface * Form::createInstance( QObject * parent) const
{
    return new Form(parent);
}


FormInterface * Form::clone() const
{
    return new Form(*this, parent());
}


QString Form::data() const
{
    return m_data;
}


void Form::setData(const QString & data)
{
    if (m_data == data)
        return;
    m_data = data;
    emit dataChanged(m_data);
}


void Form::renderInit()
{
    if (m_processor)
        renderReset();

    m_processor = new MainThreadObject(this);

    QThread *currentThread = QThread::currentThread();
    QThread *mainThread = QApplication::instance()->thread();

    if (currentThread != mainThread) {
        m_processor->moveToThread(mainThread);
    }
}


void Form::renderReset()
{
    if (m_processor) {
        QThread *currentThread = QThread::currentThread();
        QThread *mainThread = QApplication::instance()->thread();

        if (currentThread != mainThread)
            metaObject()->invokeMethod(m_processor, "clear", Qt::BlockingQueuedConnection);
        else
            metaObject()->invokeMethod(m_processor, "clear", Qt::QueuedConnection);

        m_processor = 0;
    }
}


void Form::show()
{
    if (!m_processor)
        return;

    metaObject()->invokeMethod(m_processor, "show", Qt::QueuedConnection);
}


QVariant Form::invoke(const QString &objectName, const QString &method, const QVariant &arg1, const QVariant &arg2, const QVariant &arg3, const QVariant &arg4, const QVariant &arg5, const QVariant &arg6)
{
    if (!m_processor)
        return QVariant(QVariant::Invalid);

    QVariant result = m_processor->invoke(objectName, method, arg1, arg2, arg3, arg4, arg5, arg6);
    return result;
}



#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(form, Form)
#endif
