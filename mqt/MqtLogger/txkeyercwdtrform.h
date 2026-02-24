#ifndef TXKEYERCWDTRFORM_H
#define TXKEYERCWDTRFORM_H

#include <QWidget>
#include <QHBoxLayout>
#include "cwentrywidget.h"
#include "keyerWidgetFactory.h"
#include "txKeyerFormBase.h"




class TxKeyerCwDtrForm : public TxKeyerFormBase
{
    Q_OBJECT

public:
    explicit TxKeyerCwDtrForm(QWidget *parent = nullptr);
    ~TxKeyerCwDtrForm();

    void setKeyerAvailableIndicatorOnOff(bool on)  override;
    void setRepeatIndicatorOnOff(bool on)  override;
    void setEOMLabelText(const QString &text)  override;
    void clearEOMLabelText()  override;


    void setPttEnabledIndicatorOnOff(const bool on)  override;
    void setTxStatusIndicatorOnOff(const bool on)  override;
    void setPttTypeText(const QString &text)  override;
    void clearPttTypeText()  override;
    void setStoredMessagePlayingDisplay(const QString msg)  override;

    void clearStoredMessagePlayingDisplay()  override;

   void selectAllText() override;

    void setCwFreeTextIndicatorOnOff(bool on) override;
    void setCwSliderValue(int value)  override;

    QString getCwEntryText() override;


private:

    QHBoxLayout *indicatorLayout;
    KeyerIndicators indicators;

    //QHBoxLayout *keyerErrorMessageLayout;
    //KeyerErrorMessageWidget *keyerErrorMessageDisplay;


    QHBoxLayout *cwSliderLayout;
    CwSpeedControl *cwSpeedSlider = nullptr;


    CwMessagePlayingRow cwMessagePlayingRow;


    QHBoxLayout *cwEntryLayout;
    CwEntryWidget *cwEntry;



};

#endif // TXKEYERCWDTRFORM_H
