// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "FontBig.h"
#include "FontMid.h"


struct TypeFont
{
    enum E
    {
        GOSTAU16BOLD,
        GOSTB28B,
        Count,
        None
    } value;
};


namespace Font
{
    // ������������� ������� �����. ����� ������������� ����� ������������ �������� Pop()
    void Set(const TypeFont::E);
    TypeFont::E Get();
    uint8 GetWidth(uint8 symbol);
    uint8 GetWidth(char symbol);
    uint8 GetHeight();
    uint8 GetHeight(uint8 symbol);
    bool RowNotEmpty(uint8 symbol, int row);
    bool BitIsExist(uint8 symbol, int row, int bit);
    // ������������� ���������� ������ �������� ����� ���������.
    void SetSpacing(int spacing);
    int GetSpacing();
    int GetLengthSymbols(pchar start, int num);
    int GetLengthText(pchar text);

    void SetSize(int size);
    int GetSize();
};
