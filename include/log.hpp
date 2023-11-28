#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <cstdarg>
#include <stdio.h>
#include <time.h>

#define KNRM(x) "\033[0m" x "\033[0m"
#define KRED(x) "\033[31m" x "\033[0m"
#define KGRN(x) "\033[32m" x "\033[0m"
#define KYEL(x) "\033[33m" x "\033[0m"
#define KBLU(x) "\033[34m" x "\033[0m"
#define KMAG(x) "\033[35m" x "\033[0m"
#define KCYN(x) "\033[36m" x "\033[0m"
#define KWHT(x) "\033[37m" x "\033[0m"

class Logger
{
    private:
    typedef enum
    {
        LG_DEBUG,
        LG_INFO,
        LG_WARNING,
        LG_ERROR,
    } loglevel_e;

    inline static void getTimestamp(char *timestamp)
    {
        time_t     rawtime;
        struct tm *info;

        time(&rawtime);
        info = localtime(&rawtime);

        strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", info);
    }
    inline static void log(loglevel_e level, const char *format, va_list args)
    {
        char timestamp[20];
        Logger::getTimestamp(timestamp);

        switch (level)
        {
            case LG_DEBUG: {
                fprintf(stdout, KCYN("[%s] [%s]") " ", timestamp, "DEBUG");
                break;
            }
            case LG_INFO: {
                fprintf(stdout, KWHT("[%s] [%s]") " ", timestamp, "INFO");
                break;
            }
            case LG_WARNING: {
                fprintf(stdout, KYEL("[%s] [%s]") " ", timestamp, "WARNING");
                break;
            }
            case LG_ERROR: {
                fprintf(stdout, KRED("[%s] [%s]") " ", timestamp, "ERROR");
                break;
            }
        }

        vfprintf(stdout, format, args);
        fprintf(stdout, "\n");
    };

    public:
    inline static void debug(const char *format, ...)
    {
        std::va_list args;
        va_start(args, format);
        Logger::log(LG_DEBUG, format, args);
        va_end(args);
    };

    inline static void info(const char *format, ...)
    {
        std::va_list args;
        va_start(args, format);
        Logger::log(LG_INFO, format, args);
        va_end(args);
    };
    inline static void warning(const char *format, ...)
    {
        std::va_list args;
        va_start(args, format);
        Logger::log(LG_WARNING, format, args);
        va_end(args);
    };

    inline static void error(const char *format, ...)
    {
        std::va_list args;
        va_start(args, format);
        Logger::log(LG_ERROR, format, args);
        va_end(args);
    };
};

#endif // !_LOGGER_H_
