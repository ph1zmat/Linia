// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Log.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <limits>
#include <stm32f407xx.h>


struct TimerStruct
{
    pFuncVV func;       // ������� �������
    uint    dTms;          // ������ ������������, ��
    uint    timeNextMS;    // ����� ���������� ������������. ���� == 0xffffffff, �� ������ ���������
    bool    repeat;        // ���� true, ����� �����������, ���� �� ����� ������� ������� Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};



static TimerStruct timers[Timer::Type::Count];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;



#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)



// ������� �����, ������� ����������� � timeStop ��
static void StartTIM(uint timeStop);

static void StopTIM();
// ���������� ����� ������������ ���������� �������, ���� 0, ���� �������� ���
static uint NearestTime();
// ��������� ������� �� ������
static void TuneTIM(Timer::Type type);


bool Timer::IsRun(Type type)
{
    return TIME_NEXT(type) != MAX_UINT;
}


void Timer::Init()
{
    for(uint i = 0; i < Timer::Type::Count; i++)
    {
        timers[i].timeNextMS = MAX_UINT;
    }

    HAL_TIM2::Init();
    HAL_TIM3::Init();
}


void Timer::ElapsedCallback()
{
    uint time = TIME_MS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < Timer::Type::Count; type++)
    {
        if (TIME_NEXT(type) <= time)            // ���� ������ ����� ������������
        {
            TimerStruct *timer = &timers[type];
            timer->func();
            if (timer->repeat)
            {
                do      // ���� ����� ������, ��� ��������� ������ SysTick, ������� ����������� ������������, ����� ��������� ���������,
                {       // � ���� ������� ����������� ������, ��� timer->dTm ��, �� ��� ��� ��������
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < TIME_MS);

            }
            else
            {
                timer->timeNextMS = MAX_UINT;
            }
        }
    }

    StartTIM(NearestTime());
}


void Timer::Set(Type type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}


void Timer::SetAndStartOnce(Type type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}


void Timer::SetAndEnable(Type type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}


void Timer::StartOnce(Type type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}


void Timer::Enable(Type type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}


static void TuneTIM(Timer::Type type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = TIME_MS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // ���� ������ ������ ��������� ������ ��������
    {
        StartTIM(timeNext);         // �� ������� ������ �� ���� �����
    }
}


void Timer::Disable(Type type)
{
    timers[type].timeNextMS = MAX_UINT;
    timers[type].repeat = false;
}


static uint NearestTime()
{
    uint time = MAX_UINT;

    for(uint type = 0; type < Timer::Type::Count; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}


static void StartTIM(uint timeStopMS)
{
    StopTIM();

    if(timeStopMS == MAX_UINT)
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    // 10 ������������� 0.1��. �.�. ���� ��� ����� 1��, ����� �������� (100 - 1)
    HAL_TIM3::StartIT((dT * 2) - 1);
}


static void StopTIM()
{
    HAL_TIM3::StopIT();
}


void Timer::PauseOnTicks(uint numTicks)
{
    volatile uint startTicks = TIME_TICKS;
    while (TIME_TICKS - startTicks < numTicks)
    {
    }
}


void Timer::StartLogging()
{
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}


uint Timer::LogPointUS(char * /*name*/)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
//    LOG_WRITE("%s %.2f us", name, interval / 120.0);
    return interval;
}


uint Timer::LogPointMS(char * /*name*/) //-V524
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
//    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}


void TimeMeterMS::Delay(uint timeMS)
{
    Reset();

    while (ElapsedTime() < timeMS)
    {
    }
}

#undef TIME_NEXT
