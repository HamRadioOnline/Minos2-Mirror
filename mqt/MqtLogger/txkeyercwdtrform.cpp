#include <QDebug>
#include "txkeyercwdtrform.h"
//#include "ui_txkeyercwdtrform.h"
#include "cwspeedcontrol.h"
#include "cwentrywidget.h"
#include "keyerWidgetFactory.h"

TxKeyerCwDtrForm::TxKeyerCwDtrForm(QWidget *parent)
    : TxKeyerFormBase(parent)
{


    // create cw dtr keyer ui

    indicators = KeyerWidgetFactory::createIndicators(this);
    indicatorLayout = KeyerWidgetFactory::createRowLayout();
    indicatorLayout->addWidget(indicators.keyerIndicators);
    indicatorLayout->addWidget(indicators.pttIndicator);
    indicatorLayout->addStretch();

    cwSpeedSlider = KeyerWidgetFactory::createCwSpeedControl(this);
    cwSliderLayout = KeyerWidgetFactory::createRowLayout();
    cwSliderLayout->addWidget(cwSpeedSlider);

    cwMessagePlayingRow = KeyerWidgetFactory::createCwMessagePlayingRow(this);


    cwEntry = KeyerWidgetFactory::createCwEntry(this);
    cwEntryLayout = KeyerWidgetFactory::createRowLayout();
    cwEntryLayout->addWidget(cwEntry);

    QVBoxLayout *mainLayout = KeyerWidgetFactory::createMainLayout(this);
    mainLayout->addLayout(indicatorLayout);
    //mainLayout->addLayout(keyerErrorMessageLayout);
    mainLayout->addLayout(cwSliderLayout);
    mainLayout->addLayout(cwMessagePlayingRow.layout);
    mainLayout->addLayout(cwEntryLayout);

    setLayout(mainLayout);



    connect(cwEntry, &CwEntryWidget::cwEntryReturnPressed, this,
             [this](){ emit cwEntryReturnPressed(); });

    connect(cwSpeedSlider, &CwSpeedControl::cwSpeedChanged, this,
            [this](int wpm){ emit sendWpmToPcCwkeyer(wpm); });

    qDebug() << "Form:" << this->objectName()
             << "sizeHint =" << this->sizeHint()
             << "minSizeHint =" << this->minimumSizeHint();

}

TxKeyerCwDtrForm::~TxKeyerCwDtrForm()
{

}



void TxKeyerCwDtrForm::setCwSliderValue(int value)
{
    cwSpeedSlider->setValue(value);
}


QString TxKeyerCwDtrForm::getCwEntryText()
{
    return cwEntry->getCwEntryText();
}

void TxKeyerCwDtrForm::selectAllText()
{
    cwEntry->selectAllText();
}

void TxKeyerCwDtrForm::setCwFreeTextIndicatorOnOff(bool on)
{
    cwEntry->setCwFreeTextIndicatorOnOff(on);
}


void TxKeyerCwDtrForm::setKeyerAvailableIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setKeyerAvailableIndicatorOnOff(on);
}

void TxKeyerCwDtrForm::setRepeatIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setRepeatIndicatorOnOff(on);
}

void TxKeyerCwDtrForm::setEOMLabelText(const QString &text)
{
    indicators.keyerIndicators->setEOMLabelText(text);
}

void TxKeyerCwDtrForm::clearEOMLabelText()
{
    indicators.keyerIndicators->clearEOMLabelText();
}

void TxKeyerCwDtrForm::setPttEnabledIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setPttEnabledIndicatorOnOff(on);
}


void TxKeyerCwDtrForm::setTxStatusIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setTxStatusIndicatorOnOff(on);
}

void TxKeyerCwDtrForm::setPttTypeText(const QString &text)
{
    indicators.pttIndicator->setPttTypeText(text);
}

void TxKeyerCwDtrForm::clearPttTypeText()
{
    indicators.pttIndicator->clearPttTypeText();
}

void TxKeyerCwDtrForm::setStoredMessagePlayingDisplay(const QString msg)
{
    cwMessagePlayingRow.display->setText(msg);
}

void TxKeyerCwDtrForm::clearStoredMessagePlayingDisplay()
{
    cwMessagePlayingRow.display->clear();
}
