#include "servicesconfigmanager.h"

ServicesConfigManager::ServicesConfigManager(const QString &fileName, QObject *parent)
    : QObject(parent), m_fileName(fileName)
{

}

ServiceConfig ServicesConfigManager::getServicesConfig(const QString& serviceName)
{
    QSettings config(m_fileName, QSettings::IniFormat);

    config.beginGroup(serviceName);

    ServiceConfig cfgs;
    cfgs.cacheAge = config.value("cacheAge", "").toInt();


    config.endGroup();

    return cfgs;
}

void ServicesConfigManager::setServicesConfig(const QString& serviceName, const ServiceConfig& cfgs)
{
    QSettings config(m_fileName, QSettings::IniFormat);

    config.beginGroup(serviceName);

    config.setValue("cacheAge", cfgs.cacheAge);

    config.endGroup();
}

bool ServicesConfigManager::setConfigIfChanged(const QString& serviceName, const ServiceConfig& cfg)
{
    ServiceConfig current = getServicesConfig(serviceName);

    if (current.cacheAge == cfg.cacheAge)
        return false;

    setServicesConfig(serviceName, cfg);
    return true;
}
