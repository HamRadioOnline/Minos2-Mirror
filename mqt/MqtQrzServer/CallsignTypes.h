#ifndef CALLSIGNTYPES_H
#define CALLSIGNTYPES_H

#include <QString>


enum class LoginState
{
    Idle,
    LoggingIn,
    LoggedIn,
    Failed
};

class CallsignData
{
public:
    void clear()
    {
        dataSource.clear();
        callsign.clear();
        firstName.clear();
        name.clear();
        addr1.clear();
        addr2.clear();
        county.clear();
        country.clear();
        lat.clear();
        lon.clear();
        qra.clear();
        cqZone.clear();
        ituZone.clear();
        moddate.clear();
        dbdate.clear();
        message.clear();

        dbRecords = 0;
        dbRecalls = 0;
        qrzRecalls = 0;
    }

    QString dataSource;
    QString callsign;
    QString firstName;
    QString name;
    QString addr1;
    QString addr2;
    QString county;
    QString country;
    QString lat;
    QString lon;
    QString qra;
    QString cqZone;
    QString ituZone;
    QString moddate;
    QString dbdate;
    QString message;

    int dbRecords = 0;
    int dbRecalls = 0;
    int qrzRecalls = 0;
};


class SessionData
{
public:
    SessionData(){}

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


class CallsignRequest
{
public:
    CallsignRequest() { clear(); }

    void clear()
    {
        dxCall.clear();
        spotterCall.clear();
        fromStationName.clear();
        loggerUuid.clear();
        loggerFlag = false;
    }

    void setDxCall(const QString& v) { dxCall = v; }
    QString getDxCall() const { return dxCall; }

    void setSpotterCall(const QString& v) { spotterCall = v; }
    QString getSpotterCall() const { return spotterCall; }

    void setFromStationName(const QString& v) { fromStationName = v; }
    QString getFromStationName() const { return fromStationName; }

    void setLoggerUuid(const QString& v) { loggerUuid = v; }
    QString getLoggerUuid() const { return loggerUuid; }

    void setLoggerFlag(bool v) { loggerFlag = v; }
    bool getLoggerFlag() const { return loggerFlag; }

private:
    QString dxCall;
    QString spotterCall;
    QString fromStationName;
    QString loggerUuid;
    bool loggerFlag = false;
};





#endif // CALLSIGNTYPES_H
