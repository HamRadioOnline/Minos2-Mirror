#ifndef CALLSIGNSERVICE_H
#define CALLSIGNSERVICE_H



#include "qobject.h"
#include <QString>

class QrzCallsignData;

class CallsignService : public QObject
{
    Q_OBJECT

public:
    explicit CallsignService(QObject* parent = nullptr)
        : QObject(parent) {}

    virtual ~CallsignService() = default;

    virtual QString name() const = 0;

    virtual void requestLogin(const QString& user, const QString& pass) = 0;
    virtual void lookupCallsign(const QString& call) = 0;

    virtual void resetSession() = 0;
    virtual void setCacheAge(int cacheAge) = 0;

signals:
    void loginRequested(const QString& url);
    void loginSucceeded(const QString& sessionKey);
    void loginFailed(const QString& error);

    void lookupSucceeded(const QString& call, const QrzCallsignData& result);
    void lookupFailed(const QString& call, const QString& error);

    void stateErrorMessage(QString error);
    void sendStatusToRpc(bool loggedOn, QString& error);
};
#endif // CALLSIGNSERVICE_H
