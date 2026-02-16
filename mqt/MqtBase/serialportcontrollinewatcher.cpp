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

SerialPortControlLineWatcher::SerialPortControlLineWatcher(QObject* parent)
    : QObject(parent)
{
    // Initialize maps
    for (const auto& k : lineMap.keys())
    {
        enabled[k] = false;
        lastState[k] = false;
        lineAssignments[k] = ControlFunction::None;
    }

    invertFunction[ControlFunction::Voice] = false;
    invertFunction[ControlFunction::CW] = false;


    // Polling timer
    pollTimer.setInterval(50); // 20 Hz
    connect(&pollTimer, &QTimer::timeout, this, &SerialPortControlLineWatcher::pollLines);

}

void SerialPortControlLineWatcher::setInvert(ControlFunction func, bool inv)
{
    invertFunction[func] = inv;
}


void SerialPortControlLineWatcher::startTimer()
{
    pollTimer.start();
}

void SerialPortControlLineWatcher::stopTimer()
{
    pollTimer.stop();
}

bool SerialPortControlLineWatcher::isTimerActive()
{
    return pollTimer.isActive();
}

SerialPortControlLineWatcher::~SerialPortControlLineWatcher()
{
    if (serialPort)
    {
        serialPort->close();
        serialPort->deleteLater();
    }
}

void SerialPortControlLineWatcher::setComPort(const QString& portName)
{
    // Clean up old port
    if (serialPort)
    {
        serialPort->close();
        serialPort->deleteLater();
    }

    if (portName.isEmpty())
    {
        serialPort = nullptr;
        return;
    }

    // Create new port
    serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);

    if (!serialPort->open(QIODevice::ReadWrite))
    {
        qWarning() << "Failed to open serial port:" << portName;
        serialPort->deleteLater();
        serialPort = nullptr;
    }
    else
    {
        qDebug() << "Serial port opened:" << portName;
    }
}

void SerialPortControlLineWatcher::setLineAllocation(const QString& line, ControlFunction func)
{
    if (!lineMap.contains(line))
    {
         return;
    }

    // Prevent assigning the same function to multiple lines
    if (lineAssignments.values().contains(func))
    {
        return;
    }

    lineAssignments[line] = func;
    enabled[line] = true;
}

void SerialPortControlLineWatcher::enableLine(const QString& line, bool en)
{
    if (!lineMap.contains(line))
    {
        return;
    }

    enabled[line] = en;
}

bool SerialPortControlLineWatcher::isLineEnabled(const QString &line)
{
    return enabled[line];
}

void SerialPortControlLineWatcher::resetLines()
{
    for (const auto& k : lineMap.keys())
    {
        enabled[k] = false;
        lastState[k] = false;
        lineAssignments[k] = ControlFunction::None;
    }
}

void SerialPortControlLineWatcher::pollLines()
{
    if (!serialPort || !serialPort->isOpen())
        return;

    auto pinSignals = serialPort->pinoutSignals();

    for (const auto& line : enabled.keys())
    {
        if (!enabled[line])
            continue;

        bool state = readLine(line, pinSignals);

        // Map line → logical function
        ControlFunction func =
            lineAssignments.value(line, ControlFunction::None);

        // Apply invert per function
        if (invertFunction.value(func, false))
            state = !state;

        // Level change → indicator update
        if (state != lastState[line]) {
            emit controlLineLevelChanged(func, state);
        }

        // Rising edge → trigger cancel keyer
        if (state && !lastState[line]) {
            emit controlLineTriggered(func);
        }


        lastState[line] = state;
    }
}

bool SerialPortControlLineWatcher::readLine(const QString& line, const QSerialPort::PinoutSignals& pinSignals)
{
    auto it = lineMap.find(line);
    if (it == lineMap.end())
    {
        return false;
    }

    return pinSignals.testFlag(it.value());
}
