#include "digitalmodetxkeyer.h"
#include "tlogcontainer.h"
#include "tsinglelogframe.h"
#include "txKeyerFactory.h"
#include "txkeyerCommonConstants.h"

DigitalModeTxKeyer::DigitalModeTxKeyer(QObject *parent) : TxKeyerBase{parent}
{




}

DigitalModeTxKeyer::~DigitalModeTxKeyer()
{




}


void DigitalModeTxKeyer::registerTxKeyer(TxKeyerFactory::TxKeyers* vmKeyersList)
{
    QString keyerName = getTxKeyerDisplayName(TxKeyerId::DigitalModes);

    TxKeyerCapabilities voiceMemCap;

    voiceMemCap.setTxKeyerId(TxKeyerId::DigitalModes);
    voiceMemCap.setKeyerName(keyerName);
    voiceMemCap.setNumVoiceKeys(8);
    voiceMemCap.setsupportSerial(false);
    voiceMemCap.setUseCatPTTForEom(false);
    voiceMemCap.setEnableCwMode(false);
    voiceMemCap.setSupportRepeatMsg(false);
    voiceMemCap.setHasPip(false);
    voiceMemCap.setHasTxStatus(true);
    voiceMemCap.setSetupButton(true);
    voiceMemCap.setHasAvailStatus(false);
    voiceMemCap.setHasMessageRepeat(false);


    (*vmKeyersList)[keyerName] = voiceMemCap;

}


void DigitalModeTxKeyer::setContest(BaseContestLog *c)
{
    Q_UNUSED(c)
}

void DigitalModeTxKeyer::txKeyerInit(int &numButtons)
{
    Q_UNUSED(numButtons)
}
void DigitalModeTxKeyer::sendMsgNum(TxKeyerParams &vkParam)
{
    Q_UNUSED(vkParam)
}
void DigitalModeTxKeyer::stopMsg(TxKeyerParams &vkParam)
{
     Q_UNUSED(vkParam)
}


void DigitalModeTxKeyer::sendCwMsg(TxKeyerParams &vmParams)
{
   Q_UNUSED(vmParams)
}

void DigitalModeTxKeyer::stopCwMsg()
{

}
void DigitalModeTxKeyer::sendCwFreeTextMsg(QString message)
{
    Q_UNUSED(message)
}
void DigitalModeTxKeyer::setCwMemType(int cwMemType)
{
    Q_UNUSED(cwMemType)
}
bool DigitalModeTxKeyer::getSetCwModeAndRestoreFlag()
{
    return true;
}

bool DigitalModeTxKeyer::hasRecord()
{
    return false;
}

bool DigitalModeTxKeyer::readTxKeyerButtonParams(int buttonNum, TxKeyerParams &vmParams)
{
    Q_UNUSED(buttonNum)
    Q_UNUSED(vmParams)
    return true;
}
void DigitalModeTxKeyer::saveTxKeyerButtonParams(const TxKeyerParams &vmParams )
{
    Q_UNUSED(vmParams)
}

void DigitalModeTxKeyer::setPttOnOff(bool onOff)
{
    Q_UNUSED(onOff)
}
TxKeyerCommon::KeyerEomTypes DigitalModeTxKeyer::getSelectedEomType()
{
    return TxKeyerCommon::KeyerEomTypes::Eom_None;
}
void DigitalModeTxKeyer::setSelectedEomType(TxKeyerCommon::KeyerEomTypes eomType)
{
    Q_UNUSED(eomType)
}

int DigitalModeTxKeyer::setup(TxKeyerFactory *txKeyerFactory, int &maxNumButtons, int &numButtons, QString selectedRadioName)
{
    Q_UNUSED(txKeyerFactory)
     Q_UNUSED(maxNumButtons)
    Q_UNUSED(numButtons)
     Q_UNUSED(selectedRadioName)

    return 0;
}

void DigitalModeTxKeyer::setRadioParams(int radioMaxNumButtons, QString selectedRadioName, serialCommonData::MINOS_PTT_TYPES pttType_, bool pttEnabled_)
{
    Q_UNUSED(radioMaxNumButtons)
    Q_UNUSED(selectedRadioName)
    Q_UNUSED(pttType_)
    Q_UNUSED(pttEnabled_)

}
