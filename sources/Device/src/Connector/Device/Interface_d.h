// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Queue_.h"


namespace DInterface
{
    void Update();

    // ���������� ������ �� ������� ���������, ��������� ��������
    Queue &GetOutbox();
};
