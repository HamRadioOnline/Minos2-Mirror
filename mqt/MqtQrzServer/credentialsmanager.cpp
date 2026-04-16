#include "credentialsmanager.h"

CredentialsManager::CredentialsManager(const QString& fileName, QObject* parent)
    : QObject(parent), m_fileName(fileName)
{
}

ServiceCredentials CredentialsManager::getCredentials(const QString& serviceName)
{
    QSettings config(m_fileName, QSettings::IniFormat);

    config.beginGroup(serviceName);

    ServiceCredentials creds;
    creds.username = config.value("username", "").toString();
    creds.password = config.value("password", "").toString();

    config.endGroup();

    return creds;
}

void CredentialsManager::setCredentials(const QString& serviceName, const ServiceCredentials& creds)
{
    QSettings config(m_fileName, QSettings::IniFormat);

    config.beginGroup(serviceName);

    config.setValue("username", creds.username);
    config.setValue("password", creds.password);

    config.endGroup();
}

bool CredentialsManager::setCredentialsIfChanged(const QString& serviceName, const ServiceCredentials& creds)
{
    ServiceCredentials current = getCredentials(serviceName);

    if (current.username == creds.username &&
        current.password == creds.password)
    {
        return false;
    }

    setCredentials(serviceName, creds);
    return true;
}
