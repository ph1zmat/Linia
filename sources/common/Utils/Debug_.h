// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ������ ������������ ��� �����������, ���� ����� � HardFault_Handler() ����������� ��� ����������
//#define START_PROFILING_US Debug::StartProfilingUS()
//#define POINT_PROFILING_US Debug::PointProfilingUS(__FILE__, __LINE__)
//
//#define START_PROFILING_MS Debug::StartProfilingMS()
//#define POINT_PROFILING_MS Debug::PointProfilingMS(__FILE__, __LINE__)
//
//#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;
//
//#define DEBUG_POINT_0  DEBUG_POINT(0)
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//extern void HardFault_Handler();
//#ifdef __cplusplus
//}
//#endif


namespace Debug
{
    void StartProfilingUS();
    uint PointProfilingUS(pchar file, int line);

    void StartProfilingMS();
    uint PointProfilingMS(pchar file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line[10];
    extern pchar file[10];
};
