/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
//                      Montior Serial Control Lines for TX Cancel in Keyers
//                      Control Line can be allocated to voice or cw keyer.
//
// Copyright        (c) D. G. Balharrie M0DGB/G8FKH 2017 - 2026
//
//
//
/////////////////////////////////////////////////////////////////////////////



#include "serialportcontrollinewatcher.h"
#include <QDebug>




SerialPortControlLineWatcher::SerialPortControlLineWatcher(QSerialPort* port,
                                                           QObject* parent)
    : QObject(parent), serialPort(port)
{
    if (!serialPort) {
        qWarning() << "Null serial port";
        return;
    }

    // Initialise state maps
    for (auto it = lineMap.constBegin(); it != lineMap.constEnd(); ++it) {
        const QString& k = it.key();
        enabled[k] = false;
        lastState[k] = false;
        lineAssignments[k] = ControlFunction::None;
    }

    // Polling timer
    pollTimer.setInterval(50); // 50ms = 20 Hz
    connect(&pollTimer, &QTimer::timeout, this, &SerialPortControlLineWatcher::pollLines);
    pollTimer.start();
}

void SerialPortControlLineWatcher::setLineAllocation(const QString& line,
                                                     ControlFunction func)
{
    if (!lineMap.contains(line)) return;

    // Prevent assigning same function to multiple lines
    if (lineAssignments.values().contains(func)) return;

    lineAssignments[line] = func;
    enabled[line] = true;
}

void SerialPortControlLineWatcher::enableLine(const QString& line, bool en)
{
    if (lineMap.contains(line))
        enabled[line] = en;
}

void SerialPortControlLineWatcher::pollLines()
{
    if (!serialPort || !serialPort->isOpen()) return;

    auto pinSignals = serialPort->pinoutSignals();

    for (const auto& line : enabled.keys()) {
        if (!enabled[line]) continue;

        bool state = readLine(line, pinSignals);

        if (state != lastState[line]) {
            lastState[line] = state;
            emit controlLineTriggered(line, lineAssignments.value(line, ControlFunction::None), state);
        }
    }
}

bool SerialPortControlLineWatcher::readLine(const QString& line,
                                            QSerialPort::PinoutSignals pinSignals)
{
    auto it = lineMap.find(line);
    if (it == lineMap.end()) return false;

    return pinSignals.testFlag(it.value());
}
