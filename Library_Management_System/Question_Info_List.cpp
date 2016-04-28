#include "Question_Info_List.h"
#include <fstream>
#include <atlstr.h>
#include <algorithm>
#include "Display.h"
#include <conio.h>
#include <iomanip>
#include <iostream>



Question_Info::Question_Info()
{
	/*---Default Values---*/
	ID = 1;
	Subject = "";
	Author = "";
	Viewable = true;
	isSingle = true;
	Question = "";
	Options = "|";
	Answer = "";
	Next = NULL;
	Prev = NULL;
}



Question_Info_List::Question_Info_List()
{
	/*---Constructor---*/
	/*---Initalize---*/
	Head = NULL;
	Custom_Question = NULL;

	/*---Loading Question List From File---*/
	ifstream inp;
	inp.open("Question_Info_List.txt");
	Question_Info *Current_Question = Head;
	int _ID = 0;
	while (!inp.eof())
	{
		if (Current_Question == NULL)		
		{
			/*---Then Head = Current_Question---*/
			_ID++;
			Current_Question = new Question_Info;
			Current_Question->ID = _ID;
			getline(inp, Current_Question->Subject);
			getline(inp, Current_Question->Author);
			inp >> Current_Question->Viewable;
			inp >> Current_Question->isSingle;
			inp.get();
			getline(inp, Current_Question->Question);
			getline(inp, Current_Question->Options);
			getline(inp, Current_Question->Answer);
			if (Current_Question->Subject != "")
			{
				Head = Current_Question;
			}
		}
		else
		{
			_ID++;
			Current_Question->Next = new Question_Info;
			Current_Question->Next->Prev = Current_Question;
			Current_Question = Current_Question->Next;
			Current_Question->ID = _ID;
			getline(inp, Current_Question->Subject);
			getline(inp, Current_Question->Author);
			inp >> Current_Question->Viewable;
			inp >> Current_Question->isSingle;
			inp.get();
			getline(inp, Current_Question->Question);
			getline(inp, Current_Question->Options);
			getline(inp, Current_Question->Answer);
		}
	}
	inp.close();
}



Question_Info_List::~Question_Info_List()
{
	/*---Destructor---*/
	/*---Saving Question List to File---*/
	ofstream outp;
	outp.open("Question_Info_List.txt");
	Question_Info *Current_Question = Head;
	bool Flag;		/*---To know that whether cout endl or not---*/
	if (Head != NULL)
	{
		Flag = false;
	}
	/*---Saving the Head List---*/
	while (Current_Question != NULL)
	{
		if (Flag == true)
		{
			outp << endl;
		}
		else
		{
			Flag = true;
		}
		outp << Current_Question->Subject << endl;
		outp << Current_Question->Author << endl;
		outp << Current_Question->Viewable << endl;
		outp << Current_Question->isSingle << endl;
		outp << Current_Question->Question << endl;
		outp << Current_Question->Options << endl;
		outp << Current_Question->Answer;
		Current_Question = Current_Question->Next;
	}
	Current_Question = Custom_Question;
	if (Head == NULL)
	{
		Flag = false;
	}
	else
	{
		Flag = true;
	}
	/*---Saving the Custom List---*/
	while (Current_Question != NULL)
	{
		if (Flag == true)
		{
			outp << endl;
		}
		else
		{
			Flag = true;
		}
		outp << Current_Question->Subject << endl;
		outp << Current_Question->Author << endl;
		outp << Current_Question->Viewable << endl;
		outp << Current_Question->isSingle << endl;
		outp << Current_Question->Question << endl;
		outp << Current_Question->Options << endl;
		outp << Current_Question->Answer;
		Current_Question = Current_Question->Next;
	}
	outp.close();

	/*---Deleting Head List---*/
	Question_Info *Current = Head;
	while (Current != NULL)
	{
		Question_Info *Temp = Current;
		Current = Current->Next;
		delete Temp;
	}
	/*---Deleting Custom List---*/
	Current = Custom_Question;
	while (Current != NULL)
	{
		Question_Info *Temp = Current;
		Current = Current->Next;
		delete Temp;
	}
}



void Question_Info_List::Create_Custom_List(User_Info *&Current_User)
{
	/*---Create a List of Question for the Current_User---*/
	if (Custom_Question != NULL)
	{
		/*---If Custom_Question != NULL, link it back to Head, then Create Custom list---Purpose: re-creating the Custom List*/
		if (Head != NULL)
		{
			Question_Info *Current = Head;
			while (Current->Next != NULL)
			{
				Current = Current->Next;
			}
			Current->Next = Custom_Question;
			Custom_Question->Prev = Current;
			Custom_Question = NULL;
		}
		else
		{
			Head = Custom_Question;
			Custom_Question = NULL;
		}
	}
	if (Head == NULL)
	{
		/*---No Question in Database---*/
		return;
	}
	int _ID = 0;
	Question_Info *Current_Question = Head;
	while (Current_Question != NULL)
	{
		_ID++;
		Current_Question->ID = _ID;
		Current_Question = Current_Question->Next;
	}
	Head->Prev = new Question_Info;
	Head->Prev->Next = Head;
	Head = Head->Prev;
	Current_Question = Head->Next;
	Custom_Question = new Question_Info;
	Question_Info *Create_New = Custom_Question;
	_ID = 0;
	/*---Create List for specific User---*/
	switch (Current_User->Type)
	{
	case 2:										/*---For Teacher---Subject---*/
		_ID = 0;
		while (Current_Question != NULL)
		{
			if (Current_User->Subject.find("|" + Current_Question->Subject + "|") != -1)
			{
				_ID++;
				Create_New->Next = Current_Question;
				if (Current_Question->Prev != NULL)
				{
					Current_Question->Prev->Next = Current_Question->Next;
				}
				if (Current_Question->Next != NULL)
				{
					Current_Question->Next->Prev = Current_Question->Prev;
				}
				Current_Question = Current_Question->Next;
				Create_New->Next->Prev = Create_New;
				Create_New = Create_New->Next;
				Create_New->ID = _ID;
				Create_New->Next = NULL;
			}
			else
			{
				Current_Question = Current_Question->Next;
			}
		}
		if (Custom_Question->Next == NULL)
		{
			delete Custom_Question;
			Custom_Question = NULL;
		}
		else
		{
			Custom_Question = Custom_Question->Next;
			delete Custom_Question->Prev;
			Custom_Question->Prev = NULL;
		}
		if (Head->Next == NULL)
		{
			delete Head;
			Head = NULL;
		}
		else
		{
			Head = Head->Next;
			delete Head->Prev;
			Head->Prev = NULL;
		}
		break;
	case 3:										/*---For Student---Viewable or NOT---*/
		_ID = 0;
		while (Current_Question != NULL)
		{
			if (Current_Question->Viewable)
			{
				_ID++;
				Create_New->Next = Current_Question;
				if (Current_Question->Prev != NULL)
				{
					Current_Question->Prev->Next = Current_Question->Next;
				}
				if (Current_Question->Next != NULL)
				{
					Current_Question->Next->Prev = Current_Question->Prev;
				}
				Current_Question = Current_Question->Next;
				Create_New->Next->Prev = Create_New;
				Create_New = Create_New->Next;
				Create_New->ID = _ID;
				Create_New->Next = NULL;
			}
			else
			{
				Current_Question = Current_Question->Next;
			}
		}
		if (Custom_Question->Next == NULL)
		{
			delete Custom_Question;
			Custom_Question = NULL;
		}
		else
		{
			Custom_Question = Custom_Question->Next;
			delete Custom_Question->Prev;
			Custom_Question->Prev = NULL;
		}
		if (Head->Next == NULL)
		{
			delete Head;
			Head = NULL;
		}
		else
		{
			Head = Head->Next;
			delete Head->Prev;
			Head->Prev = NULL;
		}
		break;
	}
	if (Head == NULL)
	{
		return;
	}
	_ID = 0;
	Current_Question = Head;
	while (Current_Question->Next != NULL)
	{
		_ID++;
		Current_Question->ID = _ID;
		Current_Question = Current_Question->Next;
	}
}



