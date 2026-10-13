#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define ANSI_RESET   "\033[0m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_CYAN    "\033[36m"

typedef enum {
    INFO,
    SUCCESS,
    WARNING,
    ERROR
} LogLevel;

void log_message(LogLevel level, const char *fmt, ...){
    const char *color;
    const char *label;

    switch (level) {
        case SUCCESS:
            color = ANSI_GREEN;
            label = "SUCCESS";
            break;
        case WARNING:
            color = ANSI_YELLOW;
            label = "WARNING";
            break;
        case ERROR:
            color = ANSI_RED;
            label = "ERROR";
            break;
        default:
            color = ANSI_CYAN;
            label = "INFO";
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    printf("%s[%02d:%02d:%02d] [%s] ", color, t->tm_hour, t->tm_min, t->tm_sec, label);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("%s\n", ANSI_RESET);
}

#endif