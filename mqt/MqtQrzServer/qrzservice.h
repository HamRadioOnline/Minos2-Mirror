#ifndef QRZSERVICE_H
#define QRZSERVICE_H



#include "CallsignService.h"
#include "qrzdb.h"
#include "qxmlstream.h"

class QrzServerMainWindow;

class QrzSessionData
{
public:
    QrzSessionData(){}

    void clear()
    {
        error.clear();
        message.clear();
        key.clear();
        subExp.clear();
    }

    void setError(QString error_){error = error_;}
    QString getError(){return error;}

    void setMessage(QString message_){message = message_;}
    QString getMessage(){return message;}

    void setKey(QString key_){key = key_;}
    QString getKey(){return key;}

    void setSubExp(QString subExp_){subExp = subExp_;}
    QString getSubExp(){return subExp;}

private:

    QString error;
    QString message;
    QString key;
    QString subExp;



};

class QrzServiceStateFlags
{

public:

    QrzServiceStateFlags()
    {
        clear();
    }

    void clear()
    {
        askLogonFlag = false;
        askCallsignFlag = false;
        qrzLoggedOnFlag = false;
    }

    bool getAskLogonFlag(){return askLogonFlag;}
    void setAskLogonFlag(bool state){askLogonFlag = state;}

    bool getAskCallsignFlag(){return askCallsignFlag;}
    void setAskCallsignFlag(bool state){askCallsignFlag = state;}

    bool getQrzLoggedOnFlag(){return qrzLoggedOnFlag;}
    void setQrzLoggedOnFlag(bool state){qrzLoggedOnFlag = state;}



private:

    bool askLogonFlag = false;
    bool askCallsignFlag = false;
    bool qrzLoggedOnFlag = false;




};





class QRZService : public CallsignService
{
    Q_OBJECT

public:
    explicit QRZService(QRZDB* db, QObject* parent = nullptr);

    QString name() const override;

    void requestLogin(const QString& user, const QString& pass) override;
    void lookupCallsign(const QString& call) override;

    void resetSession() override;
    void setCacheAge(int cacheAge) override;

private:
    QRZDB* m_db = nullptr;

    bool loginInProgress = false;

    QrzCallsignData qrzCallsignData;
    QrzSessionData qrzSessionData;

    void sendUrl(QString url);
    void performLogin(const QString& user, const QString& pass);

    void parseSessionData(QXmlStreamReader &xmlData);
    void parseCallsignData(QXmlStreamReader &xmlData);
    bool lookupCallsign(const QString &call, QrzCallsignData &result);
    void logMessage(QString msg);
    void callsignDataReceived();
    QString stripPasswordFromUrl(QString url);
    void sessionDataReceived();
};

#endif // QRZSERVICE_H
