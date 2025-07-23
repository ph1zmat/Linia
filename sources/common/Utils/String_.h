// 2022/03/30 08:25:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#define DEFAULT_SIZE_STRING 64

template<int capacity = DEFAULT_SIZE_STRING>
class String
{
public:

    String() { Clear(); }

    String(pchar format, ...)
    {
        std::va_list args;
        va_start(args, format);
        int num_symbols = std::vsprintf(buffer, format, args);
        va_end(args);

        if (num_symbols < 0 || num_symbols > capacity - 1)
        {
            LOG_ERROR("Very small string buffer %d, need %d:", capacity, num_symbols);
        }
    }

    void SetFormat(pchar format, ...);

    void AppendFormat(pchar format, ...);

    char *c_str() const { return (char *)&buffer[0]; }

    int Size() const { return (int)std::strlen(buffer); }

    void Append(pchar);

    void Clear() { buffer[0] = '\0'; }

    void Append(char);

    void Append(const String<capacity> &rhs) { Append(rhs.c_str()); }

    // �������� �����. � ����� �������� ����
    void AppendBytes(const void *bytes, int num_bytes);

    bool Filled() const { return (Size() == capacity - 1); }

    String<capacity> GetWord(const int num_word);

    char &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static char null_char = 0;

        return null_char;
    }

    bool operator==(pchar string)
    {
        return std::strcmp(buffer, string) == 0;
    }

    bool ToInt(int *out);

private:

    char buffer[capacity];
};
