// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


#define TRACE_HANDLER                   
/*
    Console::AddString(__FUNCTION__);
*/

#ifdef __cplusplus
extern "C" {
#endif

    
    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }

    void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *)
    {
        TRACE_HANDLER;
//        Console::AddInt((int)handle->ErrorCode);
    }


    void MemManage_Handler()
    {
        TRACE_HANDLER;
        while (1)
        {
        }
    }

    
    void UsageFault_Handler()
    {
        TRACE_HANDLER;
        while (1)
        {
        }
    }

    
    void HardFault_Handler()
    {
        TRACE_HANDLER;
        while (1)
        {
        }
    }

    
    void BusFault_Handler()
    {
        TRACE_HANDLER;
        while (1)
        {
        }
    }

    
    void PendSV_Handler()
    {
    }

    
    void NMI_Handler()
    {
    }

    
    void SVC_Handler()
    {
    }

    
    void DebugMon_Handler()
    {
    }


    void TIM3_IRQHandler()
    {
        if((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM3->SR = ~TIM_DIER_UIE;
                Timer::ElapsedCallback();
            }
        }
    }

#ifdef __cplusplus
}
#endif
