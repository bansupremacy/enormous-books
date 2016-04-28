#pragma once
#include <string>
using namespace std;

/*---Move the cursor to coordinate (x,y)---*/
void GoToXY(short x, short y);


/*---Print a string STR word by word slowly by Milisecs at the coordinate (x,y)---*/
void COUT(string STR, int Milisecs, int x, int y);


/*---Creating Main Menu List---*/
void CreatingMenu(string Options[], int _Options, int Chosen, int x, int y);


/*---Set the console color---*/
void SetColor(int ColorCode);


/*---Replacement of system("cls")---*/
void ClearScreen();
 

/*----Clear all the screen FROM (x,y)---*/
void ClearScreen(short x, short y);


/*---Replacement of system("pause")---*/
/*---Press Space, Escape or Enter to continue---*/
void Pause();