#ifndef QRZCQSERVICE_H
#define QRZCQSERVICE_H

#include "callsignservice.h"
#include "qrzdb.h"
#include <QObject>

class QRZCQService : public CallsignService
{
    Q_OBJECT

public:
    explicit QRZCQService(QRZDB* db, QObject* parent = nullptr);
    QString name() const override;
    void requestLogin(const QString& user, const QString& pass) override;
    //bool lookupCallsign(const QString& call) override;

    void resetSession() override;
    void setCacheAge(int cacheAge) override;

private:
    QRZDB* m_db;
};

#endif // QRZCQSERVICE_H
