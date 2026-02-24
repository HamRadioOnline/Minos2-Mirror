#ifndef TXKEYERFORMBASE_H
#define TXKEYERFORMBASE_H


#include <QWidget>
#include <QString>

class TxKeyerFormBase : public QWidget
{
    Q_OBJECT

public:
    explicit TxKeyerFormBase(QWidget *parent = nullptr)
        : QWidget(parent) {}

    virtual ~TxKeyerFormBase() = default;

    // Keyer indicators
    virtual void setKeyerAvailableIndicatorOnOff(bool) {}
    virtual void setKeyerAvailableVisible(bool) {}

    virtual void setRepeatIndicatorOnOff(bool) {}
    virtual void setRepeatIndicatorVisible(bool) {}

    virtual void setEOMLabelText(const QString&) {}
    virtual void clearEOMLabelText() {}

    // PTT indicators
    virtual void setPttEnabledIndicatorOnOff(bool) {}
    virtual void setTxStatusIndicatorOnOff(bool) {}

    virtual void setPttTypeText(const QString&) {}
    virtual void clearPttTypeText() {}

    virtual void setStoredMessagePlayingDisplay(const QString) {}

    virtual void clearStoredMessagePlayingDisplay() {}

    virtual void selectAllText() {}

    virtual void setCwFreeTextIndicatorOnOff(bool) {};
    virtual void setCwSliderValue(int) {}

    virtual QString getCwEntryText() {return {};}

signals:

    void sendWpmToPcCwkeyer(int wpm);
    void cwEntryReturnPressed();
};
#endif // TXKEYERFORMBASE_H
