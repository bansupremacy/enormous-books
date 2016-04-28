#include "Display.h"
#include <Windows.h>
#include <iostream>
#include <conio.h>


void GoToXY(short x, short y)
{
	COORD Position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
}


void COUT(string STR, int Milisecs, int x, int y)
{
	if ((x != NULL) && (y != NULL))
	{
		GoToXY(x, y);							/*---Jump to (x,y)---*/
	}
	for (unsigned int i = 0; i < STR.length(); i++)		/*---Print out STR slowly---*/
	{
		cout << STR[i];
		Sleep(Milisecs);
	}
}


void CreatingMenu(string Options[], int _Options, int Chosen, int x, int y)
{
	for (int i = 0; i < _Options; i++)
	{
		GoToXY(x, y + i);
		SetColor(7);
		cout << i + 1 << ". ";
		if (i == Chosen)
		{
			SetColor(112);
		}
		else
		{
			SetColor(7);
		}
		cout << Options[i];
	}
	SetColor(7);
}


void SetColor(int ColorCode)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorCode);
}


void ClearScreen()
{
	ClearScreen(0, 0);		/*---Clear FROM (0,0)---*/
}


void ClearScreen(short x, short y)
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { x, y };

	csbi.dwCursorPosition.X = x;
	csbi.dwCursorPosition.Y = y;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}


void Pause()
{
	while (true)
	{
		int t = _getch();
		/*---ESCAPE--------ENTER--------SPACE--*/
		if ((t == 27) || (t == 13) || (t == 32))
		{
			break;
		}
	}
}