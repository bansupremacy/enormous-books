#include "Question_Info_List.h"
#include "User_Info_List.h"
#include "Encryption.h"
#include "Display.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <atlstr.h>
using namespace std;


User_Info *LogIn(User_Info_List *&User_List);
void LogOut(User_Info_List *&User_List, Question_Info_List *&Question_List);
void Admin(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List);
void Teacher(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List);
void Student(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List);

void main()
{
	/*---Loading Data From File---*/
	User_Info_List *User_List = new User_Info_List;
	Question_Info_List *Question_List = new Question_Info_List;

	/*---Login Process---*/
	User_Info *Current_User = LogIn(User_List);

	if (Current_User == NULL)
	{
		/*---If User is banned---*/
		delete User_List;
		delete Question_List;
		return;
	}
	else
	{
		/*---User Type---*/
		switch (Current_User->Type)
		{
		case 1:
			Admin(Current_User, User_List, Question_List);
			break;
		case 2:
			Teacher(Current_User, User_List, Question_List);
			break;
		case 3:
			Student(Current_User, User_List, Question_List);
			break;
		}
	}
	//--------------------------------------------------
}

User_Info *LogIn(User_Info_List *&User_List)
{
	string _Username = "";
	string Database = User_List->CreateAuth();
	while (true)
	{
		ClearScreen();
		string Auth = "|";
		string Data = "";
		GoToXY(45, 13);
		cout << "Password : ";
		GoToXY(45, 12);
		cout << "Username : ";
		while (Data == "")
		{
			GoToXY(56, 12);
			/*---Reading Username---*/
			getline(cin, Data);
		}
		_Username = Data;
		Auth += Data + "|";
		Data = "";
		GoToXY(56, 13);
		bool Flag = true;
		/*---Password Printout Processing---*/
		while (Flag)
		{
			char Password = _getch();
			switch (Password)
			{
			case '\b':
				if (Data.length() > 0)
				{
					cout << "\b \b";
					Data.erase(Data.length() - 1, 1);
				}
				break;
			case '\r':
				if (Data == "")
				{
					break;
				}
				Flag = false;
				cout << endl;
				break;
			default:
				Data += Password;
				cout << "*";
				break;
			}
		}
		Auth += Encrypt(Data) + "|";
		/*---Authentication---*/
		if (Database.find(Auth) == -1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			COUT("Incorrect Username or Password ( )", 10, 40, 14);
			for (int i = 3; i > 0; i--)
			{
				GoToXY(72, 14);
				cout << i;
				GoToXY(74, 14);
				Sleep(1000);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else
		{
			break;
		}
	}
	User_Info *Current = *(User_List->GetHead());
	while (Current != NULL)
	{
		if (_Username == Current->Username)
		{
			if (!Current->Status)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				COUT("Your account has been disabled ( )", 10, 40, 14);
				for (int i = 3; i > 0; i--)
				{
					GoToXY(72, 14);
					cout << i;
					GoToXY(74, 14);
					Sleep(1000);
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				COUT("                                  ", 10, 40, 14);
				GoToXY(40, 14);
				return NULL;
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				COUT("You have successfully logged in ( )", 10, 40, 14);
				for (int i = 3; i > 0; i--)
				{
					GoToXY(73, 14);
					cout << i;
					GoToXY(75, 14);
					Sleep(1000);
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				return Current;
			}
		}
		Current = Current->Next;
	}
	return NULL;
}

void LogOut(User_Info_List *&User_List, Question_Info_List *&Question_List)
{
	ClearScreen();
	/*---Saving Data To File---*/
	delete User_List;
	delete Question_List;

	/*---UI---*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	COUT("You have successfully logged out ( )", 10, 40, 14);
	for (int i = 3; i > 0; i--)
	{
		GoToXY(74, 14);
		cout << i;
		GoToXY(76, 14);
		Sleep(1000);
	}
	COUT("                                    ", 10, 40, 14);
	GoToXY(40, 14);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Admin(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List)
{
	short x = 45;			//x-Coordinate of the menu list
	short y = 8;			//y-Coordinate of the menu list
	int _Options = 8;	//Quantity of options
	ClearScreen();
	bool Flag = true;
	string Options[8] = { "View all accounts" , "View accounts by TYPE" , "View accounts by STATUS" , "View accounts by SIMILAR USERNAMES" , "Export all accounts to XML file" , "Create account", "Modify YOUR ACCOUNT's Information" , "Logout" };
	while (Flag)
	{
		ClearScreen();
		GoToXY(93, 0);
		SetColor(47);
		cout << "You're logged in as a ADMIN";
		SetColor(7);
		int Chosen = 0;
		CreatingMenu(Options, _Options, Chosen, x, y);
		while (true)
		{
			int Action = _getch();
			if (Action == 224)
			{
				Action = _getch();
				switch (Action)
				{
				case 72:
					if (Chosen == 0)
					{
						Chosen = _Options - 1;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					else
					{
						Chosen--;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					break;
				case 80:
					Chosen++;
					Chosen %= _Options;
					CreatingMenu(Options, _Options, Chosen, x, y);
					break;
				}
			}
			else if (Action == 13)
			{
				break;
			}
		}
		//Finally, we have the chosen of enduser here
		ClearScreen();
		CString str = "Database.xml";
		CString action = "open";
		switch (Chosen)
		{
		case 7:		//Logout
			Flag = false;
			LogOut(User_List, Question_List);
			break;
		case 0:		//View ALL Accounts
			ViewAccount(User_List, Chosen + 1, Current_User);
			break;
		case 1:		//View Account by TYPE
			ViewAccount(User_List, Chosen + 1, Current_User);
			break;
		case 2:		//View Account by STATUS
			ViewAccount(User_List, Chosen + 1, Current_User);
			break;
		case 3:		//View Account by SIMILAR USERNAMES
			ViewAccount(User_List, Chosen + 1, Current_User);
			break;
		case 4:		//Export XML -> Open that XML file
			User_List->ExportXML("Database.xml");
			ShellExecute(NULL, action, str, NULL, NULL, SW_SHOW);
			break;
		case 5:		//Create new Account
			CreateUserUI(User_List);
			break;
		case 6:		//Modify YOUR ACCOUNT's Information
			ModAUserUI(Current_User);
			break;
		}
	}
}

void Teacher(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List)
{
	Question_List->Create_Custom_List(Current_User);
	short x = 45;			//x-Coordinate of the menu list
	short y = 8;			//y-Coordinate of the menu list
	int _Options = 8;	//Quantity of options
	ClearScreen();
	bool Flag = true;
	string Options[8] = { "Searching questions" , "View all your questions" , "Alter a question" , "Delete a question" , "Add a new question" , "Modify SUBJECT's Information" , "Modify ACCOUNT's Information" , "Logout" };
	while (Flag)
	{
		ClearScreen();
		GoToXY(91, 0);
		SetColor(47);
		cout << "You're logged in as a TEACHER";
		SetColor(7);
		int Chosen = 0;
		CreatingMenu(Options, _Options, Chosen, x, y);
		while (true)
		{
			int Action = _getch();
			if (Action == 224)
			{
				Action = _getch();
				switch (Action)
				{
				case 72:
					if (Chosen == 0)
					{
						Chosen = _Options - 1;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					else
					{
						Chosen--;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					break;
				case 80:
					Chosen++;
					Chosen %= _Options;
					CreatingMenu(Options, _Options, Chosen, x, y);
					break;
				}
			}
			else if (Action == 13)
			{
				break;
			}
		}
		//Finally, we have the chosen of enduser here
		ClearScreen();
		switch (Chosen)
		{
		case 7:		//Logout
			Flag = false;
			LogOut(User_List, Question_List);
			break;
		case 0:		//Searching
			Question_List->Search();
			GoToXY(0, 0);
			Pause();
			break;
		case 1:		//View Questions
			Question_List->ViewQuestions(Current_User);
			GoToXY(0, 0);
			Pause();
			break;
		case 2:		//Alter Question
			Question_List->ViewQuestions("ALL", "ALL", 0, 11);
			GoToXY(13, 0);
			SetColor(14);
			cout << "Alter a Question";
			SetColor(7);
			Question_List->Alter();
			Question_List->Create_Custom_List(Current_User);
			ClearScreen(0, 1);
			Question_List->ViewQuestions("ALL", "ALL", 0, 3);
			GoToXY(0, 0);
			Pause();
			break;
		case 3:		//Delete Question
			Question_List->ViewQuestions("ALL", "ALL", 0, 4);
			GoToXY(12, 0);
			SetColor(14);
			cout << "Delete a Question";
			SetColor(7);
			Question_List->Delete();
			ClearScreen(0, 1);
			Question_List->ViewQuestions("ALL", "ALL", 0, 3);
			GoToXY(0, 0);
			Pause();
			break;
		case 4:		//Add new Question
			GoToXY(12, 0);
			SetColor(14);
			cout << "Insert a Question";
			SetColor(7);
			Question_List->Insert();
			Question_List->Create_Custom_List(Current_User);
			ClearScreen(0, 1);
			Question_List->ViewQuestions("ALL", "ALL", 0, 3);
			GoToXY(0, 0);
			Pause();
			break;
		case 5:		//Modify SUBJECT's Information
			Question_List->ModSubject(Current_User);
			Question_List->Create_Custom_List(Current_User);
			break;
		case 6:		//Modify ACCOUNT's Information
			ModAUserUI(Current_User);
			break;
		}
	}
}

void Student(User_Info *&Current_User, User_Info_List *&User_List, Question_Info_List *&Question_List)
{
	Question_List->Create_Custom_List(Current_User);
	short x = 45;			//x-Coordinate of the menu list
	short y = 8;			//y-Coordinate of the menu list
	int _Options = 4;	//Quantity of options
	ClearScreen();
	bool Flag = true;
	string Options[4] = { "Searching questions" , "View all questions" , "Modify Account's Information" , "Logout" };
	while (Flag)
	{
		ClearScreen();
		GoToXY(91, 0);
		SetColor(47);
		cout << "You're logged in as a STUDENT";
		SetColor(7);
		int Chosen = 0;
		CreatingMenu(Options, _Options, Chosen, x, y);
		while (true)
		{
			int Action = _getch();
			if (Action == 224)
			{
				Action = _getch();
				switch (Action)
				{
				case 72:
					if (Chosen == 0)
					{
						Chosen = _Options - 1;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					else
					{
						Chosen--;
						CreatingMenu(Options, _Options, Chosen, x, y);
					}
					break;
				case 80:
					Chosen++;
					Chosen %= _Options;
					CreatingMenu(Options, _Options, Chosen, x, y);
					break;
				}
			}
			else if (Action == 13)
			{
				break;
			}
		}
		//Finally, we have the chosen of enduser here
		ClearScreen();
		switch (Chosen)
		{
		case 3:		//Logout
			Flag = false;
			LogOut(User_List, Question_List);
			break;
		case 0:		//Searching
			Question_List->Search();
			GoToXY(0, 0);
			Pause();
			break;
		case 1:		//View Questions
			Question_List->ViewQuestions(Current_User);
			break;
		case 2:		//Modify ACCOUNT's Information
			ModAUserUI(Current_User);
			break;
		}
	}
}