void Question_Info_List::ViewQuestions(User_Info *&Current_User)
{
	/*---Coordinate---*/
	short x = 5;
	short y = 0;

	/*---Variables---*/
	Question_Info *Current;
	string _Subject = "";
	string _Author = "";
	string Mem[1000];
	string Temp;
	int maxChoose;
	int Chosen;

	/*---Starting---*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	GoToXY(x, y);
	cout << "     FILTER";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (Current_User->Type == 3)
	{
		/*---ONLY FOR STUDENT USER TYPE---*/
		GoToXY(x, y + 3);
		cout << "  Show Answer : ";
		GoToXY(x + 16, y + 3);
		cout << setfill(' ') << setw(10) << "ALL";
		SetColor(8);
		cout << "   <Press ESC to return>";
		SetColor(7);
	}
	GoToXY(x, y + 2);
	cout << "       Author : ";
	GoToXY(x, y + 1);
	cout << "      Subject : ";

	/*---Subject---*/
	/*---Retrieving Subject Array---*/
	Mem[0] = "ALL";
	maxChoose = 0;
	Temp = " ";
	Current = Custom_Question;
	while (Current != NULL)
	{
		if (Temp.find(" " + Current->Subject + " ") == -1)
		{
			maxChoose++;
			Mem[maxChoose] = Current->Subject;
			Temp += Current->Subject;
			Temp += " ";
		}
		Current = Current->Next;
	}
	GoToXY(x + 16, y + 2);
	cout << setfill(' ') << setw(10) << "ALL";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 1);
	cout << setfill(' ') << setw(10) << "ALL";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Chosen = 0;
	/*---Chosing Subjects---*/
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = maxChoose;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= maxChoose + 1;
				break;
			}
			GoToXY(x + 16, y + 1);
			printf("%c[K", 27);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			cout << setfill(' ') << setw(10) << Mem[Chosen];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			_Subject = Mem[Chosen];
			GoToXY(x + 16, y + 1);
			cout << setfill(' ') << setw(10) << _Subject;
			break;
		}
	}

	/*---Author---*/
	/*---Retrieving Author Array---*/
	Mem[0] = "ALL";
	maxChoose = 0;
	Temp = " ";
	Current = Custom_Question;
	while (Current != NULL)
	{
		if (Temp.find(" " + Current->Author + " ") == -1)
		{
			maxChoose++;
			Mem[maxChoose] = Current->Author;
			Temp += Current->Author;
			Temp += " ";
		}
		Current = Current->Next;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 2);
	cout << setfill(' ') << setw(10) << "ALL";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Chosen = 0;
	/*---Chosing Authors---*/
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = maxChoose;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= maxChoose + 1;
				break;
			}
			GoToXY(x + 16, y + 2);
			printf("%c[K", 27);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			cout << setfill(' ') << setw(10) << Mem[Chosen];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			_Author = Mem[Chosen];
			GoToXY(x + 16, y + 2);
			cout << setfill(' ') << setw(10) << _Author;
			break;
		}
	}
	if (Current_User->Type == 2)
	{
		/*---Teacher---*/
		ViewQuestions(_Subject, _Author, 0, 6);
	}
	else
	{
		ViewQuestions(_Subject, _Author, -1, 6);
	}
	GoToXY(0, 0);

	/*---Show Answer or NOT---*/
	/*---Only for Student---*/
	if (Current_User->Type == 3)
	{
		/*---Retrieving max ID---*/
		int maxID = 1;
		Question_Info *Current = Custom_Question;
		while (Current->Next != NULL)
		{
			Current = Current->Next;
		}
		maxID = Current->ID;

		/*---Starting Menu---*/
		GoToXY(x + 16, y + 3);
		SetColor(112);
		cout << setfill(' ') << setw(10) << "ALL";
		SetColor(7);

		/*---Chosing ID of Question to show Answer---*/
		Chosen = 0;
		bool Flag = true;
		while (Flag)
		{
			while (true)
			{
				int Action = _getch();
				if (Action == 27)				/*---Escape ViewQuestion---*/
				{
					Flag = false;
					break;
				}
				else if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 72:					/*---Up---*/
						if (Chosen == 0)
						{
							Chosen = maxID;
						}
						else
						{
							Chosen--;
						}
						break;
					case 80:					/*---Down---*/
						Chosen++;
						Chosen %= maxID + 1;
						break;
					}
					if (Chosen == 0)
					{
						GoToXY(x + 16, y + 3);
						SetColor(112);
						cout << setfill(' ') << setw(10) << "ALL";
						SetColor(7);
						cout << " \b";
					}
					else
					{
						GoToXY(x + 16, y + 3);
						cout << "     ";
						cout << "<";
						SetColor(112);
						cout << setfill('0') << setw(4) << Chosen;
						SetColor(7);
						cout << ">";
					}
				}
				else if (Action == 13)
				{
					if (Chosen == 0)
					{
						/*---Print ALL Answer---*/
						ViewQuestions(_Subject, _Author, 0, 6);
					}
					else
					{
						/*---Print a Specific Answer---*/
						ViewQuestions(_Subject, _Author, Chosen, 6);
					}
					GoToXY(0, 0);
				}
			}
		}
	}
}



