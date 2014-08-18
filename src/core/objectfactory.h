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
 ****************************************************************************/
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
#include <QObject>
#include <QHash>
#include <QDebug>
#include "typeinfo"
#include "globals.h"

class CUTEREPORT_EXPORTS ObjectFactory
{
public:
    static ObjectFactory *instance();
    static void createInstance();
    static void deleteInstance();

    template<typename T>
    void registerClass()
    {
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
        qDebug() << constructors().size();
    }

    QObject* createObject( const QByteArray& className, QObject* parent = NULL );

private:
    explicit ObjectFactory(){}
    explicit ObjectFactory(const ObjectFactory &){}
    ~ObjectFactory(){}

    typedef QObject* (*Constructor)( QObject* parent );

    template<typename T>
    static QObject* constructorHelper( QObject* parent )
    {
        return new T( parent );
    }

    static QHash<QByteArray, Constructor>& constructors();

    static ObjectFactory *pInstance;
};


#endif // OBJECTFACTORY_H
