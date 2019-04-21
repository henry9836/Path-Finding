#include "ConsoleControls.h"

void Console_gotoXY(int x, int y) //Move Console Cursor to XY
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Console_Resize(HWND m_console, RECT m_rect, int x, int y) { //Resize Console Window

	MoveWindow(m_console, m_rect.left, m_rect.top, 550, 550, TRUE);

}

void Console_Clear() { //Clear console window
	system("CLS");
	Console_gotoXY(0,0);
}

void Console_Write(string m_string, string m_color) {
	WORD m_word = NULL;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_word);
}