void Question_Info_List::ViewQuestions(string _Subject, string _Author, int Show_Answer, int Line)
{
	Question_Info *Current = Custom_Question;
	
	ClearScreen(0, Line);
	int maxLen;
	string Temp;

	/*---Starting---*/
	GoToXY(0, Line);
	for (int i = 1; i <= 119; i++)
	{
		cout << "-";
	}
	cout << endl;
	Line++;
	cout << "|   ID   |";			
	GoToXY(118, Line);
	cout << "|" << endl;
	Line++;

	/*---Showing Question---*/
	while (Current != NULL)
	{
		/*---Condition---*/
		if (_Subject != "ALL")
		{
			if (Current->Subject != _Subject)
			{
				Current = Current->Next;
				continue;
			}
		}
		if (_Author != "ALL")
		{
			if (Current->Author != _Author)
			{
				Current = Current->Next;
				continue;
			}
		}
		
		/*---Starting---*/
		
		for (int i = 1; i <= 119; i++)
		{
			cout << "-";
		}
		cout << endl;
		Line++;

		/*---ID & Subject---*/
		cout << "|  " << setw(4) << setfill('0') << Current->ID << "  |  Subject : ";
		Temp = Current->Subject;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Author---*/
		cout << "|        |   Author : ";
		Temp = Current->Author;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Question---*/
		cout << "|        | Question : ";
		Temp = Current->Question;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Options---*/
		cout << "|        |  Options : ";
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;
		string Options = Current->Options;
		for (unsigned int i = 0; i < Options.length(); i++)
		{
			if (Options[i] == '|')
			{
				Temp = "";
				for (unsigned int j = i + 1; j < Options.length(); j++)
				{
					if (Options[j] == '|')
					{
						i = j - 1;
						break;
					}
					else
					{
						Temp += Options[j];
					}
				}
				if (Temp == "")
				{
					break;
				}
				else
				{
					cout << "|        |            ";
					maxLen = 96;
					for (unsigned int i = 0; i < Temp.length(); i++)
					{
						maxLen--;
						if (maxLen < 0)
						{
							maxLen = 96;
							cout << "|" << endl;
							Line++;
							cout << "|        |           ";
						}
						cout << Temp[i];
					}
					GoToXY(118, Line);
					cout << "|" << endl;
					Line++;
				}
			}
		}

		/*---Answer---*/
		cout << "|        |   Answer : ";
		if ((Show_Answer == 0) || (Current->ID == Show_Answer))
		{
			cout << "      \b\b\b\b\b\b";
			Temp = Current->Answer;
			for (unsigned int i = 1; i < Temp.length(); i++)
			{
				Temp.insert(i, ", ");
				i += 2;
			}
			maxLen = 96;
			for (unsigned int i = 0; i < Temp.length(); i++)
			{
				maxLen--;
				if (maxLen < 0)
				{
					maxLen = 96;
					cout << "|" << endl;
					Line++;
					cout << "|        |           ";
				}
				if (Temp[i] != ',')
				{
					SetColor(10);
				}
				cout << Temp[i];
				SetColor(7);
			}
			GoToXY(118, Line);
			cout << "|" << endl;
			Line++;
		}
		else
		{
			Temp = "<NULL>";
			maxLen = 96;
			for (unsigned int i = 0; i < Temp.length(); i++)
			{
				maxLen--;
				if (maxLen < 0)
				{
					maxLen = 96;
					cout << "|" << endl;
					Line++;
					cout << "|        |           ";
				}
				SetColor(12);
				cout << Temp[i];
				SetColor(7);
			}
			GoToXY(118, Line);
			cout << "|" << endl;
			Line++;
		}
		

		/*---Next Question---*/
		Current = Current->Next;
	}

	/*---Finishing---*/
	for (int i = 1; i <= 119; i++)
	{
		cout << "-";
	}
	cout << endl;
	Line++;
}



Question_Info **Question_Info_List::GetHead()
{
	return &Head;
}



