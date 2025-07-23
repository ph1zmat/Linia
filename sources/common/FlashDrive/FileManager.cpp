// 2025/02/03 14:33:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FileManager.h"
#include "Display/Font/Font.h"
#include "FlashDrive/FlashDrive.h"
#include "Display/Display_.h"
#include "Menu/Menu.h"
#include "Utils/Math_.h"
#include "Utils/StringUtils_.h"
#include "Display/Painter.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


namespace FileManager
{
#define FM_CURSOR_IN_DIRS       (bfm.cursorsInDirs)

    static struct BitFieldFileManager
    {
        uint  cursorsInDirs : 1;
        uint  notUsed : 31;
    } bfm = { 1, 0 };


#define RECS_ON_PAGE    23
#define WIDTH_COL       135

#define FM_REDRAW_FULL          1
#define FM_REDRAW_FOLDERS       2
#define FM_REDRAW_FILES         3

    int FM_NEED_REDRAW = FM_REDRAW_FULL;

    static char currentDir[255] = "\\";
    static int numFirstDir = 0;         // ����� ������� ����������� �������� � ����� ������. ����� ����� ���� �������� RECS_ON_PAGE ���������
    static int numCurDir = 0;           // ����� ������������� ��������
    static int numFirstFile = 0;        // ����� ������� ����������� ����� � ������ ������. ����� ����� ���� �������� RECS_ON_PAGE ������.
    static int numCurFile = 0;          // ����� ������������� �����
    static int numDirs = 0;
    static int numFiles = 0;

    static void DrawDirs(int x, int y);
    static void DrawFiles(int x, int y);
    static void DrawNameCurrentDir(int left, int top);
    static void IncCurrentDir();
    static void DecCurrentDir();
    static void IncCurrentFile();
    static void DecCurrentFile();

    static char FILE_NAME_MASK[64];

    static pchar FILE_NAME = "";
}


void FileManager::Init()
{
    std::strcpy(currentDir, "\\");
    numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
}


static void DrawLongString(int x, int y, char *string, bool hightlight)
{
    int length = Font::GetLengthText(string);

    Color color = Color::FILL;
    if (hightlight)
    {
        Painter::FillRegion(x - 1, y, WIDTH_COL + 9, 8, color);
        color = Color::BACK;
    }

    if (length <= WIDTH_COL)
    {
        Painter::DrawText(x, y, string, color);
    }
    else
    {
        Painter::DrawTextWithLimitationC(x, y, string, color, x, y, WIDTH_COL, 10);
        Painter::DrawText(x + WIDTH_COL + 3, y, "...");
    }
}


static void DrawHat(int x, int y, pchar string, int num1, int num2)
{
    Painter::FillRegion(x - 1, y, WIDTH_COL + 9, RECS_ON_PAGE * 9 + 11, Color::BACK);
    Painter::DrawFormText(x + 60, y, Color::FILL, string, num1, num2);
    Painter::DrawHLine(y + 10, x + 2, x + 140);
}


void FileManager::DrawDirs(int x, int y)
{
    FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
    DrawHat(x, y, "������� : %d/%d", numCurDir + ((numDirs == 0) ? 0 : 1), numDirs);
    char nameDir[255];
    StructForReadDir sfrd;
    y += 12;
    if (FDrive::GetNameDir(currentDir, numFirstDir, nameDir, &sfrd))
    {
        int  drawingDirs = 0;
        DrawLongString(x, y, nameDir, FM_CURSOR_IN_DIRS && ( numFirstDir + drawingDirs == numCurDir));
        while (drawingDirs < (RECS_ON_PAGE - 1) && FDrive::GetNextNameDir(nameDir, &sfrd))
        {
            drawingDirs++;
            DrawLongString(x, y + drawingDirs * 9, nameDir, FM_CURSOR_IN_DIRS && ( numFirstDir + drawingDirs == numCurDir));
        }
    }
}


void FileManager::DrawFiles(int x, int y)
{
    DrawHat(x, y, "���� : %d/%d", numCurFile + ((numFiles == 0) ? 0 : 1), numFiles);
    char nameFile[255];
    StructForReadDir sfrd;
    y += 12;
    if (FDrive::GetNameFile(currentDir, numFirstFile, nameFile, &sfrd))
    {
        int drawingFiles = 0;
        DrawLongString(x, y, nameFile, !FM_CURSOR_IN_DIRS && (numFirstFile + drawingFiles == numCurFile));
        while (drawingFiles < (RECS_ON_PAGE - 1) && FDrive::GetNextNameFile(nameFile, &sfrd))
        {
            drawingFiles++;
            DrawLongString(x, y + drawingFiles * 9, nameFile, !FM_CURSOR_IN_DIRS && (numFirstFile + drawingFiles == numCurFile));
        }
    }
}


void FileManager::DrawNameCurrentDir(int left, int top)
{
    Color::FILL.SetAsCurrent();

    int length = Font::GetLengthText(currentDir);
    if (length < 277)
    {
        Painter::DrawText(left + 1, top + 1, currentDir);
    }
    else
    {
        char *pointer = currentDir + 2;
        while (length > 277)
        {
            while (*pointer != '\\' && pointer < currentDir + 255)
            {
                pointer++;
            }
            if (pointer >= currentDir + 255)
            {
                return;
            }
            length = Font::GetLengthText(++pointer);
        }
        Painter::DrawText(left + 1, top + 1, pointer);
    }
}


