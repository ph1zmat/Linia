// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Text_.h"
#include "Utils/Debug_.h"
#include "Utils/Log_.h"

static uint timeCounter;

// ����� ������ ��������������
volatile static uint timeStartMS = 0;

// ����� ���������� ����� ��������������
volatile static uint timePrevMS = 0;

pchar Debug::file[10] = { nullptr };
int   Debug::line[10] = { 0 };


void Debug::StartProfilingMS()
{
    timeStartMS = TIME_MS;
    timePrevMS = TIME_MS; //-V656
}


uint Debug::PointProfilingMS(pchar _file, int _line)
{
    uint d = TIME_MS - timePrevMS;
    LOG_WRITE("%s %d %d", _file, _line, d);
    timePrevMS = TIME_MS;

    return d;
}


void Debug::StartProfilingUS()
{
//    timeStart = TIME_US;
//    timePrev = timeStart;
}


uint Debug::PointProfilingUS(pchar  /*_file*/, int /*_line*/)
{
//    uint d = TIME_US - timePrev;
//    LOG_WRITE("%s:%d - %d us", _file, _line, d);
//    timePrev = TIME_US;
//    return d;
    
    return 0;
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
//    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
//    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