void Question_Info_List::Alter()
{
	/*---Coordinate---*/
	short x = 5;
	short y = 1;

	int Chosen = 0;

	/*---Checking---*/
	if (Custom_Question == NULL)
	{
		GoToXY(x, y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "There Are NO Questions to Alter . . . ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return;
	}
	
	/*---Main menu---*/
	GoToXY(x, y);
	cout << "           ID : ";
	y++;
	GoToXY(x, y);
	cout << "      Subject : ";
	GoToXY(x, y + 1);
	cout << "       Author : ";
	GoToXY(x, y + 2);
	cout << "     Viewable ? Yes / No";
	GoToXY(x, y + 3);
	cout << "Single answer ? Yes / No";
	GoToXY(x, y + 4);
	cout << "     Question : ";
	y += 2;
	GoToXY(x, y + 5);
	cout << "      Options : ";					/*---Maximum 10 Options---*/
	GoToXY(x, y + 6);
	cout << "      Answers : ";
	y -= 2;

	/*---ID Processing---*/
	y--;
	int maxID;
	Question_Info *New_Question = Custom_Question;
	while (New_Question->Next != NULL)
	{
		New_Question = New_Question->Next;
	}
	maxID = New_Question->ID;
	maxID--;
	Chosen = 0;
	GoToXY(x + 16, y);
	cout << "<";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 17, y);
	cout << "0001";
	GoToXY(x + 21, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << ">";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = maxID;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= maxID + 1;
				break;
			}
			GoToXY(x + 17, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			cout << setfill('0') << setw(4) << Chosen + 1;
			GoToXY(x + 22, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			GoToXY(x + 17, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << setfill('0') << setw(4) << Chosen + 1;
			Chosen++;
			New_Question = Custom_Question;
			while (New_Question->Next != NULL)
			{
				if (New_Question->ID == Chosen)
				{
					break;
				}
				else
				{
					New_Question = New_Question->Next;
				}
			}
			break;
		}
	}

	ClearScreen(0, 11);

	/*---Reseting Data---*/
	New_Question->Subject = "";
	New_Question->Author = "";
	New_Question->Viewable = true;
	New_Question->isSingle = true;
	New_Question->Question = "";
	New_Question->Options = "|";
	New_Question->Answer = "";
	
	/*---Subject---*/
	y++;
	GoToXY(x + 16, y);
	getline(cin, New_Question->Subject);
	transform(New_Question->Subject.begin(), New_Question->Subject.end(), New_Question->Subject.begin(), toupper);

	/*---Author---*/
	GoToXY(x + 16, y + 1);
	getline(cin, New_Question->Author);
	transform(New_Question->Author.begin(), New_Question->Author.end(), New_Question->Author.begin(), toupper);

	/*---Viewable or NOT---*/
	Chosen = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 2);
	cout << "Yes";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	GoToXY(x + 22, y + 2);
	cout << "No";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 75:					/*---Left---*/
				if (Chosen == 0)
				{
					Chosen = 1;
				}
				else
				{
					Chosen--;
				}

				break;
			case 77:					/*---Right---*/
				Chosen++;
				Chosen %= 2;
				break;
			}
			switch (Chosen)
			{
			case 0:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 16, y + 2);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 22, y + 2);
				cout << "No";
				break;
			case 1:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 16, y + 2);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 22, y + 2);
				cout << "No";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
		}
		else if (Action == 13)
		{
			break;
		}
	}
	if (Chosen == 0)
	{
		New_Question->Viewable = true;
		GoToXY(x + 16, y + 2);
		cout << "Yes     ";
	}
	else
	{
		New_Question->Viewable = false;
		GoToXY(x + 16, y + 2);
		cout << "No      ";
	}

	/*---Single Answer or NOT---*/
	Chosen = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 3);
	cout << "Yes";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	GoToXY(x + 22, y + 3);
	cout << "No";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 75:					/*---Left---*/
				if (Chosen == 0)
				{
					Chosen = 1;
				}
				else
				{
					Chosen--;
				}
				break;
			case 77:					/*---Right---*/
				Chosen++;
				Chosen %= 2;
				break;
			}
			switch (Chosen)
			{
			case 0:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 16, y + 3);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 22, y + 3);
				cout << "No";
				break;
			case 1:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 16, y + 3);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 22, y + 3);
				cout << "No";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
		}
		else if (Action == 13)
		{
			break;
		}
	}
	if (Chosen == 0)
	{
		New_Question->isSingle = true;
		GoToXY(x + 16, y + 3);
		cout << "Yes     ";
	}
	else
	{
		New_Question->isSingle = false;
		GoToXY(x + 16, y + 3);
		cout << "No      ";
	}

	/*---Question---*/
	GoToXY(x + 16, y + 4);
	getline(cin, New_Question->Question);
	y += 2;

	/*---Selecting Quantity of Options---*/
	Chosen = 0;
	GoToXY(x + 16, y + 5);
	cout << "<Quantity :   >";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 28, y + 5);
	cout << "01";
	GoToXY(x + 31, y + 5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = 9;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= 10;
				break;
			}
			GoToXY(x + 28, y + 5);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			if (Chosen == 9)
			{
				cout << "10";
			}
			else
			{
				cout << "0" << Chosen + 1;
			}
			GoToXY(x + 31, y + 5);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			break;
		}
	}

	/*---Inputing the Options---*/
	GoToXY(x + 16, y + 5);
	cout << "               ";

	GoToXY(x, y + 6);
	cout << "                ";

	GoToXY(x, y + 7);
	cout << "      Answers : ";

	char Key = '@';
	for (int i = 0; i <= Chosen; i++)
	{
		Key++;
		GoToXY(x + 16, y + 6 + i);
		cout << Key << ". ";
		string _Option;
		getline(cin, _Option);
		string Temp = "";
		Temp += Key;
		Temp += ". ";
		Temp += _Option;
		Temp += "|";
		_Option = Temp;
		New_Question->Options += _Option;

		GoToXY(x, y + 7 + i);
		cout << "                ";

		GoToXY(x, y + 8 + i);
		cout << "      Answers : ";

		GoToXY(x + 16, y + 7 + i);
		if (New_Question->isSingle)
		{
			if (New_Question->Answer == "")
			{
				cout << "<Is this the Answer   Yes / No >";
				int _Chosen = 0;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 38, y + 7 + i);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 44, y + 7 + i);
				cout << "No";
				GoToXY(x + 48, y + 7 + i);
				while (true)
				{
					int Action = _getch();
					if (Action == 224)
					{
						Action = _getch();
						switch (Action)
						{
						case 75:					/*---Left---*/
							if (_Chosen == 0)
							{
								_Chosen = 1;
							}
							else
							{
								_Chosen--;
							}
							break;
						case 77:					/*---Right---*/
							_Chosen++;
							_Chosen %= 2;
							break;
						}
						switch (_Chosen)
						{
						case 0:
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
							GoToXY(x + 38, y + 7 + i);
							cout << "Yes";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							GoToXY(x + 44, y + 7 + i);
							cout << "No";
							GoToXY(x + 48, y + 7 + i);
							break;
						case 1:
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
							GoToXY(x + 44, y + 7 + i);
							cout << "No";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							GoToXY(x + 38, y + 7 + i);
							cout << "Yes";
							GoToXY(x + 48, y + 7 + i);
							break;
						}
					}
					else if (Action == 13)
					{
						if (_Chosen == 0)
						{
							Temp = "";
							Temp += Key;
							New_Question->Answer += Temp;
						}
						GoToXY(x + 16, y + 7 + i);
						cout << "                                ";
						break;
					}
				}
			}
		}
		else
		{
			cout << "<Is this the Answer   Yes / No >";
			int _Chosen = 0;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			GoToXY(x + 38, y + 7 + i);
			cout << "Yes";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			GoToXY(x + 44, y + 7 + i);
			cout << "No";
			GoToXY(x + 48, y + 7 + i);
			while (true)
			{
				int Action = _getch();
				if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 75:					/*---Left---*/
						if (_Chosen == 0)
						{
							_Chosen = 1;
						}
						else
						{
							_Chosen--;
						}
						break;
					case 77:					/*---Right---*/
						_Chosen++;
						_Chosen %= 2;
						break;
					}
					switch (_Chosen)
					{
					case 0:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
						GoToXY(x + 38, y + 7 + i);
						cout << "Yes";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						GoToXY(x + 44, y + 7 + i);
						cout << "No";
						GoToXY(x + 48, y + 7 + i);
						break;
					case 1:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
						GoToXY(x + 44, y + 7 + i);
						cout << "No";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						GoToXY(x + 38, y + 7 + i);
						cout << "Yes";
						GoToXY(x + 48, y + 7 + i);
						break;
					}
				}
				else if (Action == 13)
				{
					if (_Chosen == 0)
					{
						New_Question->Answer += Temp;
					}
					GoToXY(x + 16, y + 7 + i);
					cout << "                                ";
					break;
				}
			}
		}
		if (i == Chosen)
		{
			GoToXY(x + 16, y + 7 + i);
			cout << "<Done>";
		}
	}

	/*---Showing the Answer(s)---*/
	GoToXY(x, y + 8 + Chosen);
	cout << "      Answers : ";
	cout << New_Question->Answer[0];
	for (unsigned int i = 1; i < New_Question->Answer.length(); i++)
	{
		cout << ", " << New_Question->Answer[i];
	}
	GoToXY(x, y + 10 + Chosen);
}



