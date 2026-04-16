
#include "CallsignTypes.h"
#include "qrzservice.h"
#include "MTrace.h"

QRZService::QRZService(QRZDB* db, QObject* parent)
    : CallsignService(parent)
    , m_db(db)
{
}

QString QRZService::name() const
{
    return "QRZ";
}

void QRZService::requestLogin(const QString& user_, const QString& password_)
{
    if (loginInProgress)
        return;

    loginInProgress = true;

    QString user = user_.trimmed();
    logonCallsign = user;
    QString password = password_.trimmed();

    if (user.isEmpty() || password.isEmpty())
    {
        emit loginFailed("Missing credentials");
        loginInProgress = false;
        return;
    }

    QString url = QString("https://xmldata.qrz.com/xml/current/?")
                  + "username=" + user
                  + ";password=" + password
                  + ";agent=Minos";

    emit loginRequested(url);
}

/*
bool QRZService::lookupCallsign(const QString& call,
                                QrzCallsignData& result)
{
    // --------------------------
    // DB LOOKUP (service owns DB)
    // --------------------------
    if (m_db && m_db->getRecord(call) .getCallsign() == call)
    {
        result = m_db->getRecord(call);
        return true;
    }

    // --------------------------
    // NETWORK FALLBACK
    // --------------------------
    // Service should NOT call UI
    // So we emit a request OR call a network layer (next step)

    emit lookupNetworkRequested(call);  // OR callback style

    return false;
}
*/

