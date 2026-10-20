#ifndef JTTYFRAME_H
#define JTTYFRAME_H

#include <QFrame>
#include <QProcess>
#include <QTcpServer>
#include "frequency.h"

namespace Ui {
class JTTYFrame;
}
class QLineEdit;
class QTextEdit;
class EngineWindow;

class JTTYFrame : public QFrame
{
    Q_OBJECT
    Ui::JTTYFrame *ui;

public:
    explicit JTTYFrame(EngineWindow *parent, QLineEdit *sendEdit, QString fname, QString name);
    ~JTTYFrame();

    void sendCharacters(const QString &, int);
    void sendMode(QString);

    void closeFrame();

private:
    EngineWindow *engineWindow = nullptr;
    QString engineName;
    int jttyPort = 7502;
    bool jttyActive = false;

    QLineEdit *sendEdit = nullptr;
    QProcess *jttyProcess = nullptr;
    QTcpServer* jttyServer = nullptr;
    QTcpSocket* jttyClient = nullptr;
    QString msgbuf;

    QString fname;
    void createProcess();

    void analyseJttyMessage(QString m);
private slots:
    void onSendCharacters(QString, int markf);
    void onSetSpeeds(QString b, QString m);

    void onRigModeFreq(QString, Frequency);
    void on_finished(int err, QProcess::ExitStatus exitStatus);
    void on_error(QProcess::ProcessError error);
    void on_readyReadStandardError();
    void on_readyReadStandardOutput();
    void on_started();

    void handleSocketAcceptError();
    void onNewConnection();

    void connected();
    void disconnected();
    void connectionError(QAbstractSocket::SocketError error);
    void onReadyRead();
    void onBytesWritten(qint64 /*cnt*/);

};

#endif // JTTYFRAME_H