void Question_Info_List::Insert()
{
	/*---Coordinate---*/
	short x = 5;
	short y = 1;

	int Chosen = 0;

	/*---Creating new Question---*/
	Question_Info *New_Question;
	
	if (Head == NULL)
	{
		Head = new Question_Info;
		New_Question = Head;
	}
	else
	{
		New_Question = Head;
		while (New_Question->Next != NULL)
		{
			New_Question = New_Question->Next;
		}
		New_Question->Next = new Question_Info;
		New_Question->Next->Prev = New_Question;
		New_Question = New_Question->Next;
		New_Question->ID = New_Question->Prev->ID + 1;
	}

	/*---Main menu---*/
	GoToXY(x, y);
	cout << "      Subject : ";
	GoToXY(x, y + 1);
	cout << "       Author : ";
	GoToXY(x, y + 2);
	cout << "     Viewable ? Yes / No";
	GoToXY(x, y + 3);
	cout << "Single answer ? Yes / No";
	GoToXY(x, y + 4);
	cout << "     Question : ";
	y += 2;
	GoToXY(x, y + 5);
	cout << "      Options : ";					/*---Maximum 10 Options*/
	GoToXY(x, y + 6);
	cout << "      Answers : ";
	y -= 2;

	/*---Subject---*/
	GoToXY(x + 16, y);
	getline(cin, New_Question->Subject);
	transform(New_Question->Subject.begin(), New_Question->Subject.end(), New_Question->Subject.begin(), toupper);
	
	/*---Author---*/
	GoToXY(x + 16, y + 1);
	getline(cin, New_Question->Author);
	transform(New_Question->Author.begin(), New_Question->Author.end(), New_Question->Author.begin(), toupper);
	
	/*---Viewable or NOT---*/
	Chosen = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 2);
	cout << "Yes";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	GoToXY(x + 22, y + 2);
	cout << "No";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 75:					/*---Left---*/
				if (Chosen == 0)
				{
					Chosen = 1;
				}
				else
				{
					Chosen--;
				}

				break;
			case 77:					/*---Right---*/
				Chosen++;
				Chosen %= 2;
				break;
			}
			switch (Chosen)
			{
			case 0:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 16, y + 2);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 22, y + 2);
				cout << "No";
				break;
			case 1:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 16, y + 2);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 22, y + 2);
				cout << "No";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
		}
		else if (Action == 13)
		{
			break;
		}
	}
	if (Chosen == 0)
	{
		New_Question->Viewable = true;
		GoToXY(x + 16, y + 2);
		cout << "Yes     ";
	}
	else
	{
		New_Question->Viewable = false;
		GoToXY(x + 16, y + 2);
		cout << "No      ";
	}

	/*---Single Answer or NOT---*/
	Chosen = 0;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 16, y + 3);
	cout << "Yes";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	GoToXY(x + 22, y + 3);
	cout << "No";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 75:					/*---Left---*/
				if (Chosen == 0)
				{
					Chosen = 1;
				}
				else
				{
					Chosen--;
				}
				break;
			case 77:					/*---Right---*/
				Chosen++;
				Chosen %= 2;
				break;
			}
			switch (Chosen)
			{
			case 0:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 16, y + 3);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 22, y + 3);
				cout << "No";
				break;
			case 1:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 16, y + 3);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 22, y + 3);
				cout << "No";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
		}
		else if (Action == 13)
		{
			break;
		}
	}
	if (Chosen == 0)
	{
		New_Question->isSingle = true;
		GoToXY(x + 16, y + 3);
		cout << "Yes     ";
	}
	else
	{
		New_Question->isSingle = false;
		GoToXY(x + 16, y + 3);
		cout << "No      ";
	}

	/*---Question---*/
	GoToXY(x + 16, y + 4);
	getline(cin, New_Question->Question);
	y += 2;

	/*---Selecting Quantity of Options---*/
	Chosen = 0;
	GoToXY(x + 16, y + 5);
	cout << "<Quantity :   >";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 28, y + 5);
	cout << "01";
	GoToXY(x + 31, y + 5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = 9;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= 10;
				break;
			}
			GoToXY(x + 28, y + 5);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			if (Chosen == 9)
			{
				cout << "10";
			}
			else
			{
				cout << "0" << Chosen + 1;
			}
			GoToXY(x + 31, y + 5);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			break;
		}
	}

	/*---Removing "Quantity <....>"---*/
	GoToXY(x + 16, y + 5);
	cout << "               ";

	GoToXY(x, y + 6);
	cout << "                ";

	GoToXY(x, y + 7);
	cout << "      Answers : ";

	/*---Inputing the Options---*/
	char Key = '@';
	for (int i = 0; i <= Chosen; i++)
	{
		Key++;
		GoToXY(x + 16, y + 6 + i);
		cout << Key << ". ";
		string _Option;
		/*---Cin the Option---*/
		getline(cin, _Option);
		string Temp = "";
		Temp += Key;
		Temp += ". ";
		Temp += _Option;
		Temp += "|";
		_Option = Temp;
		New_Question->Options += _Option;
		
		/*---Moving the string "Answer: "---*/
		GoToXY(x, y + 7 + i);
		cout << "                ";

		GoToXY(x, y + 8 + i);
		cout << "      Answers : ";

		/*---Choosing whether the Answer or not*/
		GoToXY(x + 16, y + 7 + i);
		if (New_Question->isSingle)
		{
			if (New_Question->Answer == "")
			{
				cout << "<Is this the Answer   Yes / No >";
				int _Chosen = 0;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				GoToXY(x + 38, y + 7 + i);
				cout << "Yes";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				GoToXY(x + 44, y + 7 + i);
				cout << "No";
				GoToXY(x + 48, y + 7 + i);
				/*---Moving the Chosen---UI---*/
				while (true)
				{
					int Action = _getch();
					if (Action == 224)
					{
						Action = _getch();
						switch (Action)
						{
						case 75:					/*---Left---*/
							if (_Chosen == 0)
							{
								_Chosen = 1;
							}
							else
							{
								_Chosen--;
							}
							break;
						case 77:					/*---Right---*/
							_Chosen++;
							_Chosen %= 2;
							break;
						}
						switch (_Chosen)
						{
						case 0:
							/*---Highlight the Chosen---YES---*/
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
							GoToXY(x + 38, y + 7 + i);
							cout << "Yes";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							GoToXY(x + 44, y + 7 + i);
							cout << "No";
							GoToXY(x + 48, y + 7 + i);
							break;
						case 1:
							/*---Highlight the Chosen---NO---*/
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
							GoToXY(x + 44, y + 7 + i);
							cout << "No";
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							GoToXY(x + 38, y + 7 + i);
							cout << "Yes";
							GoToXY(x + 48, y + 7 + i);
							break;
						}
					}
					else if (Action == 13)
					{
						if (_Chosen == 0)
						{
							Temp = "";
							Temp += Key;
							New_Question->Answer += Temp;
						}
						GoToXY(x + 16, y + 7 + i);
						cout << "                                ";
						break;
					}
				}
			}
		}
		else
		{
			cout << "<Is this the Answer   Yes / No >";
			int _Chosen = 0;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			GoToXY(x + 38, y + 7 + i);
			cout << "Yes";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			GoToXY(x + 44, y + 7 + i);
			cout << "No";
			GoToXY(x + 48, y + 7 + i);
			while (true)
			{
				int Action = _getch();
				if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 75:					/*---Left---*/
						if (_Chosen == 0)
						{
							_Chosen = 1;
						}
						else
						{
							_Chosen--;
						}
						break;
					case 77:					/*---Right---*/
						_Chosen++;
						_Chosen %= 2;
						break;
					}
					switch (_Chosen)
					{
					case 0:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
						GoToXY(x + 38, y + 7 + i);
						cout << "Yes";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						GoToXY(x + 44, y + 7 + i);
						cout << "No";
						GoToXY(x + 48, y + 7 + i);
						break;
					case 1:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
						GoToXY(x + 44, y + 7 + i);
						cout << "No";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						GoToXY(x + 38, y + 7 + i);
						cout << "Yes";
						GoToXY(x + 48, y + 7 + i);
						break;
					}
				}
				else if (Action == 13)
				{
					if (_Chosen == 0)
					{
						Temp = "";
						Temp += Key;
						New_Question->Answer += Temp;
					}
					GoToXY(x + 16, y + 7 + i);
					cout << "                                ";
					break;
				}
			}
		}
		if (i == Chosen)
		{
			GoToXY(x + 16, y + 7 + i);
			cout << "<Done>";
		}
	}

	/*---Showing the Answer(s)---*/
	GoToXY(x, y + 8 + Chosen);
	cout << "      Answers : ";
	cout << New_Question->Answer[0];
	for (unsigned int i = 1; i < New_Question->Answer.length(); i++)
	{
		cout << ", " << New_Question->Answer[i];
	}
	GoToXY(x, y + 10 + Chosen);
}



