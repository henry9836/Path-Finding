#pragma once
#include <windows.h> 
#include <iostream>

using namespace std;

void Console_gotoXY(int x, int y);

void Console_Resize(HWND m_console, RECT m_rect, int x, int y);

void Console_Clear();

void Console_Color(string m_string, string m_color);
