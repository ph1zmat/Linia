// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Console
{
    void Init();

    void Draw();

    void AddString(pchar string);

    // ���������� true, ���� ������� ������ ������������ �� ������
    bool IsShown();

    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    void OnChanged_MaxStringsInConsole();
};

