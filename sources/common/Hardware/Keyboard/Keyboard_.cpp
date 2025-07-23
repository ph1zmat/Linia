// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


namespace Keyboard
{
#define TIME_UPDATE_KEYBOARD 2   // ����� ����� �������� ����������
#define NUM_RL 5
#define NUM_SL 5

#define PIN_SL0  GPIO_PIN_10    // 69   PA10        SL0
#define PORT_SL0 GPIOA

#define PIN_SL1  GPIO_PIN_8     // 67   PA8         SL1
#define PORT_SL1 GPIOA

#define PIN_SL2  GPIO_PIN_9     // 66   PC9         SL2
#define PORT_SL2 GPIOC

#define PIN_SL3  GPIO_PIN_8     // 65   PC8         SL3
#define PORT_SL3 GPIOC

#define PIN_SL4  GPIO_PIN_12    // 80   PC12        SL4
#define PORT_SL4 GPIOC

#define PIN_RL0  GPIO_PIN_7     // 64   PC7         RL0
#define PORT_RL0 GPIOC

#define PIN_RL1  GPIO_PIN_6     // 63   PC6         RL1
#define PORT_RL1 GPIOC

#define PIN_RL2  GPIO_PIN_15    // 62   PD15        RL2
#define PORT_RL2 GPIOD

#define PIN_RL3  GPIO_PIN_14    // 61   PD14        RL3
#define PORT_RL3 GPIOD

#define PIN_RL4  GPIO_PIN_11    // 79   PC11        RL4
#define PORT_RL4 GPIOC

#define PIN_ENC1  GPIO_PIN_11   // 58   PD11        ENC1
#define PORT_ENC1 GPIOD

#define PIN_ENC2  GPIO_PIN_12   // 59   PD12        ENC2
#define PORT_ENC2 GPIOD

#define PIN_ENCBUT GPIO_PIN_13  // 60   PD13        ENCBUT
#define PORT_ENCBUT GPIOD


    static TIM_HandleTypeDef handleTIM4;


    static const Key::E keys[NUM_SL][NUM_RL] =
    {//     RL0         RL1         RL2         RL3        RL4
        {Key::_1,    Key::_2,    Key::_3,    Key::_4,   Key::_5},       // SL0
        {Key::_6,    Key::_7,    Key::_8,    Key::_9,   Key::None},     // SL1
        {Key::_0,    Key::Dot,   Key::Minus, Key::None, Key::None},     // SL2
        {Key::Left,  Key::Right, Key::OK,    Key::Esc,  Key::None},     // SL3
        {Key::Start, Key::Stop,  Key::None,  Key::None, Key::None}      // SL4
    };

    // ������� ��������� - ����� ��� ������� ������� ����������
#define MAX_KEYS 100
    static Key::E controls[MAX_KEYS];
    // ���������� ��� ��������� ���������
    static int numActions = 0;

    // ������������� � true �������� ��������, ��� ��������� ����-���� ���������� � � ��������� ������ �� ���������� (����� �� ���� ������ ������������ ���� Long
    static bool alreadyLong[NUM_SL][NUM_RL];
    // ��� ����������� ������� ������ ���� ������������ ����� �������
    static uint timePress[NUM_SL][NUM_RL];

    // ����� ���������� ����������� �����
    static volatile uint prevRepeat = 0;
//    static volatile uint prevPause = 0;

    // ���������� ��������� ����
    static void Set_SL(int, int);
    // ���������� ��������� ���� ����� � ���� ���������
    static void Set_All_SL(int);
    // ���������� ��������� ���� rl
    static int Read_RL(int rl);
    // ���������������� ����
    static void InitPins();
    // ���������������� ������ ��� �������������� ������ ����������
    static void InitTimer();
    // �������, ������������ ���������� �� ���������� �������
    static void Update();
    // ��������� �����
    static void DetectRegulator();

    static bool KeyboardCheck();

#define BUTTON_IS_PRESS(state) ((state) == 0)
}


void Keyboard::Update()
{
    uint time = HAL_GetTick();

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Key::E key = keys[sl][rl];

            if (key != Key::None)
            {
                if (timePress[sl][rl])                      // ���� ������� ��������� � ������� ���������
                {
                    if (time - timePress[sl][rl] > 100)     // ���� ������ ����� 100 �� � ������� �������
                    {
                        if (!BUTTON_IS_PRESS(state))        // ���� ������ ������ ��������� � ������� ���������
                        {
                            timePress[sl][rl] = 0;
                            if (!alreadyLong[sl][rl])
                            {
                            }
                            alreadyLong[sl][rl] = false;
                            prevRepeat = 0;
                        }
                        else if (time - timePress[sl][rl] > 500 && !alreadyLong[sl][rl])
                        {
                            alreadyLong[sl][rl] = true;
                        }
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !alreadyLong[sl][rl])
                {
                    timePress[sl][rl] = time;
                    AddKey(key);
                    prevRepeat = 0;
                }
            }
        }

        Set_All_SL(1);
    }

    DetectRegulator();
    Set_All_SL(1);
}

