#ifndef DIGITALMODETXKEYER_H
#define DIGITALMODETXKEYER_H

#include <QObject>
#include "txKeyerbase.h"
#include "txKeyerFactory.h"
#include "txkeyerCommonConstants.h"

class DigitalModeTxKeyer : public TxKeyerBase
{
    Q_OBJECT

public:
    explicit DigitalModeTxKeyer(QObject *parent = nullptr);
    virtual ~DigitalModeTxKeyer() override;

    static void registerTxKeyer(TxKeyerFactory::TxKeyers*);


    void setContest(BaseContestLog *c) override;

    void txKeyerInit(int &numButtons) override;
    void sendMsgNum(TxKeyerParams &vkParam) override;
    void stopMsg(TxKeyerParams &vkParam) override;


    void sendCwMsg(TxKeyerParams &vmParams) override;
    void stopCwMsg() override;
    void sendCwFreeTextMsg(QString message) override;
    void setCwMemType(int cwMemType) override;
    bool getSetCwModeAndRestoreFlag() override;

    bool hasRecord() override;


    bool readTxKeyerButtonParams(int buttonNum, TxKeyerParams &vmParams) override;
    void saveTxKeyerButtonParams(const TxKeyerParams &vmParams ) override;

    void setPttOnOff(bool onOff) override;
    TxKeyerCommon::KeyerEomTypes getSelectedEomType() override;
    void setSelectedEomType(TxKeyerCommon::KeyerEomTypes eomType) override;

    int setup(TxKeyerFactory *txKeyerFactory, int &maxNumButtons, int &numButtons, QString selectedRadioName) override;
    void setRadioParams(int radioMaxNumButtons, QString selectedRadioName, serialCommonData::MINOS_PTT_TYPES pttType_, bool pttEnabled_) override;





};

#endif // DIGITALMODETXKEYER_H
