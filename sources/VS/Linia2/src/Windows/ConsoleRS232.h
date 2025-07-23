﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once 
#include "Utils/String.h"


class ConsoleRS232 : public wxFrame
{
public:

    static void Create();
    virtual ~ConsoleRS232();

    static ConsoleRS232 *self;

    void SwitchVisibility();

    void AddLine(const wxString &line);

private:

    ConsoleRS232(wxFrame *parent);

    void OnSize(wxSizeEvent &);
    void OnEnterKey(wxCommandEvent &);
    void OnTextControlKeyDown(wxKeyEvent &);
    void OnClose(wxCloseEvent &);

    class History
    {
    public:
        void Add(const wxString &text);
        wxString Next();
        wxString Prev();

    private:
        std::vector<wxString> history;
        uint position = 1;
    };

    History history;

    std::mutex mutex;

    wxTextCtrl *text = nullptr;
    wxTextCtrl *line = nullptr;
};
