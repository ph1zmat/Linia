// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Color
{
    static Color FILL;              // ���� ����� ��������
    static Color BACK;              // ���� ����
    static Color MENU_UNSELECT;     // ��� ������������� �����
    static Color MENU_SELECT;       // ��� ����������� �����
    static Color RED;               // ������� ��������� ��� ���������
    static Color GREEN;             // ������ ��������� ��� ���������
    static Color BLUE;              // ����� ��������� ��� ���������
    static Color WHITE;             // �����
    static Color BLACK;             // ׸���� ���
    static Color GRAY;
    static Color YELLOW;

    static const int COUNT = 255;

    explicit Color(uint8 i) : index(i) {}

    void SetAsCurrent() const;
    static Color GetCurrent();

    uint8 Index() const { return index;  }

    void SetValue(uint value) const { Value() = value; }
    void SetValue(uint8 red, uint8 green, uint8 blue) const { Value() = Make(red, green, blue); }

    // ������ �� ��������
    uint &Value() const;

    static uint Make(uint8 r, uint8 g, uint8 b) { return ((uint)((b) | ((g) << 8) | ((r) << 16))); }

    uint8 GetRED() const;
    uint8 GetGREEN() const;
    uint8 GetBLUE() const;

private:

    uint8 index;
};


struct ColorScheme
{
    static const int COUNT = 3;
    uint colors[Color::COUNT];

    static ColorScheme &Current();
};


namespace ColorTimer
{
    void Reset();
    bool IsMain();
}


#ifdef GUI

    #define MAKE_COLOR(r, g, b)     ((uint)((b) | ((g) << 8) | ((r) << 16)))

    #define RED_FROM_COLOR(color)   ((uint8)(((color) >> 16) & 0xFF))
    #define GREEN_FROM_COLOR(color) ((uint8)(((color) >> 8) & 0xFF))
    #define BLUE_FROM_COLOR(color)  ((uint8)((color) & 0xFF))
#else

    #define RED_FROM_COLOR(color)   ((uint8)(((color) >> 16) & 0xFF))
    #define GREEN_FROM_COLOR(color) ((uint8)(((color) >> 8) & 0xFF))
    #define BLUE_FROM_COLOR(color)  ((uint8)((color) & 0xFF))

#endif
