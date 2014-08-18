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
#ifndef QTSCRIPTSHELL_QIMAGE_H
#define QTSCRIPTSHELL_QIMAGE_H

#include <qimage.h>

#include <QtScript/qscriptvalue.h>

class QtScriptShell_QImage : public QImage
{
public:
    QtScriptShell_QImage();
    QtScriptShell_QImage(const QImage&  arg__1);
    QtScriptShell_QImage(const QSize&  size, QImage::Format  format);
    QtScriptShell_QImage(const QString&  fileName, const char*  format = 0);
    QtScriptShell_QImage(const char*  fileName, const char*  format = 0);
    QtScriptShell_QImage(const char**  xpm);
    QtScriptShell_QImage(const uchar*  data, int  width, int  height, QImage::Format  format);
    QtScriptShell_QImage(const uchar*  data, int  width, int  height, int  bytesPerLine, QImage::Format  format);
    QtScriptShell_QImage(int  width, int  height, QImage::Format  format);
    QtScriptShell_QImage(uchar*  data, int  width, int  height, QImage::Format  format);
    QtScriptShell_QImage(uchar*  data, int  width, int  height, int  bytesPerLine, QImage::Format  format);
    ~QtScriptShell_QImage();

    int  devType() const;
    int  metric(QPaintDevice::PaintDeviceMetric  metric) const;
    QPaintEngine*  paintEngine() const;

    QScriptValue __qtscript_self;
};

#endif // QTSCRIPTSHELL_QIMAGE_H
