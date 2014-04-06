/***************************************************************************
 *   This file is part of netctl-gui                                       *
 *                                                                         *
 *   netctl-gui is free software: you can redistribute it and/or           *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   netctl-gui is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with netctl-gui. If not, see http://www.gnu.org/licenses/       *
 ***************************************************************************/

#include "passwdwidget.h"
#include "ui_passwdwidget.h"

#include "mainwindow.h"


PasswdWidget::PasswdWidget(MainWindow *wid)
    : QWidget(wid),
      parent(wid),
      ui(new Ui::PasswdWidget)
{
    ui->setupUi(this);
    createActions();
}


PasswdWidget::~PasswdWidget()
{
    delete ui;
}


// ESC press event
void PasswdWidget::keyPressEvent(QKeyEvent *pressedKey)
{
    if (pressedKey->key() == Qt::Key_Escape)
        cancel();
}


void PasswdWidget::createActions()
{
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(passwdApply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(passwdApply()));
}


void PasswdWidget::setFocusToLineEdit()
{
    ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
}


void PasswdWidget::cancel()
{
    hide();
    parent->updateTabs(2);
    this->~PasswdWidget();
}


void PasswdWidget::passwdApply()
{
    hide();
    return parent->connectToUnknownEssid(ui->lineEdit->text());
}