void Question_Info_List::Delete()
{
	/*---Coordinate---*/
	short x = 5;
	short y = 1;

	int Chosen = 0;

	/*---Checking---*/
	if (Custom_Question == NULL)
	{
		GoToXY(x, y);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "There Are NO Questions to Delete . . . ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		return;
	}

	/*---Starting---*/
	GoToXY(x, y);
	cout << "           ID : ";

	int maxID;
	Question_Info *Delete_Question = Custom_Question;
	while (Delete_Question->Next != NULL)
	{
		Delete_Question = Delete_Question->Next;
	}
	maxID = Delete_Question->ID;
	maxID--;
	Chosen = 0;
	GoToXY(x + 16, y);
	cout << "<";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	GoToXY(x + 17, y);
	cout << "0001";
	GoToXY(x + 21, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << ">";
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = maxID;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= maxID + 1;
				break;
			}
			GoToXY(x + 17, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			cout << setfill('0') << setw(4) << Chosen + 1;
			GoToXY(x + 22, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else if (Action == 13)
		{
			GoToXY(x + 17, y);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << setfill('0') << setw(4) << Chosen + 1;
			Chosen++;
			/*---Warning---*/
			GoToXY(x + 5, y + 1);
			cout << "Are you sure? ";
			cout << '<';
			SetColor(112);
			cout << "YES";
			SetColor(7);
			cout << "> <NO>";

			int Approve = 0;
			while (true)
			{
				int Action = _getch();
				if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 75:					/*---Left---*/
						if (Approve == 0)
						{
							Approve = 1;
						}
						else
						{
							Approve--;
						}
						break;
					case 77:					/*---Right---*/
						Approve++;
						Approve %= 2;
						break;
					}
					GoToXY(x + 5, y + 1);
					cout << "Are you sure? ";
					cout << '<';
					if (Approve == 0)
					{
						SetColor(112);
						cout << "YES";
						SetColor(7);
						cout << "> <NO>";
					}
					else
					{
						SetColor(7);
						cout << "YES> <";
						SetColor(112);
						cout << "NO";
						SetColor(7);
						cout << ">";
					}
				}
				else if (Action == 13)
				{
					break;
				}
			}
			if (Approve == 1)
			{
				return;
			}
			/*---Delete the Question---*/
			Delete_Question = Custom_Question;
			while (Delete_Question->Next != NULL)
			{
				if (Delete_Question->ID == Chosen)
				{
					if (Chosen == 1)
					{
						Custom_Question = Custom_Question->Next;
						Custom_Question->Prev = NULL;
						delete Delete_Question;
						int _ID = 0;
						Question_Info *Temp = Custom_Question;
						while (Temp != NULL)
						{
							_ID++;
							Temp->ID = _ID;
							Temp = Temp->Next;
						}
					}
					else
					{
						int _ID = Chosen - 1;
						Question_Info *Temp = Delete_Question->Next;
						while (Temp != NULL)
						{
							_ID++;
							Temp->ID = _ID;
							Temp = Temp->Next;
						}
						Delete_Question->Prev->Next = Delete_Question->Next;
						if (Delete_Question->Next != NULL)
						{
							Delete_Question->Next->Prev = Delete_Question->Prev;
						}
					}
					break;
				}
				else
				{
					Delete_Question = Delete_Question->Next;
				}
			}
			break;
		}
	}
}



