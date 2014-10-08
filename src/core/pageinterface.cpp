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
#include "pageinterface.h"
#include "iteminterface.h"
#include <QGraphicsView>
#include <QGraphicsScene>

namespace CuteReport
{

PageInterface::PageInterface(QObject *parent) :
    ReportPluginInterface(parent)
{
}

PageInterface::PageInterface(const PageInterface &dd, QObject * parent)
    :ReportPluginInterface(dd, parent)
{
}



PageInterface::~PageInterface()
{

}


QString PageInterface::_current_property_description() const
{
//    QString propertyName = metaObject()->property(m_currentProperty).name();

//    if (propertyName == "geometry")
//        return tr("Item geomery");
//    else if (propertyName == "frame")
//        return tr("Item frame drawing sides");
//    else if (propertyName == "opacity")
//        return tr("Item opacity");
//    else if (propertyName == "rotation")
//        return tr("Rotation angle");
//    else if (propertyName == "borderPen")
//        return tr("Border pen properties");
//    else if (propertyName == "backgroundBrush")
//        return tr("Border pen background properies");
//    else if (propertyName == "enabled")
//        return tr("Is item enabled. Disabled items wont processed");
//    else if (propertyName == "order")
//        return tr("position order");

    return QString();
}


int PageInterface::_current_property_precision() const
{
    return 1;
}






PageInterface *PageInterface::clone(bool withChildren, bool init) const
{
    PageInterface * newPage = this->objectClone();
    newPage->setReportCore(this->reportCore());

    if (withChildren) {
        foreach (CuteReport::BaseItemInterface* item, items()) {
            if (item->parent() != this)     // only direct children
                continue;
            CuteReport::BaseItemInterface * itemCopy = item->clone(true, false);
            itemCopy->setReportCore(item->reportCore());
            itemCopy->setParent(newPage);
            itemCopy->setObjectName(item->objectName());
        }
    }

    if (init)
        newPage->init();

    return newPage;
}


PageManipulatorInterface::PageManipulatorInterface(QObject *parent)
    :QObject(parent)
{

}





} //namespace
