// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


 // @brief ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32)
 // � ����� ������� 120.000.000 ����� ��� �8-53 � 90.000.000 ����� ��� �8-54.
 // ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
#define TIME_TICKS (HAL_TIM2::GetTicks())
#define TIME_US    (HAL_TIM2::TimeUS())
#define TIME_MS    (HAL_TIM::TimeMS())

namespace Timer
{
    struct Type
    {
        enum E 
        {
            kPressKey,                  // ����� �������������, ����� �������� ������� �������.
            kShowLevelRShiftA,          // ����� �������������, ����� ���������� ��������� ����� �������� ������ 1.
            kShowLevelRShiftB,          // ����� �������������, ����� ���������� ��������� ����� �������� ������ 2.
            kShowLevelTrigLev,          // ����� �������������, ����� ���������� ��������� ����� ������ �������������
            kENumSignalsInSec,          // ��� ��������� ���������� ���������� ������� � �������.
            kFlashDisplay,              // ������ ��� ��������� �������� ������ �����-������ �������.
            kShowMessages,              // ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
            kMenuAutoHide,              // ������ ��� ������� ������� ��������� ����.
            kRShiftMarkersAutoHide,     // ������ ��� ������� ������� ��������� �������������� ������� �����.
            kUSB,                       // ���������������, ��� ����� ����.
            kStopSound,                 // ��������� ����
            kTemporaryPauseFPGA,        // ��������� ����� ��� �������� ������� �� ������ ����� �������� �����
            kStrNaviAutoHide,           // ������� ������ ��������� ����
            kTimerStartP2P,             // ������ ��� ������� ���������� ����� ����������� ������
            kTimerDisplay,              // ������ �� ������ ��������� ������
            USB,
            kTemp,
            Count
        } value;
        Type(E v) : value(v) {}
        operator uint8() const { return (uint8)value; }
    };

    void Init();

    // ��������� ������� timer ������� � ����� ������������
    void Set(Type type, pFuncVV func, uint dTms);

    void SetAndStartOnce(Type type, pFuncVV func, uint dTms);

    void SetAndEnable(Type type, pFuncVV func, uint dTms);

    void StartOnce(Type type);

    void Enable(Type type);

    void Disable(Type type);

    bool IsRun(Type type);

    void PauseOnTicks(uint numTicks);

    // ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����
    void StartLogging();

    uint LogPointUS(char *name);

    uint LogPointMS(char *name);

    // ���������� ��� ������������ �������
    void ElapsedCallback();
};


struct TimeMeterMS
{
    TimeMeterMS() { Reset(); }
    void Reset();
    uint ElapsedTime();
    void Delay(uint timeMS);
private:
    uint time_reset;
};
