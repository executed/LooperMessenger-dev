#ifndef STRING_CONSTANTS_H
#define STRING_CONSTANTS_H

#include <QtCore>

class STRING_CONSTANTS
{

public:
    //main
    const char* DEBUG_REGEX = "Debug: %s (%s:%u, %s)\n";
    const char* WARNING_REGEX = "Warning: %s (%s:%u, %s)\n";
    const char* CRITICAL_REGEX = "Critical: %s (%s:%u, %s)\n";
    const char* FATAL_REGEX = "Fatal: %s (%s:%u, %s)\n";

    //dialog
    const char* NO_CLIENTS = "No clients connected";
    const char* PM_SENT = "Sent private server message to ";
    const char* SERV_STARTED = " server started at ";
    const char* SERV_NOT_STARTED = " server not strated at ";
    const char* SERV_STOPPED = " server stopped at ";
    const char* PB_START_SERV = "Start server";
    const char* PB_STOP_SERV = "Stop server";
    const char* INVALID_ADDR = " invalid address ";
};

#endif //STRING_CONSTANTS_H
