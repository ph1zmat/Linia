// 2023/09/08 21:07:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN

#include "defines.h"
#include "Utils/StringUtils_.h"
#include "Utils/Stack_.h"
#include "Utils/Buffer_.h"
#include <cstdio>

#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable:4668)
#endif

#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstring>

#ifdef WIN32
    #pragma warning(pop)
#endif

#ifndef LANG
    #define LANG 0
    #define LANG_RU true
#endif


// ���������� false, ���� ����� ���������� - ������ ���������.
static bool ChooseSymbols(pchar *string);
// ���������� false, ���� ����� ���������� - ������ ���������.
static bool ChooseSpaces(pchar *string);


bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint length = std::strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}


#define  SYMBOL(x) (*(*(x)))


static bool ChooseSymbols(pchar *string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) != ' ' && SYMBOL(string) != 0x0d && SYMBOL(string + 1) != 0x0a)
    {
        (*string)++;
    }

    return true;
}


static bool ChooseSpaces(pchar *string)
{
    if (SYMBOL(string) == 0x0d && SYMBOL(string + 1) == 0x0a)
    {
        return false;
    }

    while (SYMBOL(string) == ' ')
    {
        (*string)++;
    }

    return true;
}

#undef SYMBOL


bool SU::GetWord(pchar string, Word *word, const int numWord)
{
    ChooseSpaces(&string);

    int currentWord = 0;

    while (true)
    {
        if (currentWord == numWord)
        {
            word->address = const_cast<char *>(string); //-V2567
            ChooseSymbols(&string);
            word->numSymbols = static_cast<int8>(string - word->address);

            char *pointer = word->address;
            int numSymbols = word->numSymbols;
            for (int i = 0; i < numSymbols; i++)
            {
                *pointer = static_cast<char>(std::toupper(*pointer));
                pointer++;
            }
            return true;
        }
        if (ChooseSymbols(&string))
        {
            currentWord++;
        }
        else
        {
            return false;
        }
        ChooseSpaces(&string);
    }
}


bool SU::WordEqualZeroString(Word *word, pchar string)
{
    pchar ch = string;
    char *w = word->address;

    while (*ch != 0)
    {
        if (*ch++ != *w++)
        {
            return false;
        }
    }

    return (ch - string) == word->numSymbols;
}


bool SU::EqualsZeroStrings(char *str1, char *str2)
{
    while ((*str1) == (*str2))
    {
        if ((*str1) == '\0')
        {
            return true;
        }
        str1++;
        str2++;
    }
    return false;
}


bool SU::EqualsStrings(uint8 *str1, pchar  const str2, int size)
{
    return EqualsStrings(static_cast<void *>(str1), const_cast<char *>(str2), size); //-V2567
}


