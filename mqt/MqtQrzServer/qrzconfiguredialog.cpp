/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//                      Qrz Server
// Copyright        (c) D. G. Balharrie M0DGB/G8FKH 2021
//
// Interprocess Control Logic
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2017
//
//
//
/////////////////////////////////////////////////////////////////////////////




#include "qrzconfiguredialog.h"
#include "ui_qrzconfiguredialog.h"


QrzConfigureDialog::QrzConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QrzConfigureDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->qrzLogonCallsignLineEdit->setValidator(&ucValidator);
    ui->qrzCqLogonCallsignLineEdit->setValidator(&ucValidator);

    QStringList listOfServices = {"", "QRZ, QRZCQ"};
    ui->callsignServiceComboBox->addItems(listOfServices);
}

QrzConfigureDialog::~QrzConfigureDialog()
{
    delete ui;
}

int QrzConfigureDialog::exec()
{

    ui->callsignServiceComboBox->setCurrentText(getServiceName());

    ui->qrzLogonCallsignLineEdit->setText(getQrzLogCallsign());
    ui->qrzPasswordLineEdit->setText(getQrzLogPassword());
    ui->qrzCacheAge->setValue(getQrzCacheAge());

    ui->qrzCqLogonCallsignLineEdit->setText(getQrzCqLogCallsign());
    ui->qrzCqPasswordLineEdit->setText(getQrzCqLogPassword());
    ui->qrzCqCacheAge->setValue(getQrzCqCacheAge());

    return QDialog::exec();
}

void QrzConfigureDialog::on_resetDBButton_clicked()
{
    resetDB = true;
    accept();
}


void QrzConfigureDialog::on_OKButton_clicked()
{

    setServiceName(ui->callsignServiceComboBox->currentText());

    setQrzLogCallsign(ui->qrzLogonCallsignLineEdit->text().trimmed());
    setQrzLogCallsign(ui->qrzLogonCallsignLineEdit->text().trimmed());

    setQrzCqLogCallsign(ui->qrzCqLogonCallsignLineEdit->text().trimmed());
    setQrzCqLogCallsign(ui->qrzCqLogonCallsignLineEdit->text().trimmed());

    setQrzCacheAge(ui->qrzCacheAge->value());
    setQrzCqCacheAge(ui->qrzCqCacheAge->value());

    accept();
}


void QrzConfigureDialog::on_cancelButton_clicked()
{
    reject();
}

