#ifndef PTTINDICATORWIDGET_H
#define PTTINDICATORWIDGET_H

#include <QFrame>
#include <QObject>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include  <QHBoxLayout>

class PttIndicatorWidget : public QFrame
{
      Q_OBJECT

public:
    PttIndicatorWidget(QWidget *parent = nullptr);

    void setPttEnabledIndicatorOnOff(const bool on);
    void setTxStatusIndicatorOnOff(const bool on);
    void setPttTypeText(const QString text);
    void clearPttTypeText();

private:

    QGroupBox *pttGrpBox;

    QHBoxLayout *layout;
    QPushButton *pttEnabledIndicator;
    QPushButton *txStatusIndicator;
    QLabel *pttTypeText;

};

#endif // PTTINDICATORWIDGET_H