bool SU::EqualsStrings(void *_str1, void *_str2, int size)
{
    char *str1 = static_cast<char *>(_str1);
    char *str2 = static_cast<char *>(_str2);

    for (int i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


bool SU::EqualsStrings(pchar str1, pchar str2)
{
    uint size = std::strlen(str1);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}


int SU::FirstNotNumeral(char *buffer)
{
    int result = 0;

    while (*buffer++)
    {
        char symbol = *buffer;

        if ((symbol < 0x30 || symbol > 0x39) && symbol != '.')
        {
            break;
        }

        result++;
    }

    return result;
}


float SU::StringToFloat(pchar string)
{
    float result = 0.0F;

    Stack<int8> stack(20);

    while (*string)
    {
        char symbol = *string;
        if (symbol < 0x30 || symbol > 0x39)
        {
            break;
        }
        stack.Push(symbol & 0x0f);
        string++;
    }

    {
        int pow = 1;

        while (stack.Size() > 0)
        {
            result += static_cast<float>(pow) * (float)stack.Pop();
            pow *= 10;
        }
    }

    // ������ � result ����� ����� �����

    if (*string == '.')
    {
        string++;

        float pow = 0.1F;

        while (*string)
        {
            char symbol = *string;
            if (symbol < 0x30 || symbol > 0x39)
            {
                break;
            }
            result += pow * (float)(symbol & 0x0f);
            pow /= 10.0F;
            string++;
        }
    }


    return result;
}


char *SU::ToUpper(char *str)
{
    return ToUpper(str, static_cast<int>(std::strlen(str)));
}


char *SU::ToUpper(char *_str, int size)
{
    char *str = static_cast<char *>(_str);

    for(int i = 0; i < size; i++)
    {
        str[i] = static_cast<char>(std::toupper(str[i]));
    }

    return str;
}


char SU::ToUpper(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s == 0x40)        // �
    {
        return '\xc5';
    }
    else if(s >= 0x60 && s <= 0x7a)
    {
        return static_cast<char>(s - 0x20);
    }
    else if(s >= 0xf0)
    {
        return static_cast<char>(s - 0x20);
    }

    return symbol;
}


char SU::ToLower(char symbol)
{
    uint8 s = static_cast<uint8>(symbol);

    if(s >= 0x41 && s <= 0x5a)
    {
        return static_cast<char>(s + 0x20);
    }
    else if(s >= 0xc0 && s < 0xE0)
    {
        return static_cast<char>(s + 0x20);
    }

    return symbol;
}


bool SU::StringToDouble(double *value, pchar str)
{
    const int SIZE_BUFFER = 100;
    char buffer[SIZE_BUFFER];

    strcpy_s(buffer, SIZE_BUFFER - 1, str);

    char *p = buffer;

    while (*p)
    {
        if (*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    char *end = nullptr;

    *value = std::strtod(buffer, &end);

    return (end != buffer);
}


bool SU::String2Int(pchar buffer, int *value, char **end)
{
    BufferHeap string(static_cast<int>(std::strlen(buffer)) + 1);

    std::strcpy(string.DataChar(), buffer);

    *value = std::strtol(string.DataChar(), end, 10);

    if (*end == string.DataChar())
    {
        *end = const_cast<char *>(buffer); //-V2567
    }
    else
    {
        *end = const_cast<char *>(buffer) + (*end - string.DataChar()); //-V2567
    }

    return (*end != const_cast<char *>(buffer)); //-V2567
}


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, pchar src)
{
    if (std::strlen(src) + 1 < dest_size)
    {
        std::strcpy(dest, src);
        return 0;
    }

    return 1;
}

#endif



static void Swap(char *ch1, char *ch2)
{
    char c = *ch1;
    *ch1 = *ch2;
    *ch2 = c;
}


pchar SU::Int2String(int n, char out[32])
{
    char *result = out;

    if (n < 0)
    {
        out[0] = '-';
        n = -n;
        result = &out[1];
    }

    int i = 0;

    do
    {
        result[i++] = (char)(n % 10 + '0');
        n -= n % 10;
        n /= 10;
    } while (n > 0);

    result[i] = '\0';

    for (int j = 0; j < i; j++)
    {
        i--;
        Swap(&result[j], &result[i]);
    }

    return out;
}


void SU::LeaveFewDigits(char *buffer, int size, int digits)
{
    char *pointer = buffer;

    while (*pointer != '\0' && digits != 0 && size > 0)
    {
        size--;

        if ((*pointer & 0xF0) == 0x30)
        {
            digits--;
        }

        pointer++;
    }

    if (size > 0)
    {
        *pointer = '\0';
    }
}


void SU::LeaveFewSignedDigits(char *buffer, int size, int digits)
{
    bool isControl = false;

    char *pointer = buffer;

    while (*pointer != '\0' && digits != 0 && size > 0)
    {
        size--;

        if ((*pointer & 0xF0) == 0x30)
        {
            if (*pointer == 0x30 && !isControl)
            {
                pointer++;
                continue;
            }

            isControl = true;

            digits--;
        }

        pointer++;
    }

    if (*pointer == '.' || *pointer == ',')
    {
        pointer++;
    }

    if (size > 0)
    {
        *pointer = '\0';
    }
}


void SU::ReplaceSymbol(char *string, char oldSymbol, char newSymbol)
{
    while (*string != '\0')
    {
        if (*string == oldSymbol)
        {
            *string = newSymbol;
        }

        string++;
    }
}


bool SU::ExistSymbol(char *string, char symbol)
{
    while (*string != '\0')
    {
        if (*string == symbol)
        {
            return true;
        }

        string++;
    }

    return false;
}


bool SU::IsDigit(char symbol)
{
    return symbol >= '0' && symbol <= '9';
}


pchar SU::TimeMStoText(uint timeMS, char buffer[128], bool always_show_ms)
{
    static const uint MS_IN_DAY = 24 * 60 * 60 * 1000;
    static const uint MS_IN_HOUR = 60 * 60 * 1000;
    static const uint MS_IN_MIN = 60 * 1000;
    static const uint MS_IN_SEC = 1000;

    uint value = timeMS;

    uint days = value / MS_IN_DAY;
    value -= days * MS_IN_DAY;

    uint hours = value / MS_IN_HOUR;
    value -= hours * MS_IN_HOUR;

    uint minutes = value / MS_IN_MIN;
    value -= minutes * MS_IN_MIN;

    uint secs = value / MS_IN_SEC;

    uint ms = timeMS % MS_IN_SEC;

    buffer[0] = '\0';

    if (timeMS < 1000)
    {
        std::sprintf(buffer, "%03u��", timeMS);
    }
    else
    {
        if (days)
        {
            std::sprintf(buffer, "%u�", days);
        }
        if (hours)
        {
            std::sprintf(buffer + std::strlen(buffer), "%02u�", hours); //-V512
        }
        if (minutes)
        {
            std::sprintf(buffer + std::strlen(buffer), "%02u�", minutes); //-V512
        }
        if (always_show_ms)
        {
            std::sprintf(buffer + std::strlen(buffer), "%02u.%03u�", secs, ms); //-V512
        }
        else
        {
            std::sprintf(buffer + std::strlen(buffer), "%02u�", secs); //-V512
        }
    }

    return buffer;
}


char *SU::Int2String(int value, bool alwaysSign, int numMinFields, char buffer[20])
{
    const int SIZE = 20;
    char format[SIZE] = "%";
    snprintf(&(format[1]), SIZE - 1, "0%d", numMinFields);
    std::strcat(format, "d");
    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        std::snprintf(buffer + 1, SIZE - 1, format, value);
    }
    else
    {
        snprintf(buffer, SIZE, format, value);
    }
    return buffer;
}


char *SU::MilliUnitsToUnits(int ms, char out[32])
{
    out[0] = '\0';

    float num_sec = (float)ms / 1000.0f;

    std::sprintf(out, "%f", (double)num_sec);

    char *pointer = out + std::strlen(out) - 1;

    while (*pointer == '0')
    {
        *pointer = '\0';
        pointer--;
    }

    return out;
}


int SU::FindPosition(pchar text, char symbol)
{
    uint size = std::strlen(text);

    for (uint i = 0; i < size; i++)
    {
        if (text[i] == symbol)
        {
            return (int)i;
        }
    }

    return -1;
}


char *SU::GetWord(pchar text, int start_pos, int end_pos, char *out)
{
    out[0] = '\0';

    for (int i = start_pos; i < end_pos; i++)
    {
        char buf[2] = { text[i], '\0' };
        std::strcat(out, buf);
    }

    return out;
}