void Question_Info_List::Search()
{
	/*---Coordinate---*/
	short x = 5;
	short y = 0;

	/*---Starting---*/
	GoToXY(x, y);
	SetColor(14);
	cout << "SEARCHING BY";
	SetColor(112);
	GoToXY(x + 3, y + 1);
	cout << "Keyword";
	SetColor(7);
	GoToXY(x + 3, y + 2);
	cout << "Subject";
	GoToXY(x + 3, y + 3);
	cout << " Author";

	int Chosen = 0;
	while (true)
	{
		int Action = _getch();
		if (Action == 224)
		{
			Action = _getch();
			switch (Action)
			{
			case 72:					/*---Up---*/
				if (Chosen == 0)
				{
					Chosen = 2;
				}
				else
				{
					Chosen--;
				}
				break;
			case 80:					/*---Down---*/
				Chosen++;
				Chosen %= 3;
				break;
			}
			switch (Chosen)
			{
			case 0:
				SetColor(112);
				GoToXY(x + 3, y + 1);
				cout << "Keyword";
				SetColor(7);
				GoToXY(x + 3, y + 2);
				cout << "Subject";
				GoToXY(x + 3, y + 3);
				cout << " Author";
				break;
			case 1:
				GoToXY(x + 3, y + 1);
				cout << "Keyword";
				SetColor(112);
				GoToXY(x + 3, y + 2);
				cout << "Subject";
				SetColor(7);
				GoToXY(x + 3, y + 3);
				cout << " Author";
				break;
			case 2:
				GoToXY(x + 3, y + 1);
				cout << "Keyword";
				GoToXY(x + 3, y + 2);
				cout << "Subject";
				SetColor(112);
				GoToXY(x + 3, y + 3);
				cout << " Author";
				SetColor(7);
				break;
			}
		}
		else if (Action == 13)
		{
			break;
		}
	}

	GoToXY(x + 3, y + 1);
	cout << "Keyword : ALL";
	GoToXY(x + 3, y + 2);
	cout << "Subject : ALL";
	GoToXY(x + 3, y + 3);
	cout << " Author : ALL";

	GoToXY(x + 13, y + 1 + Chosen);
	cout << "   \b\b\b";
	string Value = "";
	getline(cin, Value);
	if (Chosen != 0)
	{
		transform(Value.begin(), Value.end(), Value.begin(), toupper);
	}

	/*---Print out Question list at Line---*/
	int Line = 6;
	Question_Info *Current = Custom_Question;
	ClearScreen(0, Line);
	int maxLen;
	string Temp;

	/*---Starting---*/
	GoToXY(0, Line);
	for (int i = 1; i <= 119; i++)
	{
		cout << "-";
	}
	cout << endl;
	Line++;
	cout << "|   ID   |";
	GoToXY(118, Line);
	cout << "|" << endl;
	Line++;

	/*---Showing Question---*/
	while (Current != NULL)
	{
		/*---Condition---*/
		switch (Chosen)
		{
		case 0:
			if (Current->Question.find(Value) == -1)
			{
				Current = Current->Next;
				continue;
			}
			break;
		case 1:
			if (Current->Subject.find(Value) == -1)
			{
				Current = Current->Next;
				continue;
			}
			break;
		case 2:
			if (Current->Author.find(Value) == -1)
			{
				Current = Current->Next;
				continue;
			}
			break;
		}

		/*---Starting---*/

		for (int i = 1; i <= 119; i++)
		{
			cout << "-";
		}
		cout << endl;
		Line++;

		/*---ID & Subject---*/
		cout << "|  " << setw(4) << setfill('0') << Current->ID << "  |  Subject : ";
		Temp = Current->Subject;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Author---*/
		cout << "|        |   Author : ";
		Temp = Current->Author;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Question---*/
		cout << "|        | Question : ";
		Temp = Current->Question;
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Options---*/
		cout << "|        |  Options : ";
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;
		string Options = Current->Options;
		for (unsigned int i = 0; i < Options.length(); i++)
		{
			if (Options[i] == '|')
			{
				Temp = "";
				for (unsigned int j = i + 1; j < Options.length(); j++)
				{
					if (Options[j] == '|')
					{
						i = j - 1;
						break;
					}
					else
					{
						Temp += Options[j];
					}
				}
				if (Temp == "")
				{
					break;
				}
				else
				{
					cout << "|        |            ";
					maxLen = 96;
					for (unsigned int i = 0; i < Temp.length(); i++)
					{
						maxLen--;
						if (maxLen < 0)
						{
							maxLen = 96;
							cout << "|" << endl;
							Line++;
							cout << "|        |           ";
						}
						cout << Temp[i];
					}
					GoToXY(118, Line);
					cout << "|" << endl;
					Line++;
				}
			}
		}

		/*---Answer---*/
		cout << "|        |   Answer : ";
		Temp = Current->Answer;
		for (unsigned int i = 1; i < Temp.length(); i++)
		{
			Temp.insert(i, ", ");
			i += 2;
		}
		maxLen = 96;
		for (unsigned int i = 0; i < Temp.length(); i++)
		{
			maxLen--;
			if (maxLen < 0)
			{
				maxLen = 96;
				cout << "|" << endl;
				Line++;
				cout << "|        |           ";
			}
			cout << Temp[i];
		}
		GoToXY(118, Line);
		cout << "|" << endl;
		Line++;

		/*---Next Question---*/
		Current = Current->Next;
	}

	/*---Finishing---*/
	for (int i = 1; i <= 119; i++)
	{
		cout << "-";
	}
	cout << endl;
	Line++;

}



