#pragma once

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QMap>

class SerialPortControlLineWatcher : public QObject
{
    Q_OBJECT
public:
    // Nested enum for logical function
    enum class ControlFunction {
        None,
        Voice,
        CW
    };
    Q_ENUM(ControlFunction)

    explicit SerialPortControlLineWatcher(QSerialPort* port, QObject* parent = nullptr);

    // Assign a line to Voice or CW
    void setLineAllocation(const QString& lineName, ControlFunction func);

    // Enable/disable monitoring for a line
    void enableLine(const QString& lineName, bool enable);

signals:
    // Fully qualified enum type for Qt moc
    void controlLineTriggered(QString lineName,
                              SerialPortControlLineWatcher::ControlFunction function,
                              bool active);

private slots:
    void pollLines();

private:
    bool readLine(const QString& line, QSerialPort::PinoutSignals pinSignals);

    QSerialPort* serialPort;
    QTimer pollTimer;

    // Mapping: line name → logical function (Voice/CW)
    QMap<QString, ControlFunction> lineAssignments;

    // Enabled/disabled state
    QMap<QString, bool> enabled;

    // Last known states
    QMap<QString, bool> lastState;

    // Line → QSerialPort::PinoutSignals map
    inline static const QMap<QString, QSerialPort::PinoutSignal> lineMap = {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        {"CTS", QSerialPort::ClearToSendSignal},
        {"DSR", QSerialPort::DataSetReadySignal},
        {"DCD", QSerialPort::DataCarrierDetectSignal},
        {"RI",  QSerialPort::RingIndicatorSignal}
#else
        {"CTS", QSerialPort::ClearToSend},
        {"DSR", QSerialPort::DataSetReady},
        {"DCD", QSerialPort::DataCarrierDetect},
        {"RI",  QSerialPort::RingIndicator}
#endif
    };
};
