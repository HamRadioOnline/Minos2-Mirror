#ifndef SERVICESCONFIGMANAGER_H
#define SERVICESCONFIGMANAGER_H

#include <QObject>
#include <QSettings>

struct ServiceConfig
{
    int cacheAge = 0;
};


class ServicesConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit ServicesConfigManager(const QString& fileName, QObject* parent = nullptr);

    ServiceConfig getServicesConfig(const QString& serviceName);
    void setServicesConfig(const QString& serviceName, const ServiceConfig& config);
    bool setConfigIfChanged(const QString &serviceName, const ServiceConfig &cfg);

private:
    QString m_fileName;


};

#endif // SERVICESCONFIGMANAGER_H
