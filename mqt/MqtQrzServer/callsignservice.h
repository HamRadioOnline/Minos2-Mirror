#ifndef CALLSIGNSERVICE_H
#define CALLSIGNSERVICE_H



#include "qobject.h"
#include <QString>

class QrzCallsignData;


enum class LoginState
{
    Idle,
    LoggingIn,
    LoggedIn,
    Failed
};

class CallsignService : public QObject
{
    Q_OBJECT

public:
    explicit CallsignService(QObject* parent = nullptr)
        : QObject(parent) {}

    virtual ~CallsignService() = default;

    virtual QString name() const = 0;

    virtual void requestLogin(const QString& user, const QString& pass) = 0;
    //virtual void lookupCallsign(const QString& call) = 0;

    virtual void resetSession() = 0;
    virtual void setCacheAge(int cacheAge) = 0;

signals:
    void loginRequested(const QString url);
    void loginSucceeded(const QString sessionKey);
    void loginFailed(const QString error);

    void lookupSucceeded(const QString& call, const QrzCallsignData result);
    void lookupFailed(const QString& call, const QString error);

    void sendToErrorTextLabel(QString error);
    void sendToMessageTextLabel(QString msg);
    void sendToLogWindow(QString error);
    //void sendStatusToRpc(bool loggedOn, QString error);
    void logon();

    void sendQrzResponseToClusterServer(QString dxCall, QString dxQra, QString dxCallState, QString spotterCall, QString spotterQra, QString spotterCallState);
    void sendQtrResponseToLoggerDisplay(QrzCallsignData qrzCallsignData, QString state, QString fromStationName, QString uuid);


    void sendServiceStateConnected(bool state);

protected:

    LoginState loginState = LoginState::Idle;
};
#endif // CALLSIGNSERVICE_H
