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
#ifndef DETAILHEADER_H
#define DETAILHEADER_H

#include <bandinterface.h>
#include "globals.h"

class DetailHeaderPrivate;
class DetailHeaderView;

class  DetailHeader : public CuteReport::BandInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.BandInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::BandInterface)

    Q_PROPERTY(QString dataset READ dataset WRITE setDataset NOTIFY datasetChanged)
    Q_PROPERTY(QString condition READ condition WRITE setCondition NOTIFY conditionChanged)
    Q_PROPERTY(bool reprintOnNewPage READ reprintOnNewPage WRITE setReprintOnNewPage NOTIFY reprintOnNewPageChanged)
    Q_PROPERTY(bool startNewPage READ startNewPage WRITE setStartNewPage NOTIFY startNewpageChanged)
    Q_PROPERTY(bool resetDetailNumber READ resetDetailNumber WRITE setResetDetailNumber)


public:
    DetailHeader(QObject * parent = 0);
    virtual ~DetailHeader();

    virtual BaseItemInterface * createInstance(QObject * parent) const;
    virtual BaseItemInterface * clone();
    virtual QByteArray serialize();
    virtual void deserialize(QByteArray &data);
    virtual bool canContain(QObject * object);

    virtual void renderInit(CuteReport::RendererPublicInterface * renderer);
    virtual void renderReset();
    virtual CuteReport::RenderedItemInterface * render(int customDPI = 0);

    int layoutPriority() const { return 50;}
    BandInterface::AccomodationType accommodationType() const {return AccomodationEveryPage;}

    virtual QIcon itemIcon() const;
    virtual QString moduleName() const;
    virtual QString itemGroup() const;

    QString dataset() const;
    void setDataset(const QString & dataset);

    QString condition() const;
    void setCondition(const QString & condition);

    bool startNewPage() const;
    void setStartNewPage(bool b);

    bool reprintOnNewPage() const;
    void setReprintOnNewPage(bool b);

    bool resetDetailNumber() const;
    void setResetDetailNumber(bool b);

    QString _current_property_description() const;

signals:
    void datasetChanged(QString);
    void conditionChanged(QString);
    void startNewpageChanged(bool);
    void reprintOnNewPageChanged(bool);
    void resetDetailNumberChanged(bool);


protected:
    DetailHeader(DetailHeaderPrivate &dd, QObject * parent);

private:
    Q_DECLARE_PRIVATE(DetailHeader)
    CuteReport::RendererPublicInterface * m_renderer;
};


//} //namespace
#endif
