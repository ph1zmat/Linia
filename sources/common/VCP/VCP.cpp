// 2025/03/19 10:57:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "VCP/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math_.h"
#include "SCPI/SCPI.h"
#include "USBD/usbd_cdc_interface.h"
#include <usbd_core.h>
#include <usbd_cdc.h>
#include <cstdarg>
#include <cstring>


namespace VCP
{
    USBD_HandleTypeDef handleUSBD;
    PCD_HandleTypeDef  handlePCD;
    bool               cableIsConnected = false;
    bool               isConnected = false;

    static bool PrevSendingComplete();

    static void SendDataAsynch(uint8 *data, int size);
}


void VCP::Init()
{
    USBD_Init(&handleUSBD, &VCP_Desc, 0);
    USBD_RegisterClass(&handleUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handleUSBD, &USBD_CDC_fops);
    USBD_Start(&handleUSBD);
} 


bool VCP::PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    return pCDC->TxState == 0;
}


void VCP::SendDataAsynch(uint8 *buffer, int size)
{
#define SIZE_BUFFER 64
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::Min(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {}
    std::memcpy(trBuf, buffer, (uint)size);

    USBD_CDC_SetTxBuffer(&handleUSBD, trBuf, (uint16)size);
    USBD_CDC_TransmitPacket(&handleUSBD);
}


#define SIZE_BUFFER_VCP 256     // \todo ���� ��������� ������ ������ 512, �� �� ��207 �����
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;


void VCP::Flush()
{
    if (sizeBuffer)
    {
        volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
        while (pCDC->TxState == 1) {}
        USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, (uint16)sizeBuffer);
        USBD_CDC_TransmitPacket(&handleUSBD);
        while (pCDC->TxState == 1) {}
    }

    sizeBuffer = 0;
}


void VCP::SendBufferSynch(const uint8 *buffer, int size)
{
    if (isConnected)
    {
        volatile USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *)handleUSBD.pClassData;
    
        do 
        {
            if (sizeBuffer + size > SIZE_BUFFER_VCP)
            {
                int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
                LIMITATION(reqBytes, 0, size)
                while (pCDC->TxState == 1) {}
                std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)reqBytes);
                USBD_CDC_SetTxBuffer(&handleUSBD, buffSend, SIZE_BUFFER_VCP);
                USBD_CDC_TransmitPacket(&handleUSBD);
                size -= reqBytes;
                buffer += reqBytes;
                sizeBuffer = 0;
            }
            else
            {
                std::memcpy(buffSend + sizeBuffer, (void *)buffer, (uint)size);
                sizeBuffer += size;
                size = 0;
            }
        } while (size);
    }
}


void VCP::SendStringAsynch0D0A(char *format, ...)
{
    if (isConnected)
    {
        static char buffer[200];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        std::strcat(buffer, "\r\n");
        SendDataAsynch((uint8 *)buffer, (int)std::strlen(buffer));
    }
}


void VCP::SendStringAsynchRAW(char *format, ...)
{
    if (isConnected)
    {
        static char buffer[200];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        SendDataAsynch((uint8 *)buffer, (int)std::strlen(buffer));
    }
}
