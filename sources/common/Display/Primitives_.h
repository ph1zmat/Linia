// 2023/09/08 21:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors_.h"


namespace Primitives
{
    // ������������ ��� ��������������� �������� ���� �����, ����
    class Label
    {
    public:
        Label(int _x, int _y, pchar _text, const Color &_color_text, const Color &_color_back, const Color &_color_border) :
            x(_x), y(_y), text(_text), color_text(_color_text), color_back(_color_back), color_border(_color_border) { }
        void Draw();
        void SetState(pchar text, const Color &color_text, const Color &color_back);
        void SetColors(const Color &text, const Color &back, const Color &border);
    private:
        int x;
        int y;
        pchar text;
        Color color_text;
        Color color_back;
        Color color_border;
    };

    class Rect
    {
    public:
        Rect(int _width, int _height) : width(_width), height(_height) {}
        void Draw(int x, int y);
        void Draw(int x, int y, const Color &);
        void DrawRounded(int x, int y, int round, const Color &);
        void Fill(int x, int y);
        void Fill(int x, int y, Color color);
        void FillRounded(int x, int y, int round, const Color &colorFill, const Color &colorBound);
    private:
        int width;
        int height;
    };


    class Point
    {
    public:
        Point() {}
        void Draw(int x, int y, const Color &);
        void Draw(int x, int y);

        static void MoveTo(int x, int y);
        static void MoveOn(int dx, int dy);

    private:
        static int m_x;
        static int m_y;
    };


    class HLine
    {
    public:
        HLine(int _length) : length(_length) {}
        int Draw(int x, int y, const Color &);
        int Draw(const Coord &, const Color &);
        int Draw(const Coord &);
        int Draw(int x, int y);
    private:
        const int length;
    };


    class VLine
    {
    public:
        VLine(int l) : length(l) {}
        int Draw(int x, int y, const Color &);
        int Draw(int x, int y);
        int Draw(const Coord &);
    private:
        const int length;
    };


    // ������������ ����� - ��� ����� 45 ��������
    class DLine
    {
    public:
        DLine(int _length, int _dx, int _dy) : length(_length), dx(_dx), dy(_dy) {}
        void Draw(int x, int y);
    private:
        const int length;
        const int dx;
        const int dy;
    };

    class Line
    {
    public:
        Coord Draw(int x1, int y1, int x2, int y2);
        Coord Draw(const Coord &, int x2, int y2);
    };

    class ArrowRight
    {
    public:

        static const int WIDTH = 15;

        void Draw(int x, int y);
    };

    class ArrowLeft
    {
    public:

        static const int WIDTH = 15;

        void Draw(int x, int y);
    };

    class ArrowUp
    {
    public:

        static const int HEIGHT = 15;

        void Draw(int x, int y);
    };

    class ArrowDown
    {
    public:

        static const int HEIGHT = 15;

        void Draw(int x, int y);
    };

    class Axis
    {
    public:

        void Draw(const Coord &zero, int up, int right, int down);
    };

    class HMeasuringLines
    {
//      x0                    x1  y0
//       |/       tex        \|   
//       +--------------------+   y1
//       |\                  /|
//       |                    |
//       |                    |   y2

    public:

        HMeasuringLines(int _x0, int _x1, int _y0, int _y1, int _y2) :
            m_x0(_x0), m_x1(_x1), m_y0(_y0), m_y1(_y1), m_y2(_y2) { }

        void Draw();

    private:
        const int m_x0;
        const int m_x1;
        const int m_y0;
        const int m_y1;
        const int m_y2;
    };

    class VMeasuringLines
    {
//     x0          x1          x2
//      -----------+-----------  y0
//                /|\
//                 |
//                 |
//                 |
//                \|/
//      -----------+----------- y1
    public:

        VMeasuringLines(int x0, int x1, int x2, int y0, int y1) :
            m_x0(x0), m_x1(x1), m_x2(x2), m_y0(y0), m_y1(y1) { }

        void Draw();

    private:
        const int m_x0;
        const int m_x1;
        const int m_x2;
        const int m_y0;
        const int m_y1;
    };

    class ExHMeasuringLines
    {
//  x0      x1            x2        x3
//          |             |           y0
//         \|             |/  text
//  --------+-------------+---------  y1
//         /|             |\
//          |             |
//          |             |           y2

    public:

        ExHMeasuringLines(int x0, int x1, int x2, int x3, int y0, int y1, int y2, int _d_text, pchar _text) :
            m_x0(x0), m_x1(x1), m_x2(x2), m_x3(x3), m_y0(y0), m_y1(y1), m_y2(y2), d_text(_d_text), text(_text) { }

        void Draw();

    private:

        const int m_x0;
        const int m_x1;
        const int m_x2;
        const int m_x3;
        const int m_y0;
        const int m_y1;
        const int m_y2;
        const int d_text;
        const pchar text;
    };
}
