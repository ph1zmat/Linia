// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Font/Font.h"


struct Color;


class Text
{
public:
    explicit Text(pchar format, ...);
    ~Text();
    int Write(int x, int y, const Color &);
    int Write(int x, int y);
    int Write(int x, int y, int width, const Color &);
    int Write(int x, int y, int width);
    int WriteScaled(int x, int y, int size);
    void WriteInCenterRect(int x, int y, int width, int height, const Color &);
    void WriteInCenterRect(int x, int y, int width, int height);
    pchar c_str() const;
    char *LastSymbol();
private:
    char *text = nullptr;

    void Create(pchar text);

    int WriteSymbol(int x, int y, uint8 symbol) const;
    int WriteSymbolScaled(int x, int y, uint8 symbol, int scale) const;

    // ���������� ���������� ���� � ������
    int NumWords() const;

    // ���������� ���������� � ����� numWord
    void GetWord(int numWord, char *(&start), int &num);

    // ���������� num ��������, ������� � ������ start
    int WriteSymbols(char *start, int num, int x, int y) const;

    int WriteSymbolsScaled(char *start, int num, int x, int y, int size) const;

    // ���������� num ��������, ������� � ������ �����, � ���� ������� width ��������
    void WriteSymbols(char *start, int num, int x, int y, int width, const Color &) const;

    void WriteSymbols(char *start, int num, int x, int y, int width) const;

    void Clear();
};
