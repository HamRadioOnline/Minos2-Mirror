#ifndef QRZSERVICE_H
#define QRZSERVICE_H



#include "CallsignService.h"
#include "qrzdb.h"
#include "qxmlstream.h"

class QrzServerMainWindow;









class QRZService : public CallsignService
{
    Q_OBJECT

public:
    explicit QRZService(QRZDB* db, QObject* parent = nullptr);

    QString name() const override;

    void requestLogin(const QString& user, const QString& pass) override;
    //void lookupCallsign(const QString& call) override;

    void resetSession() override;
    void setCacheAge(int cacheAge) override;

private:
    QRZDB* m_db = nullptr;

    bool loginInProgress = false;
    QString logonCallsign;  //callsign/username to logon to service

    CallsignData qrzCallsignData;
    SessionData qrzSessionData;


    QString qrzStateErrorMessage;

    void sendUrl(QString url);
    void performLogin(const QString& user, const QString& pass);

    void parseSessionData(QXmlStreamReader &xmlData);
    void parseCallsignData(QXmlStreamReader &xmlData);
    //bool lookupCallsign(const QString &call, QrzCallsignData &result);
    void logMessage(QString msg);
    void callsignDataReceived();
    QString stripPasswordFromUrl(QString url);
    void sessionDataReceived();
    void parseDXCCData(QXmlStreamReader &xmlData);
};

#endif // QRZSERVICE_H
