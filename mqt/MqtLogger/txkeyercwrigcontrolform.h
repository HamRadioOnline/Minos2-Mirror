#ifndef TXKEYERCWRIGCONTROLFORM_H
#define TXKEYERCWRIGCONTROLFORM_H

#include <QWidget>
#include "cwentrywidget.h"
#include "keyerWidgetFactory.h"
#include "txKeyerFormBase.h"


class TxKeyerCwRigControlForm : public TxKeyerFormBase
{
    Q_OBJECT

public:
    explicit TxKeyerCwRigControlForm(QWidget *parent = nullptr);
    ~TxKeyerCwRigControlForm();

    void setCwFreeTextIndicatorOnOff(bool on) override;


    QString getCwEntryText() override;

    void setKeyerAvailableIndicatorOnOff(bool on) override;
    void setRepeatIndicatorOnOff(bool on) override;
    void setEOMLabelText(const QString &text) override;
    void clearEOMLabelText() override;


    void setPttEnabledIndicatorOnOff(const bool on) override;
    void setTxStatusIndicatorOnOff(const bool on) override;
    void setPttTypeText(const QString &text) override;
    void clearPttTypeText() override;
    void setStoredMessagePlayingDisplay(const QString msg) override;

    void clearStoredMessagePlayingDisplay() override;

    //void setErrorMessageDisplayText(const QString errormsg)  override;

    void selectAllText()  override;


private:

    QHBoxLayout *indicatorLayout;
    KeyerIndicators indicators;


    //QHBoxLayout *keyerErrorMessageLayout;
    //KeyerErrorMessageWidget *keyerErrorMessageDisplay;


    CwMessagePlayingRow cwMessagePlayingRow;


    QHBoxLayout *cwEntryLayout;
    CwEntryWidget *cwEntry;


};

#endif // TXKEYERCWRIGCONTROLFORM_H
