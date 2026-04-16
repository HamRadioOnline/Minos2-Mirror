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



#ifndef QRZCONFIGUREDIALOG_H
#define QRZCONFIGUREDIALOG_H

#include <QDialog>
#include "cutils.h"

namespace Ui {
class QrzConfigureDialog;
}

class QrzConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QrzConfigureDialog(QWidget *parent = nullptr);
    ~QrzConfigureDialog();

    void setServiceName(QString serviceName_){serviceName = serviceName_;}
    QString getServiceName(){return serviceName;}

    void setQrzLogCallsign(QString callsign){qrzLogCallsign = callsign;}
    QString getQrzLogCallsign(){return qrzLogCallsign;}

    void setQrzLogPassword(QString password){qrzLogPassword = password;}
    QString getQrzLogPassword(){return qrzLogPassword;}

    void setQrzCacheAge(int age){qrzCacheAge = age;}
    int getQrzCacheAge(){return qrzCacheAge;}

    void setQrzCqLogCallsign(QString callsign){qrzCqLogCallsign = callsign;}
    QString getQrzCqLogCallsign(){return qrzCqLogCallsign;}

    void setQrzCqLogPassword(QString password){qrzCqLogPassword = password;}
    QString getQrzCqLogPassword(){return qrzCqLogPassword;}

    void setQrzCqCacheAge(int age){qrzCqCacheAge = age;}
    int getQrzCqCacheAge(){return qrzCqCacheAge;}

    bool resetDB = false;

public Q_SLOTS:
        virtual int exec() override;

private slots:

    void on_resetDBButton_clicked();

    void on_OKButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::QrzConfigureDialog *ui;
    UpperCaseValidator ucValidator;

    QString serviceName;

    QString qrzLogCallsign;
    QString qrzLogPassword;
    int qrzCacheAge = 0;

    QString qrzCqLogCallsign;
    QString qrzCqLogPassword;
    int qrzCqCacheAge = 0;

    void saveSettings();
};

#endif // QRZCONFIGUREDIALOG_H
