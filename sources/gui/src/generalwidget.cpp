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

#include "generalwidget.h"
#include "ui_generalwidget.h"

#include <QKeyEvent>


GeneralWidget::GeneralWidget(QWidget *parent, const QMap<QString, QString> settings)
    : QWidget(parent),
      ui(new Ui::GeneralWidget)
{
    ui->setupUi(this);
    connectionType = ui->comboBox_connection;
    ifaceDirectory = new QDir(settings[QString("IFACE_DIR")]);
    profileDirectory = new QDir(settings[QString("PROFILE_DIR")]);
    createActions();
    clear();
}


GeneralWidget::~GeneralWidget()
{
    delete connectionType;
    delete ifaceDirectory;
    delete profileDirectory;
    delete ui;
}


void GeneralWidget::clear()
{
    ui->lineEdit_description->setText(QString("Generated by Netctl GUI"));
    ui->comboBox_connection->setCurrentIndex(0);
    ui->comboBox_interface->clear();
    ui->comboBox_interface->addItems(ifaceDirectory->entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    ui->comboBox_bindto->clear();
    ui->comboBox_bindto->addItems(ifaceDirectory->entryList(QDir::Dirs | QDir::NoDotAndDotDot));
    ui->listWidget_bindto->setCurrentRow(-1);
    ui->listWidget_bindto->clear();
    ui->comboBox_after->clear();
    ui->comboBox_after->addItems(profileDirectory->entryList(QDir::Files));
    ui->listWidget_after->setCurrentRow(-1);
    ui->listWidget_after->clear();
    ui->lineEdit_execUpPost->clear();
    ui->lineEdit_execDownPre->clear();
    ui->checkBox_forceConnect->setCheckState(Qt::Unchecked);
    ui->checkBox_debug->setCheckState(Qt::Unchecked);

    ui->pushButton_generalAdvanced->setChecked(false);
    showAdvanced();
}


void GeneralWidget::createActions()
{
    connect(ui->pushButton_generalAdvanced, SIGNAL(clicked(bool)), this, SLOT(showAdvanced()));
    connect(ui->pushButton_after, SIGNAL(clicked(bool)), this, SLOT(addAfter()));
    connect(ui->comboBox_after->lineEdit(), SIGNAL(returnPressed()), this, SLOT(addAfter()));
    connect(ui->pushButton_bindto, SIGNAL(clicked(bool)), this, SLOT(addBindTo()));
    connect(ui->comboBox_bindto->lineEdit(), SIGNAL(returnPressed()), this, SLOT(addBindTo()));
}


void GeneralWidget::addAfter()
{
    ui->listWidget_after->addItem(ui->comboBox_after->currentText());
}


void GeneralWidget::addBindTo()
{
    ui->listWidget_bindto->addItem(ui->comboBox_bindto->currentText());
}


void GeneralWidget::keyPressEvent(QKeyEvent *pressedKey)
{
    if (pressedKey->key() == Qt::Key_Delete) {
        if (ui->listWidget_bindto->hasFocus() &&
            (ui->listWidget_bindto->currentItem() != nullptr))
            delete ui->listWidget_bindto->currentItem();
        else if (ui->listWidget_after->hasFocus() &&
                 (ui->listWidget_after->currentItem() != nullptr))
            delete ui->listWidget_after->currentItem();
    }
}


void GeneralWidget::showAdvanced()
{
    if (ui->pushButton_generalAdvanced->isChecked()) {
        ui->widget_generalAdvanced->setHidden(false);
        ui->pushButton_generalAdvanced->setText(QApplication::translate("GeneralWidget", "Hide advanced"));
    } else {
        ui->widget_generalAdvanced->setHidden(true);
        ui->pushButton_generalAdvanced->setText(QApplication::translate("GeneralWidget", "Show advanced"));
    }
}


QMap<QString, QString> GeneralWidget::getSettings()
{
    QMap<QString, QString> settings;

    if (isOk() != 0) return settings;

    settings[QString("Description")] = QString("'%1'").arg(ui->lineEdit_description->text());
    settings[QString("Connection")] = ui->comboBox_connection->currentText();
    settings[QString("Interface")] = ui->comboBox_interface->currentText();
    if (ui->listWidget_bindto->count() != 0) {
        QStringList interfaces;
        for (int i=0; i<ui->listWidget_bindto->count(); i++)
            interfaces.append(ui->listWidget_bindto->item(i)->text());
        settings[QString("BindsToInterfaces")] = interfaces.join(QChar(' '));
    }
    if (ui->listWidget_after->count() != 0) {
        QStringList profiles;
        for (int i=0; i<ui->listWidget_after->count(); i++)
            profiles.append(ui->listWidget_after->item(i)->text());
        settings[QString("After")] = profiles.join(QChar(' '));
    }
    if (!ui->lineEdit_execUpPost->text().isEmpty())
        settings[QString("ExecUpPost")] = QString("'%1'").arg(ui->lineEdit_execUpPost->text());
    if (!ui->lineEdit_execDownPre->text().isEmpty())
        settings[QString("ExecDownPre")] = QString("'%1'").arg(ui->lineEdit_execDownPre->text());
    if (ui->checkBox_forceConnect->checkState() == Qt::Checked)
        settings[QString("ForceConnect")] = QString("yes");
    if (ui->checkBox_debug->checkState() == Qt::Checked)
        settings[QString("NETCTL_DEBUG")] = QString("yes");

    return settings;
}


int GeneralWidget::isOk()
{
    // bind interfaces is not set
    if ((ui->comboBox_connection->currentText() == QString("bond")) ||
        (ui->comboBox_connection->currentText() == QString("bridge")) ||
        (ui->comboBox_connection->currentText() == QString("vlan")) ||
        (ui->comboBox_connection->currentText() == QString("macvlan")) ||
        (ui->comboBox_connection->currentText() == QString("openvswitch")))
        if (ui->listWidget_bindto->count() == 0)
            return 1;
    // empty description
    if (ui->lineEdit_description->text().isEmpty())
        return 2;
    // all fine
    return 0;
}


void GeneralWidget::setSettings(const QMap<QString, QString> settings)
{
    clear();

    if (settings.contains(QString("Description")))
        ui->lineEdit_description->setText(settings[QString("Description")]);
    if (settings.contains(QString("Connection"))) {
        int index = ui->comboBox_connection->findText(settings[QString("Connection")]);
        ui->comboBox_connection->setCurrentIndex(index);
    }
    if (settings.contains(QString("Interface"))) {
        ui->comboBox_interface->addItem(settings[QString("Interface")]);
        ui->comboBox_interface->setCurrentIndex(ui->comboBox_interface->count() - 1);
    }
    if (settings.contains(QString("BindsToInterfaces")))
        ui->listWidget_bindto->addItems(settings[QString("BindsToInterfaces")].split(QChar('\n')));
    if (settings.contains(QString("After")))
        ui->listWidget_after->addItems(settings[QString("After")].split(QChar('\n')));
    if (settings.contains(QString("ExecUpPost")))
        ui->lineEdit_execUpPost->setText(settings[QString("ExecUpPost")]);
    if (settings.contains(QString("ExecDownPre")))
        ui->lineEdit_execDownPre->setText(settings[QString("ExecDownPre")]);
    if (settings.contains(QString("ForceConnect")))
        if (settings[QString("ForceConnect")] == QString("yes"))
            ui->checkBox_forceConnect->setCheckState(Qt::Checked);
    if (settings.contains(QString("NETCTL_DEBUG")))
        if (settings[QString("NETCTL_DEBUG")] == QString("yes"))
            ui->checkBox_debug->setCheckState(Qt::Checked);
}
