﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/ComPort/ComPort.h"
#include "Communicator/ComPort/rs232.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "Windows/ConsoleRS232.h"
#include <cstring>
#include <chrono>
#include <thread>


using namespace std::chrono;


namespace ComPort
{
    static int connected_port = -1;     // 0 == COM1

    static String lastAnswer;

    static void (*callbackOnReceive)(pchar) = nullptr;

    static wxString ExtractMessage(char *);


    // Столько первых портов опрашивается
    static const int NUM_PORTS = 32;

    // Нумерация начинается с 0 : 0 == COM1
    bool PortIsExist(int);

    // Каждофреймовое обновление при подключённом устройстве
    void UpdateConnected();

    // Попытка приконнектиться в соотвествии с настройками
    bool TryConnect();

    bool IsConnected();

    int Receive(char *buffer, int size);
}


void ComPort::GetComports(std::vector<bool> &ports)
{
    ports.clear();

    for (int i = 0; i < NUM_PORTS; i++)
    {
        static const pchar mode = "8N1";

        if (RS232_OpenComport(i, 115200, mode, 0) == 0)
        {
            ports.push_back(true);

            RS232_CloseComport(i);
        }
        else
        {
            ports.push_back(false);
        }
    }
}


bool ComPort::TryConnect()
{
    for (int i = 0; i < NUM_PORTS; i++)
    {
        bool find_port = SET::GUI::serial_port_auto_find.Get();

        bool current_port = (i == SET::GUI::serial_port_num.Get());

        if (find_port || current_port)
        {
            if (RS232_OpenComport(i, 115200, "8N1", 0) == 0)
            {
                connected_port = i;

                milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()) + 1000ms;

                do
                {
                    UpdateConnected();
                } while (duration_cast<milliseconds>(system_clock::now().time_since_epoch()) < end);

                connected_port = -1;

                RS232_CloseComport(i);
            }
        }
    }

    return false;
}


pchar ComPort::NameOpenedPort()
{
    static char buffer[32];

    std::sprintf(buffer, "COM%d", connected_port + 1);

    return buffer;
}


void ComPort::Close()
{
    if (IsConnected())
    {
        RS232_CloseComport(connected_port);

        connected_port = -1;
    }
}


bool ComPort::IsConnected()
{
    return connected_port != -1;
}


void ComPort::Send(pchar buffer)
{
    if (IsConnected())
    {
        Send((void *)buffer, (int)std::strlen(buffer));
    }
}


void ComPort::Send(void *buffer, int num_bytes)
{
    if (IsConnected())
    {
        RS232_SendBuf(connected_port, (uint8 *)buffer, num_bytes);
    }
}


int ComPort::Receive(char *buffer, int size)
{
    if (IsConnected())
    {
        return RS232_PollComport(connected_port, reinterpret_cast<unsigned char *>(buffer), size);
    }

    return 0;
}


void ComPort::Update()
{
    if (IsConnected())
    {
        UpdateConnected();
    }
    else
    {
        if (TryConnect())
        {
            LOG_WRITE("Обнаружен карт-ридер на порту %s", NameOpenedPort());
        }
    }
}


void ComPort::UpdateConnected()
{
    if (IsConnected())
    {
        const int SIZE_BUFFER = 4096;
        static char buffer[SIZE_BUFFER] = { 0 };  // Принятые данные
        static int bytes_in_buffer = 0;

        static bool first = true;

        if (first)
        {
            first = false;
            std::memset(buffer, 0xFF, SIZE_BUFFER);
        }

        int n = ComPort::Receive(buffer + bytes_in_buffer, SIZE_BUFFER - bytes_in_buffer);

        if (n)
        {
            bytes_in_buffer += n;

            while (true)
            {
                wxString message = ExtractMessage(buffer);

                if (message.Length())
                {
                    lastAnswer.Set(message.c_str());

                    if (lastAnswer.GetWord(1, " ") != "Task")
                    {
                        if (message != "#OK")                       // Подтверждения отбрасываем, чтобы не засорять лог
                        {
                            ConsoleRS232::self->AddLine(String("%s%s", REVERSE_PROMT, buffer).c_str());

                            Log::FromDevice(buffer);
                        }

                        if (callbackOnReceive)
                        {
                            callbackOnReceive(buffer);
                        }
                    }

                    int remove_bytes = (int)message.Length() + 1;
                    bytes_in_buffer -= remove_bytes;

                    if (bytes_in_buffer)
                    {
                        std::memmove(buffer, buffer + remove_bytes, (uint)bytes_in_buffer);
                    }

                    std::memset(buffer + bytes_in_buffer, 0xFF, (uint)(SIZE_BUFFER - bytes_in_buffer));
                }
                else
                {
                    break;
                }
            }
        }
    }
}


wxString ComPort::ExtractMessage(char *buffer)
{
    if (SU::BeginWith(buffer, "#OK") ||
        SU::BeginWith(buffer, "#CARD") ||
        SU::BeginWith(buffer, "#LOG") ||
        SU::BeginWith(buffer, "#WARN") ||
        SU::BeginWith(buffer, "#ERROR") ||
        SU::BeginWith(buffer, "#DATA") ||
        SU::BeginWith(buffer, "#MAKE") ||
        SU::BeginWith(buffer, "#KEYSMFSL0"))
    {
        for (int i = 0; ; i++)
        {
            if (buffer[i] == '\n')
            {
                buffer[i] = '\0';

                return wxString(buffer);
            }

            if (buffer[i] == -1)
            {
                return "";
            }
        }
    }
    else
    {
        pchar pointer = SU::Consist(buffer, "#END\n");

        if (pointer)
        {
            int index = pointer - buffer + 4;
            buffer[index] = '\0';
            return wxString(buffer);
        }
    }

    return "";
}


void ComPort::WaitWord(int num_word, pchar word)
{
    while (lastAnswer.GetWord(num_word, " ") != word)
    {
        UpdateConnected();
    }

    lastAnswer.Set("");
}


void ComPort::CallbackOnReceive::Set(void (*callback)(pchar))
{
    callbackOnReceive = callback;
}


void ComPort::CallbackOnReceive::Reset()
{
}
