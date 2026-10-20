#include <QLineEdit>
#include <QTextEdit>

#include "QtUtils.h"
#include "AppStartup.h"
#include "MTrace.h"
#include "delayedaction.h"
#include "dmmainwindow.h"
#include "fileutils.h"
#include "frequency.h"
#include "rxbuffer.h"
#include "enginewindow.h"
#include "engineconfigure.h"

#include "jttyframe.h"
#include "ui_jttyframe.h"

JTTYFrame::JTTYFrame(EngineWindow *parent, QLineEdit *sendEdit, QString fname, QString name) :
    QFrame(parent),
    ui(new Ui::JTTYFrame),
    engineWindow(parent),
    engineName(name),
    sendEdit(sendEdit),
    fname(fname)
{
    ui->setupUi(this);
    connect(mainWindow, &DMMainWindow::setSpeeds, this, &JTTYFrame::onSetSpeeds);
    connect(engineWindow, &EngineWindow::setSpeeds, this, &JTTYFrame::onSetSpeeds);
    connect(engineWindow, &EngineWindow::sendCharactersDown, this, &JTTYFrame::onSendCharacters);
    connect(engineWindow, &EngineWindow::rigModeFreq, this, &JTTYFrame::onRigModeFreq);

    jttyPort = EngineConfigure::getEnginePort(name);
    createProcess();
    jttyServer = new QTcpServer();
    connect(jttyServer, &QTcpServer::acceptError, this, &JTTYFrame::handleSocketAcceptError);

    connect(jttyServer,
            &QTcpServer::newConnection,
            this,
            &JTTYFrame::onNewConnection);

    jttyServer->listen(QHostAddress::LocalHost, EngineConfigure::getEnginePort(name));
}

JTTYFrame::~JTTYFrame()
{
    delete ui;
}
void JTTYFrame::handleSocketAcceptError()
{
    trace("SocketAcceptError");
}

void JTTYFrame::onNewConnection()
{
    jttyClient = jttyServer->nextPendingConnection();

    if (jttyClient)
    {
        trace("New connection from WSJT-X");
        connect(jttyClient, &QAbstractSocket::readyRead, this, &JTTYFrame::onReadyRead);
        connect(jttyClient, &QTcpSocket::connected, this, &JTTYFrame::connected);
        connect(jttyClient, &QTcpSocket::disconnected, this, &JTTYFrame::disconnected);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        connect(jttyClient, &QTcpSocket::errorOccurred, this, &JTTYFrame::connectionError);
#else
        connect(jttyClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(JTTYFrame(QAbstractSocket::SocketError)));
#endif

        connect(jttyClient, &QAbstractSocket::bytesWritten, this, &JTTYFrame::onBytesWritten);
    }
}
void JTTYFrame::connectionError(QAbstractSocket::SocketError /*error*/)
{
    trace("Socket error: " + jttyClient->errorString());
    // an error occurred. Close down current process, restart
    if (jttyClient)
    {
        jttyClient->deleteLater();
        jttyClient = nullptr;
    }
}
void JTTYFrame::sendCharacters(const QString &, int)
{

}
void JTTYFrame::onBytesWritten(qint64 /*cnt*/)
{

}

void JTTYFrame::sendMode(QString)
{

}

void JTTYFrame::closeFrame()
{
    if (jttyProcess)
    {
        trace(engineName + " About to ask engine to exit");
        jttyActive = false;
        jttyClient->close();
        jttyClient->deleteLater();
        jttyClient = nullptr;

        jttyServer->close();
        jttyServer->deleteLater();
        jttyServer = nullptr;

        jttyProcess->close();
        if (jttyProcess)
        {
            // if close happens quickly then on_finished will
            // clear jttyProcess
            jttyProcess->waitForFinished(1000);
        }

    }
}

void JTTYFrame::createProcess()
{
    jttyProcess = new QProcess(this);
    connect (jttyProcess, &QProcess::started, this, &JTTYFrame::on_started);
    connect (jttyProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &JTTYFrame::on_finished);
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    connect (jttyProcess, &QProcess::errorOccurred, this, &JTTYFrame::on_error);
#else
    connect (jttyProcess, &QProcess::error, this, &JTTYFrame::on_error);
#endif

    connect (jttyProcess, &QProcess::readyReadStandardError, this, &JTTYFrame::on_readyReadStandardError);
    connect (jttyProcess, &QProcess::readyReadStandardOutput, this, &JTTYFrame::on_readyReadStandardOutput);

    QStringList engineOpts = {"-p=" + QString::number(jttyPort),
                               "--mode=jtty",
                              "--rig-name=forEW1"};

    QString p = engineOpts.join(" ");
    jttyProcess->start(fname, engineOpts, QProcess::ReadWrite);
    jttyActive = true;
}

void JTTYFrame::analyseJttyMessage(QString m)
{
    trace("jttyMessage " + m);
}

void JTTYFrame::onSendCharacters(QString, int markf)
{

}

void JTTYFrame::onSetSpeeds(QString b, QString m)
{

}

void JTTYFrame::onRigModeFreq(QString, Frequency)
{

}

void JTTYFrame::on_finished(int err, QProcess::ExitStatus exitStatus)
{
    trace( "JTTY:on_finished:" + QString::number(err) + ":" + QString::number(exitStatus));
    if (jttyProcess)
    {

        jttyProcess->closeWriteChannel();
        jttyProcess->deleteLater();
        jttyProcess = nullptr;
    }
    if (jttyActive)
    {
        createProcess();
    }
}

void JTTYFrame::on_error(QProcess::ProcessError error)
{
    trace( "JTTY:on_error:" + QString::number(error));
    jttyProcess->deleteLater();
    jttyProcess = nullptr;
}

void JTTYFrame::on_readyReadStandardError()
{
    if (jttyProcess)
    {
        QString r = jttyProcess->readAllStandardError();
        trace( "JTTY:stdErr:" + r);
    }
}

void JTTYFrame::on_readyReadStandardOutput()
{
    if (jttyProcess)
    {
        QString line = jttyProcess->readAllStandardOutput();
        trace("JTTY:stdOut:" + line);
    }
}

void JTTYFrame::on_started()
{
    trace("JTTY:on_started");

}

void JTTYFrame::connected()
{
    trace("JTTY connected");

}

void JTTYFrame::disconnected()
{
    trace("JTTY disconnected");

}
void JTTYFrame::onReadyRead()
{
    QByteArray b = jttyClient->readAll();
    QString msg = QString(b);

    QString traceMsg = msg.remove("\r");
    if (traceMsg.endsWith("\n"))
    {
        traceMsg.chop(1);
    }
    trace(QString(engineName + " messageRx: %1").arg(traceMsg));

    // Now chop into lines and decode the JSON in each one
    msgbuf.append(msg);

    int p = msgbuf.indexOf("\n");
    while (p >= 0)
    {
        QString m = msgbuf.left(p + 1);
        msgbuf = msgbuf.mid(p + 1);
        p = msgbuf.indexOf("\n");

        analyseJttyMessage(m);
    }


}
