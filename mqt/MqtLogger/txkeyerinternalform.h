#ifndef TXKEYERINTERNALFORM_H
#define TXKEYERINTERNALFORM_H

#include "txKeyerFormBase.h"

class TxKeyerInternalForm : public TxKeyerFormBase
{

    Q_OBJECT

public:
    explicit TxKeyerInternalForm(QWidget *parent = nullptr);
    ~TxKeyerInternalForm();

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

};

#endif // TXKEYERINTERNALFORM_H
