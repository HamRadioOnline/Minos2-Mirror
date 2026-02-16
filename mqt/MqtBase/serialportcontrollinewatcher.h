#ifndef SERIALPORTCONTROLLINEWATCHER_H
#define SERIALPORTCONTROLLINEWATCHER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QMap>

class SerialPortControlLineWatcher : public QObject
{
    Q_OBJECT
public:
    enum class ControlFunction {
        None,
        Voice,
        CW
    };
    Q_ENUM(ControlFunction)

    explicit SerialPortControlLineWatcher(QObject* parent = nullptr);
    ~SerialPortControlLineWatcher() override;

    // Set or change COM port dynamically
    void setComPort(const QString& portName);

    // Assign a line to Voice/CW
    void setLineAllocation(const QString& lineName, ControlFunction func);
    void setInvert(ControlFunction func, bool inv);

    // Enable/disable a line
    void enableLine(const QString& lineName, bool enable);
    bool isLineEnabled(const QString &line);

    // Reset all lines (useful when switching keyer type)
    void resetLines();

    void startTimer();
    void stopTimer();
    bool isTimerActive();



signals:
signals:
    void controlLineTriggered(ControlFunction func);          // rising edge only
    void controlLineLevelChanged(ControlFunction func, bool); // level for indicator




private slots:
    void pollLines();

private:
    bool readLine(const QString& line, const QSerialPort::PinoutSignals& pinSignals);

    QSerialPort* serialPort = nullptr;
    QTimer pollTimer;

    // Line state tracking
    QMap<QString, ControlFunction> lineAssignments;
    QMap<QString, bool> enabled;
    QMap<QString, bool> lastState;
    QMap<ControlFunction, bool> invertFunction;


    // Line → QSerialPort::PinoutSignal map
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




#endif // SERIALPORTCONTROLLINEWATCHER_H
