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
#ifndef TYPES_H
#define TYPES_H

#include <QtCore>
#include <QUrl>
#include "globals.h"

namespace CuteReport{

class BaseItemInterface;
class BandInterface;


class CUTEREPORT_EXPORTS Margins
{
public:
    explicit Margins();
    explicit Margins(qreal left, qreal top, qreal right, qreal bottom);
    ~Margins();
    qreal left() const;
    qreal top() const;
    qreal right() const;
    qreal bottom() const;
    void setLeft(qreal value);
    void setTop(qreal value);
    void setRight(qreal value);
    void setBottom(qreal value);
    void setCoords(qreal x1, qreal y1, qreal x2, qreal y2);

    bool operator ==(const Margins & margins);

private:
    qreal m_left;
    qreal m_top;
    qreal m_right;
    qreal m_bottom;
};

QDataStream &operator<<(QDataStream &s, const Margins &p);
QDataStream &operator>>(QDataStream &s, Margins &p);

class CUTEREPORT_EXPORTS Units
{
public:
    explicit Units();
    explicit Units(qreal mm, qreal inch, int pixel);
    ~Units();
    void setMM(qreal value);
    void setInch(qreal value);
    void setPixel(int value);
    qreal mm() const;
    qreal inch() const;
    int pixel() const;

private:
    qreal m_mm;
    qreal m_inch;
    int m_pixel;
};


/** synced with QPrinter::Unit */
enum Unit {
    Millimeter = 0,
    Point = 1,
    Inch = 2,
    Pica = 3,
    Didot = 4,
    Cicero = 5,
    Pixel = 6,
    UnitNotDefined = 7,
    UNIT_MAX = 8
};

enum FrameBorderSide {
    DrawLeft = 1, /**< Draw left frame*/
    DrawRight = 2, /**< Draw right frame*/
    DrawTop = 4, /**< Draw top frame*/
    DrawBottom = 8 /**< Draw bottom frame*/
};


//struct ItemFrame
//{
//    FrameBorderSide borderSides;
//    QPen pen;
//    QBrush brush;
//};


enum ItemType {NotDefinedItemType = 0, ItemItem = 1, BandItem = 2};
enum ModuleType {UnknownModuleType, ItemModule, PageModule, DatasetModule, StorageModule, RendererModule, PrinterModule, FormModule, ExportModule};
enum StorageSyncStatus {StorageSyncSuccessful = 1, StorageSyncFailed = 2, StorageSyncInProgress = 3};
enum Orientation { Portrait = 0, Landscape = 1 };

static const int ItemViewItemPointer = 0;

enum LogLevel {
    LogUndefined = 0,
    LogDebug = 1,
    LogInfo = 2,
    LogError = 3,
    LogWarning = 4,
    LogCritical = 5,
    LogMetric = 6,
    LOGLEVELMAXVALUE = 7
};


enum MessageLevel {
    MessageUndefined = 0,
    MessageNormal = 1,
    MessageWarning = 2,
    MessageError = 4
};


enum MetricType {
    MetricGeometry = 1,
    MetricPosition = 2,
    MetricClick = 4,
    MetricPress = 8,
    MetricRelease = 16,
    MetricDrag = 32,
    MetricDrop = 64
};


enum RenderingType {
    RenderingTemplate = 0,
    RenderingReport = 1
};

enum RenderingStage {
    RenderingNewPage = 1,
    RenderingNormal = 2
};

enum ResultValue {
    ResultOk = 0,
    ResultNotDefined = 1,
    ResultWarnings = 2,
    ResultError = 3
};

typedef QList<CuteReport::BaseItemInterface*> BaseItemsList;
typedef BaseItemsList* BaseItemsListPointer;
typedef QList<CuteReport::BandInterface*> BandsList;
typedef BandsList* BandsListPointer;

enum StorageFileType {FileUnknown, FileDir, FileReport, FileTemplate, FileImage};

struct StorageObjectInfo {
    explicit StorageObjectInfo(): size(0), type(FileUnknown){}
    QUrl url;
    qint64 size;
    StorageFileType type;
};


struct StorageListResult {
    qint32 requestId;
    QList<StorageObjectInfo> objects;
    QString error;
};

struct StorageProgress {
    qint32 requestId;
    int max;
    int current;
    QString message;
};


enum ItemDataRole {
    SyntaxRole =  Qt::UserRole + 1,
    ExampleRole = Qt::UserRole + 3,
    TemplateRole = Qt::UserRole + 4,
    DescriptionRole = Qt::UserRole + 5,
    ReturnValueCountRole = Qt::UserRole + 6,
    ReturnValue1Role = Qt::UserRole + 7,
    ReturnValue2Role = Qt::UserRole + 8,
    ReturnValue3Role = Qt::UserRole + 9,
    ReturnValue4Role = Qt::UserRole + 10,
    ReturnValue5Role = Qt::UserRole + 11,
    ConstructorCountRole = Qt::UserRole + 20,
    Constructor1Role = Qt::UserRole + 21,
    Constructor2Role = Qt::UserRole + 22,
    Constructor3Role = Qt::UserRole + 23,
    Constructor4Role = Qt::UserRole + 24,
    ParameterCountRole = Qt::UserRole + 30,
    Parameter1Role = Qt::UserRole + 40,
    Parameter2Role = Qt::UserRole + 41,
    Parameter3Role = Qt::UserRole + 42,
    Parameter4Role = Qt::UserRole + 43,
    Parameter5Role = Qt::UserRole + 44,
};


enum Events {
    PageMouseBlock = QEvent::User + 100,
    PageMouseUnblock = QEvent::User + 101,
};


} //namespace

Q_DECLARE_METATYPE(CuteReport::Unit)
Q_DECLARE_METATYPE(CuteReport::Units)
Q_DECLARE_METATYPE(CuteReport::Margins)
Q_DECLARE_METATYPE(CuteReport::LogLevel)
Q_DECLARE_METATYPE(CuteReport::MessageLevel)
Q_DECLARE_METATYPE(CuteReport::MetricType)
Q_DECLARE_METATYPE(CuteReport::RenderingType)
Q_DECLARE_METATYPE(CuteReport::ResultValue)
Q_DECLARE_METATYPE(CuteReport::StorageFileType)
Q_DECLARE_METATYPE(CuteReport::StorageObjectInfo)
Q_DECLARE_METATYPE(CuteReport::StorageListResult)
Q_DECLARE_METATYPE(CuteReport::StorageProgress)


#endif // TYPES_H
