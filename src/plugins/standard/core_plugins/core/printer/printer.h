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
#ifndef PRINTER_H
#define PRINTER_H

#include "printerinterface.h"
#include "globals.h"
#include "types.h"

namespace CuteReport {
}


class Printer : public CuteReport::PrinterInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "CuteReport.PrinterInterface/1.0")
#endif
    Q_INTERFACES(CuteReport::PrinterInterface)

    Q_PROPERTY(bool showDialog READ isShowDialog WRITE setShowDialog NOTIFY showDialogChanged)
    Q_PROPERTY(bool scaleToFit READ scaleToFit WRITE setScaleToFit NOTIFY scaleToFitChanged)
    Q_PROPERTY(bool keepAspectRatio READ keepAspectRatio WRITE setKeepAspectRatio NOTIFY keepAspectRatioChanged)
    Q_PROPERTY(bool doubleSided READ doubleSided WRITE setDoubleSided NOTIFY doubleSidedChanged)
    Q_PROPERTY(bool useDuplexerIfExists READ useDuplexer WRITE setUseDuplexer NOTIFY useDuplexerChanged)
//    Q_PROPERTY(CuteReport::Margins duplexerMargins READ duplexerMargins WRITE setDuplexerMargins NOTIFY duplexerMarginsChanged)

public:
    explicit Printer(QObject *parent = 0);
    ~Printer();

    virtual QString moduleShortName() const {return QString("Printer");}
    virtual QString suitName() const { return "Standard"; }

    CuteReport::PrinterHelperInterface * helper();
    virtual PrinterInterface * createInstance( QObject * parent=0) const;
    virtual PrinterInterface * clone() const;

    virtual bool print(CuteReport::ReportInterface * report);
    virtual QString lastError();

    bool isShowDialog() const;
    void setShowDialog(bool b);

    bool scaleToFit();
    void setScaleToFit(bool b);

    bool keepAspectRatio();
    void setKeepAspectRatio(bool b);

    bool doubleSided();
    void setDoubleSided(bool b);

    bool useDuplexer();
    void setUseDuplexer(bool b);

    CuteReport::Margins duplexerMargins();
    void setDuplexerMargins(CuteReport::Margins margins);

    virtual QString _current_property_description() const;

signals:
    void showDialogChanged(bool);
    void scaleToFitChanged(bool);
    void keepAspectRatioChanged(bool);
    void doubleSidedChanged(bool);
    void useDuplexerChanged(bool);
    void duplexerMarginsChanged(CuteReport::Margins);

private:
    bool process_(CuteReport::ReportInterface * report, bool forceShowDialog = false);

private:
    explicit Printer(const Printer &dd, QObject *parent);

    QString m_lastError;
    bool m_showDialog;
    bool m_scaleToFit;
    bool m_keepAspectRatio;
    bool m_doubleSided;
    bool m_useDuplexer;
    CuteReport::Margins m_duplexerMargins;
};


#endif // PRINTER_H
