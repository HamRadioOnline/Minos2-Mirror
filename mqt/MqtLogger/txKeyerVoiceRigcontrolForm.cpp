#include <QDebug>
#include "txKeyerVoiceRigcontrolForm.h"
#include "keyerWidgetFactory.h"



TxVoiceRigControlForm::TxVoiceRigControlForm(QWidget *parent)
    : TxKeyerFormBase(parent)
{

    // create ui

    indicators = KeyerWidgetFactory::createIndicators(this);
    indicatorLayout = KeyerWidgetFactory::createRowLayout(nullptr);
    indicatorLayout->addWidget(indicators.keyerIndicators);
    indicatorLayout->addWidget(indicators.pttIndicator);
    indicatorLayout->addStretch();


    QVBoxLayout *mainLayout = KeyerWidgetFactory::createMainLayout(nullptr);
    mainLayout->addLayout(indicatorLayout);

    setLayout(mainLayout);

    qDebug() << "Form:" << this->objectName()
             << "sizeHint =" << this->sizeHint()
             << "minSizeHint =" << this->minimumSizeHint();



}

TxVoiceRigControlForm::~TxVoiceRigControlForm()
{

}



void TxVoiceRigControlForm::setKeyerAvailableIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setKeyerAvailableIndicatorOnOff(on);
}

void TxVoiceRigControlForm::setKeyerAvailableVisible(bool visible)
{
    indicators.keyerIndicators->setKeyerAvailableVisible(visible);
}

void TxVoiceRigControlForm::setRepeatIndicatorOnOff(bool on)
{
    indicators.keyerIndicators->setRepeatIndicatorOnOff(on);
}

void TxVoiceRigControlForm::setRepeatIndicatorVisible(bool visible)
{
    indicators.keyerIndicators->setRepeatIndicatorVisible(visible);
}


void TxVoiceRigControlForm::setEOMLabelText(const QString &text)
{
    indicators.keyerIndicators->setEOMLabelText(text);
}

void TxVoiceRigControlForm::clearEOMLabelText()
{
    indicators.keyerIndicators->clearEOMLabelText();
}

void TxVoiceRigControlForm::setPttEnabledIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setPttEnabledIndicatorOnOff(on);
}


void TxVoiceRigControlForm::setTxStatusIndicatorOnOff(const bool on)
{
    indicators.pttIndicator->setTxStatusIndicatorOnOff(on);
}

void TxVoiceRigControlForm::setPttTypeText(const QString &text)
{
    indicators.pttIndicator->setPttTypeText(text);
}

void TxVoiceRigControlForm::clearPttTypeText()
{
    indicators.pttIndicator->clearPttTypeText();
}


