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
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"
#include "globals.h"

namespace CuteReport
{

inline qreal mm2pixel(qreal mm, qreal dpi) { return ((mm * dpi) / 25.4); }
inline qreal pixel2mm(qreal pixels, qreal dpi) { return (pixels * 25.4) / dpi; }
inline qreal inch2pixel(qreal inches, qreal dpi) { return (inches * dpi);}
inline qreal pixel2inch(qreal pixels, qreal dpi) { return pixels/dpi; }

inline qreal mm2pixel_trunc(qreal mm, qreal dpi) { return (int)((mm * dpi) / 25.4); }
inline qreal mm2pixel_ceil(qreal mm, qreal dpi) { return ceil((mm * dpi) / 25.4); }
//inline qreal mm2pixel_round(qreal mm, qreal dpi) { return round((mm * dpi) / 25.4); }

inline qreal mm2inch(qreal mm) { return mm/25.4; }
inline qreal inch2mm(qreal inch) { return inch * 25.4; }

CUTEREPORT_EXPORTS QRectF  convertUnit(const QRectF & rect, Unit unitIn, Unit unitOut, int dpi);
CUTEREPORT_EXPORTS QPointF convertUnit(const QPointF & point, Unit unitIn, Unit unitOut, int dpi);
CUTEREPORT_EXPORTS QSizeF  convertUnit(const QSizeF & size, Unit unitIn, Unit unitOut, int dpi);
CUTEREPORT_EXPORTS qreal   convertUnit(const qreal & value, Unit unitIn, Unit unitOut, int dpi);

//CUTEREPORT_EXPORTS QRectF  convertUnit2(const QRectF & rect, Unit unitIn, Unit unitOut, int dpi);
//CUTEREPORT_EXPORTS QPointF convertUnit2(const QPointF & point, Unit unitIn, Unit unitOut, int dpi);

CUTEREPORT_EXPORTS CuteReport::Unit    unitFromString(const QString & str);
CUTEREPORT_EXPORTS QString             unitToFullString(CuteReport::Unit unit);
CUTEREPORT_EXPORTS QString             unitToShortString(CuteReport::Unit unit);

CUTEREPORT_EXPORTS bool typeIsNumber(QVariant::Type type);

CUTEREPORT_EXPORTS QSet<QString> findVariables(const QString & str);
CUTEREPORT_EXPORTS QString setVariablesValue(const QString & str, const QVariantHash & values);
CUTEREPORT_EXPORTS bool isStringValued(const QString & str, const QVariantHash & values, QStringList * missedVariables = 0);

} //namespace
#endif // FUNCTIONS_H
