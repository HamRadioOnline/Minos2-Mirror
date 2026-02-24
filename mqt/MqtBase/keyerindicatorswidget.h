#ifndef KEYERINDICATORSWIDGET_H
#define KEYERINDICATORSWIDGET_H

#include <QFrame>
#include <QObject>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include  <QHBoxLayout>
class KeyerIndicatorsWidget : public QFrame
{
      Q_OBJECT

public:

    KeyerIndicatorsWidget(QWidget *parent = nullptr);


    void setKeyerAvailableIndicatorOnOff(bool on);
    void setKeyerAvailableVisible(bool visible);
    void setRepeatIndicatorOnOff(bool on);
    void setRepeatIndicatorVisible(bool visible);
    void setEOMLabelText(const QString text);

    void clearEOMLabelText();
    void setEOMLabelTextVisible(bool visible);
    void setKeyerAvailableSetToolTip(QString text);

private:

    QGroupBox *indicatorGrpBox;

    QHBoxLayout *layout;
    QPushButton *keyerAvailIndicator;
    QPushButton *repeatIndicator;
    QLabel *eomText;
};

#endif // KEYERINDICATORSWIDGET_H
