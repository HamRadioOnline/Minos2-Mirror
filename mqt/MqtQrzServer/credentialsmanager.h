#ifndef CREDENTIALSMANAGER_H
#define CREDENTIALSMANAGER_H

#include <QObject>
#include <QSettings>

struct ServiceCredentials
{
    QString username;
    QString password;

    bool isEmpty() const
    {
        return username.isEmpty() || password.isEmpty();
    }
};



class CredentialsManager : public QObject
{
    Q_OBJECT

public:
    explicit CredentialsManager(const QString& fileName, QObject* parent = nullptr);

    ServiceCredentials getCredentials(const QString& serviceName);
    void setCredentials(const QString& serviceName, const ServiceCredentials& creds);

    //int getCacheAge(const QString& serviceName) const;
    //void setCacheAge(const QString& serviceName, int age);

    void migrateLegacySettings();  // call once at startup

    bool setCredentialsIfChanged(const QString &serviceName, const ServiceCredentials &creds);
private:
    QString m_fileName;
};

#endif // CREDENTIALSMANAGER_H
