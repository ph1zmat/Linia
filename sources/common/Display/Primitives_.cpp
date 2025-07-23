// 2023/09/08 21:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "Settings/Settings.h"
#include "Display/Primitives_.h"
#include "Display/Text_.h"


using namespace Primitives;

int Point::m_x = 0;
int Point::m_y = 0;


void Label::Draw()
{
    int width = 100;
    int height = 30;

    Rect(width, height).FillRounded(x, y, 2, color_back, color_border);

    color_text.SetAsCurrent();

    Text(text).WriteInCenterRect(x, y, width, height);
}


void Label::SetState(pchar _text, const Color &_color_text, const Color &_color_back)
{
    text = _text;

    if (_color_text.Value() != Color::COUNT)
    {
        color_text = _color_text;
    }

    if (_color_back.Value() != Color::COUNT)
    {
        color_back = _color_back;
    }
}


void Label::SetColors(const Color &_text, const Color &back, const Color &border)
{
    color_text = _text;
    color_back = back;
    color_border = border;
}


void Rect::FillRounded(int x, int y, int round, const Color &colorFill, const Color &colorBound)
{
    DrawRounded(x, y, round, colorBound);
    Rect(width - 3, height - 3).Fill(x + 2, y + 2, colorFill);
    HLine(width - 3).Draw(x + 2, y + 1);
    HLine(width - 3).Draw(x + 2, y + height - 1);
    VLine(height - 3).Draw(x + 1, y + 2);
    VLine(height - 3).Draw(x + width - 1, y + 2);
}


void Rect::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    Fill(x, y);
}


void Rect::DrawRounded(int x, int y, int round, const Color &color)
{
    color.SetAsCurrent();

    HLine(width - round * 2 + 1).Draw(x + round, y);
    HLine(width - round * 2 + 1).Draw(x + round, y + height);
    VLine(height - round * 2 + 1).Draw(x, y + round);
    VLine(height - round * 2 + 1).Draw(x + width, y + round);

    if (round == 2)
    {
        Point().Draw(x + 1, y + 1);
        Point().Draw(x + width - 1, y + 1);
        Point().Draw(x + 1, y + height - 1);
        Point().Draw(x + width - 1, y + height - 1);
    }
}


void Rect::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    Draw(x, y);
}

void Rect::Draw(int x, int y)
{
    HLine(width).Draw(x, y);
    HLine(width).Draw(x, y + height - 1);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width - 1, y);
}


void Rect::Fill(int x0, int y0)
{
    for (int y = y0; y < y0 + height; y++)
    {
        HLine(width).Draw(x0, y);
    }
}


void DLine::Draw(int x, int y)
{
    for (int i = 0; i < length; i++)
    {
        Point().Draw(x, y);
        x += dx;
        y += dy;
    }
}


void ArrowRight::Draw(int x, int y)
{
    int d = 3;

//    HLine(WIDTH).Draw(x, y);
    Line().Draw(x, y - d, x + WIDTH, y);
    Line().Draw(x, y + d, x + WIDTH, y);
}


void ArrowLeft::Draw(int x, int y)
{
    int d = 3;

//    HLine(WIDTH).Draw(x, y);
    Line().Draw(x, y, x + WIDTH, y + d);
    Line().Draw(x, y, x + WIDTH, y - d);
}


void ArrowUp::Draw(int x, int y)
{
    int d = 3;

//    VLine(HEIGHT).Draw(x, y);
    Line().Draw(x - d, y, x, y - HEIGHT);
    Line().Draw(x + d, y, x, y - HEIGHT);
}


void ArrowDown::Draw(int x, int y)
{
    int d = 3;

    Line().Draw(x - d, y, x, y + HEIGHT);
    Line().Draw(x + d, y, x, y + HEIGHT);
}


void HMeasuringLines::Draw()
{
    Coord delta(0, 0);
    int x0 = m_x0 + delta.x;
    int x1 = m_x1 + delta.x;
    int y0 = m_y0 + delta.y;
    int y1 = m_y1 + delta.y;
    int y2 = m_y2 + delta.y;

    VLine(y2 - y0).Draw(x0, y0);
    VLine(y2 - y0).Draw(x1, y0);
    HLine(x1 - x0).Draw(x0, y1);

    ArrowRight().Draw(x1 - ArrowRight::WIDTH, y1);
    ArrowLeft().Draw(x0, y1);
}


void VMeasuringLines::Draw()
{
    HLine(m_x2 - m_x0).Draw(m_x0, m_y0);
    HLine(m_x2 - m_x0).Draw(m_x0, m_y1);
    VLine(m_y1 - m_y0).Draw(m_x1, m_y0);

    ArrowUp().Draw(m_x1, m_y0 + ArrowUp::HEIGHT);
    ArrowDown().Draw(m_x1, m_y1 - ArrowDown::HEIGHT);
}


void ExHMeasuringLines::Draw()
{
    Coord delta(0, 0);
    int x0 = m_x0 + delta.x;
    int x1 = m_x1 + delta.x;
    int x2 = m_x2 + delta.x;
    int x3 = m_x3 + delta.x;
    int y0 = m_y0 + delta.y;
    int y1 = m_y1 + delta.y;
    int y2 = m_y2 + delta.y;

    VLine(y2 - y1).Draw(x1, y0);
    VLine(y2 - y1).Draw(x2, y0);
    HLine(x3 - x0).Draw(x0, y1);

    ArrowRight().Draw(x1 - ArrowRight::WIDTH, y1);
    ArrowLeft().Draw(x2, y1);

    Text(text).Write(x2 + d_text, y1 - Font::GetHeight());
}


void Axis::Draw(const Coord &zero, int up, int right, int down)
{
    VLine(down + up).Draw(zero.x, zero.y - up);
    HLine(right).Draw(zero.x, zero.y);
    ArrowUp().Draw(zero.x, zero.y - up + ArrowUp::HEIGHT);
    ArrowRight().Draw(zero.x + right - ArrowRight::WIDTH, zero.y);

    Text("U").Write(zero.x - 20, zero.y - up);
    Text("t").Write(zero.x + right - 5, zero.y - 20);
}


void Point::MoveTo(int x, int y)
{
    Line().Draw(x, y, m_x, m_y);

    m_x = x;
    m_y = y;
}


void Point::MoveOn(int dx, int dy)
{
    MoveTo(m_x + dx, m_y + dy);
}
