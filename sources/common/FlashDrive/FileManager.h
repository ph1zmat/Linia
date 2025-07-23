// 2025/02/03 14:33:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 


namespace FileManager
{
    // ���������� ���� ��� ��� ����������� ����� ������
    void Init();

    // ���������� �������� ��������
    void Draw();

    void PressSB_LevelUp();

    void PressSB_LevelDown();

    void RotateRegSet(int angle);

    bool GetNameForNewFile(char name[255]);

    void PressSB_Tab();
};
