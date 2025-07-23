﻿// 2025/6/4 15:46:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PanelRegister;


class PainterRegister : public Painter
{
public:

    // Ширина бита
    static const int W_B = 20;

    PainterRegister(wxWindow *parent, PanelRegister *panel, const wxPoint &position);

    virtual void OnPaint(wxPaintEvent &) override;

    wxFont GetDefaultFont(int size = -1);

private:

    bool first_paint = true;

    PanelRegister *panel = nullptr;

    std::vector<wxCheckBox *> chbox;

    // Возвращает координаты верхнего левого угла отображения бита
    wxPoint CoordBit(int num_bit);

    void DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *);

    void DrawDescriptions(int index, wxGraphicsContext *);

    // Если нельзя вписать текст в данную ширину, то размер шрифта будет уменьшаться, пока не впишется
    void DrawTextInCenter(int x, int y, int width, const wxString &, int size, wxGraphicsContext *);

    void SetHintCheckBox(int num_bit);

    wxString GetDescription(int index_desc, int num_bit);
};
