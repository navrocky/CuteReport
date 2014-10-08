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
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#include "optionsdialogrendererpage.h"
#include "ui_optionsdialogrendererpage.h"
#include "rendererinterface.h"
#include "reportcore.h"
#include "core.h"

OptionsDialogRendererPage::OptionsDialogRendererPage(CuteDesigner::Core *core) :
    OptionsDialogPageInterface(core),
    ui(new Ui::OptionsDialogRendererPage)
  , m_core(core)
  , m_currentModule(0)
{
    ui->setupUi(this);

    ui->cbModules->addItems(m_core->reportCore()->moduleNames(CuteReport::RendererModule));

    CuteReport::RendererInterface* defaultModule  = m_core->reportCore()->defaultRenderer();

    if (defaultModule) {
        ui->cbModules->setCurrentIndex(ui->cbModules->findText(defaultModule->moduleFullName()));
        currentModuleChanged(defaultModule->moduleFullName());
    }

    connect(ui->cbModules, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentModuleChanged(QString)));
}

OptionsDialogRendererPage::~OptionsDialogRendererPage()
{
    delete ui;
}


void OptionsDialogRendererPage::activate()
{
}


void OptionsDialogRendererPage::deactivate()
{
    if (m_currentModule) {
        m_core->settings()->setValue("CuteReport/PrimaryRenderer", m_currentModule->moduleFullName());
//        m_currentModule->helper()->save();
        m_core->settings()->setValue(QString("CuteReport/Renderer_%1_options").arg(m_currentModule->moduleFullName()), m_core->reportCore()->moduleOptionsStr(m_currentModule)) ;
    }
}


QListWidgetItem * OptionsDialogRendererPage::createButton(QListWidget * listWidget)
{
    QListWidgetItem *configButton = new QListWidgetItem(listWidget);
    configButton->setIcon(QIcon(":/images/renderer_96x96.png"));
    configButton->setText(tr("Renderer"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    return configButton;
}


void OptionsDialogRendererPage::currentModuleChanged(QString moduleName)
{
    if (m_currentModule) {
        //m_currentModule->helper()->save();
        m_core->settings()->setValue(QString("CuteReport/Renderer_%1_options").arg(m_currentModule->moduleFullName()), m_core->reportCore()->moduleOptionsStr(m_currentModule)) ;
//        delete m_currentModule->helper();
    }

    CuteReport::RendererInterface* module = m_core->reportCore()->rendererModule(moduleName);

    if (module) {
        m_core->reportCore()->setDefaultRenderer( moduleName );
        m_currentModule = m_core->reportCore()->defaultRenderer();
        m_core->reportCore()->setModuleOptionsStr( m_currentModule, m_core->settings()->value(QString("CuteReport/Renderer_%1_options").arg(m_currentModule->moduleFullName())).toString());
    }
}
