#ifndef QRZDB_H
#define QRZDB_H

#include <QObject>
#include <QSqlDatabase>
#include "qrzServerCommon.h"

class QRZDB : public QObject
{
    Q_OBJECT
public:
    explicit QRZDB(QObject *parent = nullptr);

    bool createRecord(const CallsignData &csData);

    CallsignData getRecord(const QString cs);

    int getRecordCount();

    void resetDB();
};

#endif // QRZDB_H
