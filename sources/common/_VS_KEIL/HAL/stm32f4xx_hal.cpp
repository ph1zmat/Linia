#include "defines.h"
#include <stm32f4xx_hal.h>
#include <ctime>



uint32_t HAL_GetTick()
{
#ifdef GUI
    return (uint)std::clock();
#else
    return 0;
#endif
}


void HAL_Delay(uint32_t time)
{
    volatile uint timeStart = HAL_GetTick();

    while (HAL_GetTick() - timeStart < time) //-V776
    {

    }
}


HAL_StatusTypeDef HAL_Init()
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_DeInit()
{
    return HAL_OK;
}


void HAL_IncTick()
{
}