void QRZService::sendUrl(QString url)
{
    logMessage(QString("sendUrl - %1").arg(stripPasswordFromUrl(url)));

    QNetworkAccessManager m_NetworkMngr;

    QUrl qurl( url );
    QNetworkRequest qnr( qurl );

    qnr.setRawHeader( "User-Agent" , "Mozilla/4.0 (compatible;Minos2)" );

    QSharedPointer<QNetworkReply> reply = QSharedPointer<QNetworkReply>(m_NetworkMngr.get( QNetworkRequest( QUrl( url ) ) ));

    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(&m_NetworkMngr, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(); // blocks stack until "finished()" has been called


    if ( reply->error() == QNetworkReply::NoError )
    {
        qrzStateErrorMessage.clear();

        int raw = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (raw == 301)
        {
            QUrl redirect =  reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            QNetworkRequest qnr1( redirect );
            qnr1.setRawHeader( "User-Agent" , "Mozilla/4.0 (compatible;Minos2)" );

            reply = QSharedPointer<QNetworkReply>(m_NetworkMngr.get( qnr1 ));
            QEventLoop loop;
            QObject::connect( reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            raw = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        }

        QXmlStreamReader xmlData;
        QByteArray xml = reply->readAll();
        xmlData.addData( xml );
        if (raw == 200)
        {
            logMessage(QString("XML read %1").arg(QString(xml)));
            if (xmlData.readNextStartElement())
            {
                if (xmlData.name().contains(QString("QRZDatabase")))
                {
                    bool csRX = false;
                    while (xmlData.readNextStartElement())
                    {
                        if (xmlData.name().contains(QString("Session")))
                        {
                            parseSessionData(xmlData);
                            sessionDataReceived();
                        }
                        else if (xmlData.name().contains(QString("Callsign")))
                        {
                            csRX = true;
                            parseCallsignData(xmlData);

                        }
                        else if (xmlData.name().contains(QString("DXCC")))
                        {
                            parseDXCCData(xmlData);
                        }
                    }
                    if (csRX)
                    {
                        QString sessmess = qrzSessionData.getMessage();
                        if (!sessmess.isEmpty())
                        {
                            qrzCallsignData.setMessage(sessmess);
                        }
                        qrzCallsignData.setDBDate(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd HH:mm:ss"));
                        qdb->createRecord(qrzCallsignData);
                        qrzRequests++;
                        dbRecords++;
                        callsignDataReceived();
                    }
                }
            }
        }
    }
    else
    {

        QString url_ = stripPasswordFromUrl(url);

        // error
        QString sslError;
        if (!QSslSocket::supportsSsl())
        {
            logMessage(QString("OpenSSSL version build is %1").arg(QSslSocket::sslLibraryBuildVersionString()));
            sslError = "\n" + tr("SSL not supported on this system.");
        }

        QString msg = QString( "HTTP Get of " ) + url_ + " failed: " + reply->errorString() + sslError;
        logMessage ( QString( "HTTP Get of " ) + url_ + " failed: " + reply->errorString()  + sslError );


        emit sendToErrorTextLabel(reply->errorString());
        emit sendToLogWindow(reply->errorString());
        emit sendStatusToRpc(qrzServiceStateFlags.getQrzLoggedOnFlag(), reply->errorString());

    }
}

QString QRZService::stripPasswordFromUrl(QString url)
{
    if (url.contains(";password="))
    {
        QStringList ul = url.split(";password=");
        if (ul.count() == 2)
        {
            return ul[0];
        }
    }
    return url;
}


void QRZService::sessionDataReceived()
{
    if (!qrzSessionData.getError().isEmpty())
    {
        logMessage(QString("Qrz Error: %1").arg(qrzSessionData.getError()));

        emit sendToErrorTextLabel(qrzSessionData.getError());
        emit sendToLogWindow(qrzSessionData.getError());

        if (qrzSessionData.getError() != "Connection refused")
        {
            // if "Connection refused" then logon won't work
            // for at least 24 hours, so no point in trying

            if (qrzSessionData.getKey().isEmpty())
            {
                // session has expired, we need to re-connect

                loginState = LoginState::Idle;
                emit logon();
            }
        }
    }

    if (loginState == LoginState::LoggingIn)
    {
        if (!qrzSessionData.getKey().isEmpty() && qrzSessionData.getError().isEmpty())
        {
            // logon succesfull
            loginState = LoginState::LoggedIn;
            QString msg = QString("Qrz Logged on Ok with call %1").arg(logonCallsign);
            logMessage(msg);
            emit sendToLogWindow(tr("Qrz logged on Ok with call %1").arg(logonCallsign));
            emit sendToErrorTextLabel("");
            emit sendToMessageTextLabel("");
            emit sendServiceStateConnected(true);
        }
        else
        {
            // send error message to client
            QString errorMsg = qrzSessionData.getError();
            logMessage(errorMsg);
            emit sendToLogWindow(tr("Logon failed to Qrz.com, logon callsign = %1, error = %2").arg(logonCallsign, errorMsg));
            emit sendServiceStateConnected(false);
            qrzServiceStateFlags.clear();
        }
    }


    if (qrzServiceStateFlags.getAskCallsignFlag())
    {
        QString stateMsg;
        if (!qrzSessionData.getError().isEmpty())
        {
            stateMsg = qrzSessionData.getError();
        }
        //        else if (!qrzSessionData.getMessage().isEmpty())
        //        {
        //            stateMsg = qrzSessionData.getMessage();
        //        }

        if (!stateMsg.isEmpty())
        {
            qrzCallsignData.clear();
            qrzCallsignData.setCallsign(requestedStation.getDxCall());
            if (requestedStation.getLoggerFlag())
            {
                QrzServerRpc::getQrzServerRpc()->sendQrzResponseToLoggerDisplay(qrzCallsignData, stateMsg, requestedStation.getFromStationName(),requestedStation.getLoggerUuid());

            }
            else
            {
                QrzServerRpc::getQrzServerRpc()->sendQrzResponseToClusterServer(qrzCallsignData.getCallsign(), "", stateMsg, "", "", "");
            }
            qrzServiceStateFlags.setAskCallsignFlag(false);
        }
    }

}

void QRZService::callsignDataReceived()
{
    if (qrzServiceStateFlags.getAskCallsignFlag())
    {
        if (!requestedStation.getLoggerFlag())
        {
            // a request from cluster Server

            QString msg = QString("Cluster Qrz Callsign Data received for call = %1, Qra = %2 - Send to Cluster Server").arg(requestedStation.getDxCall(), qrzCallsignData.getQra());
            logMessage(msg);

            emit sendToLogWindow(tr("Cluster Qrz Callsign Data received for call = %1, Qra = %2 - Send to Cluster Server").arg(requestedStation.getDxCall(), qrzCallsignData.getQra()));

            emit sendQrzResponseToClusterServer(requestedStation.getDxCall(), qrzCallsignData.getQra(), QRA_LOOKUP_OK, requestedStation.getSpotterCall(), "", rpcConstants::qrzServerCallOK);

        }
        else
        {
            // a request from logger
            QString msg = QString(QString("Logger Qrz Callsign Data received for call = %1, Send to Qrz Display in Logger Server").arg(requestedStation.getDxCall()));
            logMessage(msg);
            emit sendToLogWindow(tr("Logger Qrz Callsign Data received for call = %1, Send to Qrz Display in Logger Server").arg(requestedStation.getDxCall()));
            QString stateMsg = "";

            qrzCallsignData.setDbRecords(dbRecords);
            qrzCallsignData.setDbRecalls(dbRequests);
            qrzCallsignData.setQrzRecalls(qrzRequests);
            emit sendQtrResponseToLoggerDisplay(qrzCallsignData, stateMsg, requestedStation.getFromStationName(), requestedStation.getLoggerUuid());

        }
        qrzServiceStateFlags.setAskCallsignFlag(false);
    }
}


void QRZService::parseSessionData(QXmlStreamReader &xmlData)
{
    logMessage(QString("Parse Session Data"));
    qrzSessionData.clear();
    while(xmlData.readNextStartElement())
    {
        if (xmlData.name() == QString("Error"))
        {
            qrzSessionData.setError(xmlData.readElementText());
            logMessage(QString("Session Data: Error = %1").arg(qrzSessionData.getError()));
        }
        else if(xmlData.name() == QString("Key"))
        {
            qrzSessionData.setKey( xmlData.readElementText() );
            logMessage(QString("Session Data: Key = %1").arg(qrzSessionData.getKey()));
        }
        else if (xmlData.name() == QString("SubExp"))
        {
            qrzSessionData.setSubExp(xmlData.readElementText());
            logMessage(QString("Session Data: SubExp = %1").arg(qrzSessionData.getSubExp()));
        }
        else if (xmlData.name() == QString("Message"))
        {
            qrzSessionData.setMessage(xmlData.readElementText());
            logMessage(QString("Session Data: Message = %1").arg(qrzSessionData.getMessage()));
        }
        else
        {
            xmlData.skipCurrentElement();
        }
    }
}

void QRZService::parseCallsignData(QXmlStreamReader &xmlData)
{
    trace(QString("Parse Callsign Data"));

    qrzCallsignData.clear();

    qrzCallsignData.setDataSource("QRZ.com");

    while(xmlData.readNextStartElement())
    {
        if(xmlData.name() == QString("call"))
        {
            qrzCallsignData.setCallsign(xmlData.readElementText());
            trace(QString("Callsign Data: callsign = %1").arg(qrzCallsignData.getCallsign()));
        }
        else if (xmlData.name() == QString("fname"))
        {
            qrzCallsignData.setFirstName(xmlData.readElementText());
            trace(QString("Callsign Data: first name = %1").arg(qrzCallsignData.getFirstName()));
        }
        else if (xmlData.name() == QString("name"))
        {
            qrzCallsignData.setName(xmlData.readElementText());
            trace(QString("Callsign Data: name = %1").arg(qrzCallsignData.getName()));
        }
        else if (xmlData.name() == QString("addr1"))
        {
            qrzCallsignData.setAddr1(xmlData.readElementText());
            trace(QString("Callsign Data: addr1 = %1").arg(qrzCallsignData.getAddr1()));
        }
        else if (xmlData.name() == QString("addr2"))
        {
            qrzCallsignData.setAddr2(xmlData.readElementText());
            trace(QString("Callsign Data: addr2 = %1").arg(qrzCallsignData.getAddr2()));
        }
        else if (xmlData.name() == QString("county"))
        {
            qrzCallsignData.setCounty(xmlData.readElementText());
            trace(QString("Callsign Data: county = %1").arg(qrzCallsignData.getCounty()));
        }
        else if (xmlData.name() == QString("country"))
        {
            qrzCallsignData.setCountry(xmlData.readElementText());
            trace(QString("Callsign Data: country = %1").arg(qrzCallsignData.getCountry()));
        }
        else if (xmlData.name() == QString("lat"))
        {
            qrzCallsignData.setLat(xmlData.readElementText());
            trace(QString("Callsign Data: lat = %1").arg(qrzCallsignData.getLat()));
        }
        else if (xmlData.name() == QString("lon"))
        {
            qrzCallsignData.setLon(xmlData.readElementText());
            trace(QString("Callsign Data: lon = %1").arg(qrzCallsignData.getLon()));
        }
        else if (xmlData.name() == QString("grid"))
        {
            QString grid = xmlData.readElementText();
            if (grid.size() == 6)
            {
                grid = grid.replace(4, 2, grid.right(2).toUpper());
            }
            qrzCallsignData.setQra(grid);
            trace(QString("Callsign Data: grid = %1").arg(qrzCallsignData.getQra()));
        }
        else if (xmlData.name() == QString("cqzone"))
        {
            qrzCallsignData.setCqZone(xmlData.readElementText());
            trace(QString("Callsign Data: cqZone = %1").arg(qrzCallsignData.getCqZone()));
        }
        else if (xmlData.name() == QString("ituzone"))
        {
            qrzCallsignData.setItuZone(xmlData.readElementText());
            trace(QString("Callsign Data: ituZone = %1").arg(qrzCallsignData.getItuZone()));
        }
        else if (xmlData.name() == QString("moddate"))
        {
            qrzCallsignData.setModDate(xmlData.readElementText());
            trace(QString("Callsign Data: moddate = %1").arg(qrzCallsignData.getModDate()));
        }
        else
        {
            xmlData.skipCurrentElement();
        }
    }
}

bool QRZService::lookupCallsign(const QString& call,
                                QrzCallsignData& result)
{
    QrzCallsignData data = m_db->getRecord(call);

    if (!data.getCallsign().isEmpty())
    {
        result = data;
        return true;
    }

    emit lookupNetworkRequested(call);
    return false;
}

void QRZService::parseDXCCData(QXmlStreamReader &xmlData)
{
    Q_UNUSED(xmlData)
}


void QRZService::logMessage(QString msg)
{
    trace("QRZService: " + msg);
}
