#ifndef N1MMBROADCAST_H
#define N1MMBROADCAST_H

#include "spotbasedata.h"
#include <QObject>
#include <QUdpSocket>
class BaseContact;
class BaseContestLog;

class SA6FAXResponse
{
public:
    QString resp;
    bool found = false;
    int confidence = 0;
    QString call;
    QString matchedCall;
    QString id;
    QString locator;

    SA6FAXResponse();
    SA6FAXResponse(QString resp);

    bool parse(QString);
};
class N1MMBroadcast:public QObject
{
    Q_OBJECT

    QUdpSocket bc;

    bool contactsSelect = false;
    bool extCSSelect = false;
    bool extSA6FAXCSSelect = false;
    bool spotsSelect = false;
    bool wsjtxRbSelect = false;
    bool ADIFSelect = false;
    QString contactsAddr;
    quint16 contactsPort = 0;
    QString extCSAddr;
    quint16 extCSPort = 0;
    QString extSA6FAXCSAddr;
    quint16 extSA6FAXCSPort = 0;
    QString spotsAddr;
    quint16 spotsPort = 0;
    QString wsjtxRbAddr;
    quint16 wsjtxRbPort = 0;
    QString ADIFAddr;
    quint16 ADIFPort = 0;

    QHostAddress contactsHost;
    QHostAddress extCSHost;
    QHostAddress extSA6FAXCSHost;
    QHostAddress spotsHost;
    QHostAddress wsjtxRbHost;
    QHostAddress ADIFHost;

    QString genContactStanza(QString type, BaseContestLog *c, QSharedPointer<BaseContact> tct);
    
    QString genDeleteStanza(QSharedPointer<BaseContact> tct);

    QString genSpotsStanza(QSharedPointer<ClusterSpotData> spotMsg, bool delSpot);
public:
    N1MMBroadcast();

    SA6FAXResponse lastResponse;
    void configure();
    bool setAddress(QString addr, QHostAddress &host);
    
private slots:
    void afterQSOSaved(BaseContestLog *c, QSharedPointer<BaseContact> tct);
    void wsjtxDatagram(int, QByteArray *);
    void callsignLookup(BaseContestLog *, QString);
    void dxSpots(QSharedPointer<ClusterSpotData> spotMsg, bool delSpot);

    void pending_datagrams ();
};

#endif // N1MMBROADCAST_H