/*---Printout Subject List, Set ColorCode at option #Number---*/
/*---Number == -1---=>---All Line with Color(7)---------------*/
/*---Number == N----=>---Line #N with Color(ColorCode)--------*/
void ShowSubjectList(string Subject[], int _Subject, int ColorCode, int Number)
{
	int Line = 4;
	GoToXY(0, Line);
	SetColor(14);
	cout << "SUBJECT LIST";
	SetColor(7);
	for (int i = 0; i < _Subject; i++)
	{
		if (i == Number)
		{
			GoToXY(0, Line + 1 + i);
			cout << "                                ";
			GoToXY(2, Line + 1 + i);
			SetColor(ColorCode);
			cout << Subject[i];
			SetColor(7);
		}
		else
		{
			GoToXY(2, Line + 1 + i);
			cout << "                              ";
			GoToXY(2, Line + 1 + i);
			cout << Subject[i];
		}
	}
}



void Question_Info_List::ModSubject(User_Info *&Current_User)
{
	/*---Creating Subject List---*/
	string SubjectSring = Current_User->Subject;
	string Subject[100];
	int _Subject = -1;
	for (unsigned int i = 0; i < SubjectSring.length(); i++)
	{
		if (SubjectSring[i] == '|')
		{
			_Subject++;
			Subject[_Subject] = "";
		}
		else
		{
			Subject[_Subject] += SubjectSring[i];
		}
	}

	/*---Print out Subject list---*/
	ShowSubjectList(Subject, _Subject, 7, -1);

	/*---Coordinate---*/
	short x = 50;
	short y = 8;

	/*---Main Menu---*/
	int _Options = 4;	//Quantity of options
	int Chosen = 0;
	bool Flag = true;
	string Temp;
	SetColor(14);
	GoToXY(x + 5, y - 1);
	cout << "OPTIONS";
	string Options[4] = { "Add a Subject" , "Edit a Subject" , "Delete a Subject" , "Back" };
	while (Flag)
	{
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
		bool InputString;
		int Chosen2 = 0;
		switch (Chosen)
		{
		case 3:		//Back
			Flag = false;
			break;
		case 0:		//Add a Subject
			CreatingMenu(Options, _Options, -1, x, y);
			GoToXY(0, 5 + _Subject);
			SetColor(10);
			cout << "->";
			SetColor(7);
			InputString = true;
			Temp = "";
			while (InputString)
			{
				char Digit = _getch();
				switch (Digit)
				{
				case '\b':
					if (Temp.length() > 0)
					{
						cout << "\b \b";
						Temp.erase(Temp.length() - 1, 1);
					}
					break;
				case '\r':
					if (Temp == "")
					{
						break;
					}
					InputString = false;
					break;
				default:
					if (Temp.length() < 30)
					{
						Temp += Digit;
						cout << Digit;
					}
					break;
				}
			}
			transform(Temp.begin(), Temp.end(), Temp.begin(), toupper);
			GoToXY(0, 5 + _Subject);
			cout << "  " << Temp;
			Subject[_Subject] = Temp;
			_Subject++;
			break;
		case 1:		//Edit a Subject
			if (_Subject == 0)
			{
				break;
			}
			CreatingMenu(Options, _Options, -1, x, y);
			Chosen2 = 0;
			ShowSubjectList(Subject, _Subject, 42, Chosen2);
			while (true)
			{
				int Action = _getch();
				if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 72:
						if (Chosen2 == 0)
						{
							Chosen2 = _Subject - 1;
							ShowSubjectList(Subject, _Subject, 42, Chosen2);
						}
						else
						{
							Chosen2--;
							ShowSubjectList(Subject, _Subject, 42, Chosen2);
						}
						break;
					case 80:
						Chosen2++;
						Chosen2 %= _Subject;
						ShowSubjectList(Subject, _Subject, 42, Chosen2);
						break;
					}
				}
				else if (Action == 13)
				{
					GoToXY(0, 5 + Chosen2);
					SetColor(10);
					cout << "->                            ";
					SetColor(7);
					GoToXY(2, 5 + Chosen2);
					InputString = true;
					Temp = "";
					/*---Input a string----*/
					while (InputString)
					{
						char Digit = _getch();
						switch (Digit)
						{
						case '\b':
							if (Temp.length() > 0)
							{
								cout << "\b \b";
								Temp.erase(Temp.length() - 1, 1);
							}
							break;
						case '\r':
							if (Temp == "")
							{
								break;
							}
							InputString = false;
							break;
						default:
							/*---Length max = 30---*/
							if (Temp.length() < 30)
							{
								Temp += Digit;
								cout << Digit;
							}
							break;
						}
					}
					transform(Temp.begin(), Temp.end(), Temp.begin(), toupper);
					GoToXY(0, 5 + Chosen2);
					cout << "  " << Temp;
					Subject[Chosen2] = Temp;
					break;
				}
			}
			break;
		case 2:		//Delete a Subject
			if (_Subject == 0)
			{
				break;
			}
			CreatingMenu(Options, _Options, -1, x, y);
			Chosen2 = 0;
			ShowSubjectList(Subject, _Subject, 76, Chosen2);
			while (true)
			{
				int Action = _getch();
				if (Action == 224)
				{
					Action = _getch();
					switch (Action)
					{
					case 72:
						if (Chosen2 == 0)
						{
							Chosen2 = _Subject - 1;
							ShowSubjectList(Subject, _Subject, 76, Chosen2);
						}
						else
						{
							Chosen2--;
							ShowSubjectList(Subject, _Subject, 76, Chosen2);
						}
						break;
					case 80:
						Chosen2++;
						Chosen2 %= _Subject;
						ShowSubjectList(Subject, _Subject, 76, Chosen2);
						break;
					}
				}
				else if (Action == 13)
				{
					GoToXY(0, 4 + _Subject);
					SetColor(7);
					cout << "                              ";
					for (int i = Chosen2; i < _Subject - 1; i++)
					{
						Subject[i] = Subject[i + 1];
					}
					_Subject--;
					ShowSubjectList(Subject, _Subject, 7, -1);
					break;
				}
			}
			break;
		}
	}


	/*---Saving Subject back to Account Info*/
	SubjectSring = "|";
	for (int i = 0; i < _Subject; i++)
	{
		SubjectSring += Subject[i];
		SubjectSring += "|";
	}
	Current_User->Subject = SubjectSring;
}