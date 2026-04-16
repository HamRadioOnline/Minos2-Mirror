#ifndef QRZSERVERCOMMON_H
#define QRZSERVERCOMMON_H


#include <QString>
#include "PublishState.h"

extern const char * QRZ_BUTTON_ON_STYLE;
extern const char * QRZ_BUTTON_OFF_STYLE;

extern const char * QRA_LOOKUP_OK;

class QrzServer
{
public:
    QString routerName;
    QString app;
    QString publisherProgram;
    PublishState state;

};








#endif // QRZSERVERCOMMON_H