void FileManager::Draw()
{
    if (!FM_NEED_REDRAW)
    {
        return;
    }

    int left = 1;
    int top = 1;
    int width = 297;
    int left2col = width / 2;

    if (FM_NEED_REDRAW == FM_REDRAW_FULL)
    {
        Display::BeginScene();
        Menu::Draw();
        Painter::DrawRectangle(0, 0, width, 239, Color::FILL);
//        Painter::FillRegion(left, top, Grid::Width() - 2, Grid::FullHeight() - 2, Color::BACK);
        FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
        DrawNameCurrentDir(left, top + 2);
//        Painter::DrawVLine(left2col, top + 16, 239, Color::FILL);
        Painter::DrawHLine(top + 15, 0, width);
    }

    if (FM_NEED_REDRAW != FM_REDRAW_FILES)
    {
        DrawDirs(left + 2, top + 18);
    }

    if (FM_NEED_REDRAW != FM_REDRAW_FOLDERS)
    {
        DrawFiles(left2col + 3, top + 18);
    }

    Display::EndScene();

    FM_NEED_REDRAW = 0;
}

void FileManager::PressSB_LevelDown()
{
    FM_NEED_REDRAW = FM_REDRAW_FULL;
    if (!FM_CURSOR_IN_DIRS)
    {
        return;
    }
    char nameDir[100];
    StructForReadDir sfrd;
    if (FDrive::GetNameDir(currentDir, numCurDir, nameDir, &sfrd))
    {
        if (std::strlen(currentDir) + std::strlen(nameDir) < 250)
        {
            FDrive::CloseCurrentDir(&sfrd);
            std::strcat(currentDir, "\\");
            std::strcat(currentDir, nameDir);
            numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
        }

    }
    FDrive::CloseCurrentDir(&sfrd);
}


void FileManager::PressSB_LevelUp()
{
    FM_NEED_REDRAW = FM_REDRAW_FULL;
    if (std::strlen(currentDir) == 1)
    {
        return;
    }
    char *pointer = currentDir + std::strlen(currentDir);
    while (*pointer != '\\')
    {
        pointer--;
    }
    *pointer = '\0';
    numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
    FM_CURSOR_IN_DIRS = 1;
}


void FileManager::IncCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir++;
        if (numCurDir > numDirs - 1)
        {
            numCurDir = 0;
            numFirstDir = 0;
        }
        if (numCurDir - numFirstDir > RECS_ON_PAGE - 1)
        {
            numFirstDir++;
        }
    }
}


void FileManager::DecCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir--;
        if (numCurDir < 0)
        {
            numCurDir = numDirs - 1;
            numFirstDir = numDirs - RECS_ON_PAGE;
            LIMITATION(numFirstDir, 0, numCurDir)
        }
        if (numCurDir < numFirstDir)
        {
            numFirstDir = numCurDir;
        }
    }
}


void FileManager::IncCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile++;
        if (numCurFile > numFiles - 1)
        {
            numCurFile = 0;
            numFirstFile = 0;
        }
        if (numCurFile - numFirstFile > RECS_ON_PAGE - 1)
        {
            numFirstFile++;
        }
    }
}


void FileManager::DecCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile--;
        if (numCurFile < 0)
        {
            numCurFile = numFiles - 1;
            numFirstFile = numFiles - RECS_ON_PAGE;
            LIMITATION(numFirstFile, 0, numCurFile)
        }
        if (numCurFile < numFirstFile)
        {
            numFirstFile = numCurFile;
        }
    }
}


void FileManager::RotateRegSet(int angle)
{
    if (FM_CURSOR_IN_DIRS)
    {
        angle > 0 ? DecCurrentDir() : IncCurrentDir();
        FM_NEED_REDRAW = FM_REDRAW_FOLDERS;
    }
    else
    {
        angle > 0 ? DecCurrentFile() : IncCurrentFile();
        FM_NEED_REDRAW = FM_REDRAW_FILES;
    }
}


bool FileManager::GetNameForNewFile(char name[255])
{
    char buffer[20];

    static int number = 0;

    std::strcpy(name, currentDir);
    std::strcat(name, "\\");

    int size = (int)std::strlen(FILE_NAME);
    if (size == 0)
    {
        return false;
    }

    PackedTime time = HAL_RTC::GetPackedTime();
    //  1          2           3         4           5             6
    uint values[] = { 0u, time.year, time.month, time.day, time.hours, time.minutes, time.seconds };

    char *ch = FILE_NAME_MASK;
    char *wr = name;

    while (*wr != '\0')
    {
        wr++;
    }

    while (*ch)
    {
        if (*ch >= 0x30)
        {
            *wr = *ch;
            wr++;
        }
        else
        {
            if (*ch == 0x07)
            {
                number++;
                std::strcpy(wr, SU::Int2String(number, false, *(ch + 1), buffer));
                wr += std::strlen(buffer);
                ch++;
            }
            else
            {
                if (*ch >= 0x01 && *ch <= 0x06)
                {
                    std::strcpy(wr, SU::Int2String((int)values[(uint)(*ch)], false, 2, buffer));
                    wr += std::strlen(buffer);
                }
            }
        }
        ch++;
    }

    *wr = '.';
    *(wr + 1) = '\0';

    std::strcat(name, "bmp");

    return true;
}


void FileManager::PressSB_Tab()
{
    FM_NEED_REDRAW = FM_REDRAW_FOLDERS;

    if (FM_CURSOR_IN_DIRS)
    {
        if (numFiles != 0)
        {
            FM_CURSOR_IN_DIRS = 0;
        }
    }
    else
    {
        if (numDirs != 0)
        {
            FM_CURSOR_IN_DIRS = 1;
        }
    }
}