void Keyboard::DetectRegulator()
{
    // ����������� �������
    static bool prevStatesIsOne = false;

    bool stateLeft = (HAL_GPIO_ReadPin(PORT_ENC1, PIN_ENC1) == GPIO_PIN_SET);
    bool stateRight = (HAL_GPIO_ReadPin(PORT_ENC2, PIN_ENC2) == GPIO_PIN_SET);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        prevStatesIsOne = false;

        AddKey(Key::GovLeft);
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        prevStatesIsOne = false;

        AddKey(Key::GovRight);
    }

    {
        static bool prev_button = false;

        static TimeMeterMS meter;

        if (meter.ElapsedTime() < 100)
        {
            return;
        }

        bool state = (HAL_GPIO_ReadPin(PORT_ENCBUT, PIN_ENCBUT) == GPIO_PIN_RESET);

        if (state != prev_button)
        {
            prev_button = state;

            if (state)
            {
                AddKey(Key::GovButton);
            }

            meter.Reset();
        }
    }
}

bool Keyboard::KeyboardCheck()
{
    bool keyboardFail = false;

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Key::E key = keys[sl][rl];

            if (key != Key::None)
            {
                if (BUTTON_IS_PRESS(state))
                {
                    keyboardFail = true;
                }
            }
        }

        Set_All_SL(1);
    }
    return keyboardFail;
}

bool Keyboard::Init()
{
    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            timePress[sl][rl] = 0;
        }
    }

    InitPins();
    InitTimer();
    return KeyboardCheck();
}


void Keyboard::Set_All_SL(int st)
{
    for (int i = 0; i < NUM_SL; i++)
    {
        Set_SL(i, st);
    }
}


void Keyboard::Set_SL(int bus, int st)
{
    static GPIO_TypeDef * const ports[NUM_SL] = { PORT_SL0, PORT_SL1, PORT_SL2, PORT_SL3, PORT_SL4 };
    static const uint16 pins[NUM_SL] = { PIN_SL0,  PIN_SL1,  PIN_SL2,  PIN_SL3, PIN_SL4 };
    static const GPIO_PinState state[2] = { GPIO_PIN_RESET, GPIO_PIN_SET };

    HAL_GPIO_WritePin((GPIO_TypeDef *)ports[bus], pins[bus], state[st]); //-V2567
}


int Keyboard::Read_RL(int rl)
{
    static GPIO_TypeDef * const ports[NUM_RL] = { PORT_RL0, PORT_RL1, PORT_RL2, PORT_RL3, PORT_RL4 };
    static const uint16 pins[NUM_RL] = { PIN_RL0,  PIN_RL1,  PIN_RL2, PIN_RL3, PIN_RL4 };

    return HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[rl], pins[rl]); //-V2567
}


void Keyboard::InitPins()
{
    GPIO_InitTypeDef is =
    {
        PIN_SL0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        0
    };

    HAL_GPIO_Init(PORT_SL0, &is); //-V525

    is.Pin = PIN_SL1;
    HAL_GPIO_Init(PORT_SL1, &is);

    is.Pin = PIN_SL2;
    HAL_GPIO_Init(PORT_SL2, &is);

    is.Pin = PIN_SL3;
    HAL_GPIO_Init(PORT_SL3, &is);

    is.Pin = PIN_SL4;
    HAL_GPIO_Init(PORT_SL4, &is);

    is.Mode = GPIO_MODE_INPUT;

    is.Pin = PIN_RL0;
    HAL_GPIO_Init(PORT_RL0, &is);

    is.Pin = PIN_RL1;
    HAL_GPIO_Init(PORT_RL1, &is);

    is.Pin = PIN_RL2;
    HAL_GPIO_Init(PORT_RL2, &is);

    is.Pin = PIN_RL3;
    HAL_GPIO_Init(PORT_RL3, &is);

    is.Pin = PIN_RL4;
    HAL_GPIO_Init(PORT_RL4, &is);

    is.Pin = PIN_ENC1;
    HAL_GPIO_Init(PORT_ENC1, &is);

    is.Pin = PIN_ENC2;
    HAL_GPIO_Init(PORT_ENC2, &is);

    is.Pin = PIN_ENCBUT;
    HAL_GPIO_Init(PORT_ENCBUT, &is);
}


void Keyboard::InitTimer()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    // �������������� ������, �� ����������� �������� ����� ���������� ����������
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE_KEYBOARD * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    if (HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    Set_All_SL(1);
}


void Keyboard::AddKey(Key::E key)
{
    controls[numActions++] = key;
}


bool Keyboard::Empty()
{
    return numActions == 0;
}



Key::E Keyboard::NextKey()
{
    if (Empty())
    {
        return Key::Count;
    }

    Key::E result = controls[0];

    for (int i = 1; i < numActions; i++)
    {
        controls[i - 1] = controls[i];
    }

    --numActions;
    return result;
}


#ifdef __cplusplus
extern "C" {
#endif

    void TIM4_IRQHandler();

    void TIM4_IRQHandler()
    {
        HAL_TIM_IRQHandler(&Keyboard::handleTIM4);
    }

#ifdef __cplusplus
}
#endif


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //-V2009 //-V2558
{
    if (htim == &Keyboard::handleTIM4)
    {
        Keyboard::Update();
    }
}


void Keyboard::Lock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncEmptyUpdate);
}


void Keyboard::Unlock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}


pchar Key::Name(E value)
{
    static pchar names[Key::Count] =
    {
        "None",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "0",
        "-",
        ".",
        "START",
        "<-",
        "->",
        "ESC",
        "OK",
        "STOP",
        "",
        "",
        ""
    };

    return names[value];
}
