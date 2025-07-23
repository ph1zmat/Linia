// 2023/09/08 21:07:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
//#include "Utils/String.h"


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


namespace SU
{
    char *GetWord(pchar, int start_pos, int end_pos, char *out);

    int FindPosition(pchar, char);

    // ������� �� ����������� � ���������� � ������� � ������
    char *MilliUnitsToUnits(int, char[32]);

    // ��� ������� ����� ����������� � �������� �������� �����.
    bool GetWord(pchar string, Word *word, const int numWord);

    bool WordEqualZeroString(Word *word, pchar string);

    // ���������� ��� ������. ����� �������� ������ �� str1
    bool EqualsStrings(pchar str1, pchar str2);

    bool EqualsStrings(void *str1, void *str2, int size);

    bool EqualsStrings(uint8 *str1, pchar  const str2, int size);

    bool EqualsZeroStrings(char *str1, char *str2);

    // ���������� ������� ������� �������, ������� �� �������� ������ ���� ������
    int FirstNotNumeral(char *buffer);

    // ����������� ������ � float. �������������� ������� �� ������� ����������� �������
    float StringToFloat(pchar string);

    char *ToUpper(char *str, int size);

    char *ToUpper(char *str);

    char ToUpper(char symbol);

    char ToLower(char symbol);

    bool IsDigit(char symbol);

    bool StringToDouble(double *value, pchar buffer);

    bool String2Int(pchar buffer, int *value, char **end);
 
    pchar Int2String(int n, char out[32]);

    // ��������� � ������ buffer �������� size ����� digits ����. ������ ���� ���� �����������
    void LeaveFewDigits(char *buffer, int size, int digits);

    // ��������� � ������ buffer �������� size ����� digits �������� ����. ��� ����� ������ ���������� �����
    void LeaveFewSignedDigits(char *buffer, int size, int digits);

    // �������� � ������ ��� ������� old �� new
    void ReplaceSymbol(char *string, char oldSymbol, char newSymbol);

    // ���������� true, ���� � ������ ���������� ������ symbol
    bool ExistSymbol(char *string, char symbol);
    
    inline uint Hash(uint hash, char byte)
    {
        return (uint8)byte + (hash << 6) + (hash << 16) - hash;
    }

    inline uint CalculateHash(const void *buffer, int size)
    {
        uint crc = 0;

        const uint8 *data = (const uint8 *)buffer;

        for (int i = 0; i < size; i++)
        {
            crc = Hash(crc, (char)*data++);
        }

        return crc;
    }

    pchar TimeMStoText(uint timeMS, char buffer[128], bool always_show_ms);

    /// @brief ����������� value � ��������� ������
    /// @attention ������ ����� ��������� �� ���������� ������ �������. ���� ��������� ����� ������� ���������� �������, �� ��� ����� ����������� ����
    char *Int2String(int value,        // ��������
        bool alwaysSign,               // ���� ���������� � true, ���� ����� ���������� ������
        int numMinFields,              // ����������� ����� ���������� ���������. ���� ��� ������ ����� ������� �� ���������, ������ ����������� ������
        char bufferOut[20]);           // ���� ������������ ������������ ��������
};


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, pchar src);

#endif
