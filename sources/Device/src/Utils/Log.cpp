// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


void Log::AddString(char *format, ...) //-V2560
{
    char string[100];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(string, format, args);
    va_end(args);
}


void Log::Trace(char *file, int line, char *format, ...) //-V2560
{
    char message[200];
    std::sprintf(message, "%s:%d ", file, line);

    char text[100];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(text, format, args);
    va_end(args);

    std::strcat(message, text);
}
