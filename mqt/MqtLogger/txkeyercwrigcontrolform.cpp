#include <QDebug>
#include "txkeyercwrigcontrolform.h"



TxKeyerCwRigControlForm::TxKeyerCwRigControlForm(QWidget *parent)
    : TxKeyerFormBase(parent)
{


    // create ui

    indicators = KeyerWidgetFactory::createIndicators(this);
    indicatorLayout = KeyerWidgetFactory::createRowLayout();
    indicatorLayout->addWidget(indicators.keyerIndicators);
    indicatorLayout->addWidget(indicators.pttIndicator);
    indicatorLayout->addStretch();


    cwMessagePlayingRow = KeyerWidgetFactory::createCwMessagePlayingRow(nullptr);


    cwEntry = KeyerWidgetFactory::createCwEntry(this);
    cwEntryLayout = KeyerWidgetFactory::createRowLayout();
    cwEntryLayout->addWidget(cwEntry);

    QVBoxLayout *mainLayout = KeyerWidgetFactory::createMainLayout(nullptr);
    mainLayout->addLayout(indicatorLayout);
    //mainLayout->addLayout(keyerErrorMessageLayout);
    mainLayout->addLayout(cwMessagePlayingRow.layout);
    mainLayout->addLayout(cwEntryLayout);

    setLayout(mainLayout);

    qDebug() << "Form:" << this->objectName()
             << "sizeHint =" << this->sizeHint()
             << "minSizeHint =" << this->minimumSizeHint();

}

TxKeyerCwRigControlForm::~TxKeyerCwRigControlForm()
{

}




QString TxKeyerCwRigControlForm::getCwEntryText()
{
    return cwEntry->getCwEntryText();
}

void TxKeyerCwRigControlForm::selectAllText()
{
    cwEntry->selectAllText();
}

void TxKeyerCwRigControlForm::setCwFreeTextIndicatorOnOff(bool on)
{
    cwEntry->setCwFreeTextIndicatorOnOff(on);
}


void TxKeyerCwRigControlForm::setKeyerAvailableIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setKeyerAvailableIndicatorOnOff(on);
}

void TxKeyerCwRigControlForm::setRepeatIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setRepeatIndicatorOnOff(on);
}

void TxKeyerCwRigControlForm::setEOMLabelText(const QString &text)
{
    indicators.keyerIndicators->setEOMLabelText(text);
}

void TxKeyerCwRigControlForm::clearEOMLabelText()
{
    indicators.keyerIndicators->clearEOMLabelText();
}

void TxKeyerCwRigControlForm::setPttEnabledIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setPttEnabledIndicatorOnOff(on);
}


void TxKeyerCwRigControlForm::setTxStatusIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setTxStatusIndicatorOnOff(on);
}

void TxKeyerCwRigControlForm::setPttTypeText(const QString &text)
{
    indicators.pttIndicator->setPttTypeText(text);
}

void TxKeyerCwRigControlForm::clearPttTypeText()
{
    indicators.pttIndicator->clearPttTypeText();
}

void TxKeyerCwRigControlForm::setStoredMessagePlayingDisplay(const QString msg)
{
    cwMessagePlayingRow.display->setText(msg);
}

void TxKeyerCwRigControlForm::clearStoredMessagePlayingDisplay()
{
    cwMessagePlayingRow.display->clear();
}

