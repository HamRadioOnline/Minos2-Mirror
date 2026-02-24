#ifndef TXKEYERDIGITALMODESFORM_H
#define TXKEYERDIGITALMODESFORM_H

#include <QWidget>
#include "txKeyerFormBase.h"

class TxKeyerDigitalModesForm : public TxKeyerFormBase
{
    Q_OBJECT

public:
    explicit TxKeyerDigitalModesForm(QWidget *parent = nullptr);
    ~TxKeyerDigitalModesForm();

    void setKeyerAvailableIndicatorOnOff(bool on) override;
    void setKeyerAvailableVisible(bool visible) override;

    void setRepeatIndicatorOnOff(bool on) override;
    void setRepeatIndicatorVisible(bool visible) override;

    void setEOMLabelText(const QString &text) override;
    void clearEOMLabelText() override;

    void setPttEnabledIndicatorOnOff(bool on) override;
    void setTxStatusIndicatorOnOff(bool on) override;

    void setPttTypeText(const QString &text) override;
    void clearPttTypeText() override;

private:

};

#endif // TXKEYERDIGITALMODESFORM_H
