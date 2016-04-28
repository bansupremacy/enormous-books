#include "User_Info_List.h"
#include "Display.h"
#include "Encryption.h"
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <ctime>
#include <iostream>


User_Info::User_Info()
{
	Subject = "|";
	Username = "";
	Password = "";
	Name = "";
	Day = 10;
	Month = 10;
	Year = 1999;
	Address = "";
	Sex = "";
	Type = 1;				//Admin = 1 / Teacher = 2 / Student = 3
	Status = true;			//(activated / deactivated)
	Next = NULL;
	Prev = NULL;
}

User_Info_List::User_Info_List()
{
	//---Initalize---
	Head = NULL;
	Auth = "|";
	//--------------------------------------------------
	//---Loading User List---
	ifstream inp;
	inp.open("User_Info_List.txt");
	User_Info *Current_User = Head;
	while (!inp.eof())
	{
		if (Current_User == NULL)
		{
			Current_User = new User_Info;
			getline(inp, Current_User->Subject);
			getline(inp, Current_User->Username);
			getline(inp, Current_User->Password);
			getline(inp, Current_User->Name);
			inp >> Current_User->Day;
			inp >> Current_User->Month;
			inp >> Current_User->Year;
			inp.get();
			getline(inp, Current_User->Address);
			getline(inp, Current_User->Sex);
			inp >> Current_User->Type;
			inp >> Current_User->Status;
			inp.get();
			if (Current_User->Subject != "")
			{
				Head = Current_User;
			}
		}
		else
		{
			Current_User->Next = new User_Info;
			Current_User->Next->Prev = Current_User;
			Current_User = Current_User->Next;
			getline(inp, Current_User->Subject);
			getline(inp, Current_User->Username);
			getline(inp, Current_User->Password);
			getline(inp, Current_User->Name);
			inp >> Current_User->Day;
			inp >> Current_User->Month;
			inp >> Current_User->Year;
			inp.get();
			getline(inp, Current_User->Address);
			getline(inp, Current_User->Sex);
			inp >> Current_User->Type;
			inp >> Current_User->Status;
			inp.get();
		}
	}
	inp.close();
}

User_Info_List::~User_Info_List()
{
	/*---Saving User List to File---*/
	bool FirstLine = true;
	ofstream outp;
	outp.open("User_Info_List.txt");
	User_Info *Current_User = Head;
	while (Current_User != NULL)
	{
		if (!FirstLine)
		{
			outp << endl;
		}
		else
		{
			FirstLine = false;
		}
		outp << Current_User->Subject << endl;
		outp << Current_User->Username << endl;
		outp << Current_User->Password << endl;
		outp << Current_User->Name << endl;
		outp << Current_User->Day << endl;
		outp << Current_User->Month << endl;
		outp << Current_User->Year << endl;
		outp << Current_User->Address << endl;
		outp << Current_User->Sex << endl;
		outp << Current_User->Type << endl;
		outp << Current_User->Status;
		Current_User = Current_User->Next;
	}
	outp.close();

	/*---Delete User List---*/
	User_Info *Current = Head;
	while (Current != NULL)
	{
		User_Info *Temp = Current;
		Current = Current->Next;
		delete Temp;
	}
}

string User_Info_List::CreateAuth()
{
	if (Auth != "|")
	{
		return Auth;
	}
	else
	{
		User_Info *Current = Head;
		while (Current != NULL)
		{
			Auth += Current->Username + "|" + Current->Password + "|";
			Current = Current->Next;
		}
		return Auth;
	}
}

User_Info **User_Info_List::GetHead()
{
	return &Head;
}

//This function find a user by username and return the pointer to that user

//Create a new user by stacking him on the list
void User_Info_List::CreateUser(User_Info* NewUser)
{
	//Adding TmpUser to the list
	if (Head == NULL)
	{
		Head = NewUser;
		Head->Next = NULL;
		Head->Prev = NULL;
	}
	else
	{
		NewUser->Next = Head;
		NewUser->Prev = NULL;
		Head->Prev = NewUser;
		Head = NewUser;
	}
}

//Create a new user according to alphabetic order
void User_Info_List::CreateUserDescending(User_Info* NewUser)
{
	/*---Updating Authentication String---*/
	Auth += NewUser->Username;
	Auth += "|";

	User_Info *Tmp = Head;
	User_Info *TmpPrev = NULL;

	if (Tmp == NULL)
	{
		this->CreateUser(NewUser);
		return;
	}

	while (Tmp != NULL)
	{
		if (CmpStringAlpha(NewUser->Username, Tmp->Username) == 1)
		{
			if (Tmp == Head)			//if Tmp is Head
			{
				this->CreateUser(NewUser);
				return;
			}
			else                       //if Tmp is in the middle
			{
				NewUser->Next = Tmp;
				NewUser->Prev = Tmp->Prev;
				Tmp->Prev->Next = NewUser;
				Tmp->Prev = NewUser;
				return;
			}
		}
		else
		{
			TmpPrev = Tmp;			//save Tmp is Tmp is next to NULL
			Tmp = Tmp->Next;
		}
	}
	TmpPrev->Next = NewUser;		//if no Tmp is after NewUser
	NewUser->Prev = TmpPrev;
	NewUser->Next = NULL;
}

//Remove a User link
void User_Info_List::Remove(User_Info *DeleteUser)
{
	/*---Updating Authentication String---*/
	Auth.erase(Auth.find(DeleteUser->Username) - 1, DeleteUser->Username.length() + 1);

	if (DeleteUser->Prev) //check if the previous node is a NULL
		DeleteUser->Prev->Next = DeleteUser->Next; //if not set the next pointer of that to the next pointer of the current
	if (DeleteUser->Next) //the same process as above
		DeleteUser->Next->Prev = DeleteUser->Prev;
	if (DeleteUser == Head) //check if the DeleteUser is the Head, if yes set Head to next node
		Head = DeleteUser->Next;
	delete DeleteUser;

}

//Show only a specific user
void User_Info::showAUser()
{
	cout << "Username: " << this->Username
		<< "\nName: " << this->Name
		<< "\nDate of birth: " << this->Day << '/' << this->Month << '/' << this->Year
		<< "\nAddress: " << this->Address
		<< "\nSex: " << this->Sex
		<< "\nSubject: " << this->Subject
		<< "\nType: " << this->Type
		<< "\nStatus: " << this->Status << endl;
}

//Export XML
void User_Info_List::ExportXML(string filename)
{
	//------The structure for this XML is
	//---root: UserList
	//---child F1: User - attribute: Type
	//---child F2: Username - attribute: Student?, Name, DOB, Address, Sex, Status
	//---child F3: (of DOB): Day, Month, Year

	User_Info *Tmp = Head;
	ofstream of;
	of.open(filename.c_str());
	if (of.is_open())
	{
		of << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		of << "<userlist>\n"; //root

		while (Tmp != NULL)
		{
			of << "<user type=\"";	//<user>
			switch (Tmp->Type)
			{
			case 1: of << "admin"; break;
			case 2: of << "teacher"; break;
			case 3: of << "student"; break;
			default: of << "error"; break;
			}
			of << "\">\n";

			of << "<username>" << Tmp->Username << "</username>\n" //<username>
				<< "<name>" << Tmp->Name << "</name>\n" //<name>
				<< "<dob>" //<dob>
				<< "<day>" << Tmp->Day << "</day>\n"
				<< "<month>" << Tmp->Month << "</month>\n"
				<< "<year>" << Tmp->Year << "</year>\n" << "</dob>\n" //</dob>
				<< "<address>" << Tmp->Address << "</address>\n" //<address>
				<< "<sex>" << Tmp->Sex << "</sex>\n" //<sex>
				<< "<subject>" << Tmp->Subject << "</subject>\n" //<subject>
				<< "<status>"; //<status>
			if (Tmp->Status)
				of << "Activated";
			else
				of << "Deactivated";
			of << "</status>\n" //</status>
				<< "</user>\n"; //</user>
			Tmp = Tmp->Next;
		}
		of << "</userlist>\n"; //end
	}
	else
		cout << "_ERROR_READFILE\n";

	of.close();
}
//========================================================================


//=====================USER-INTERFACE RELATED FUNCTIONS===================

//View Accounts inclduing remove, ban and reset password (User interface)
void ViewAccount(User_Info_List *&List, int ViewMode, User_Info *Current_Admin)//1: View All, 2: Type, 3: Status, 4: SimilarUsername
{
	short x = 40;				//the coordinates
	short y = 8;				//the coordinates
	string _Username = "";	//similar username
	int _Status = 0;		//0: deactivated, 1: activated, 2: QUIT
	int _Type = 1;			//1: ADMIN, 2: TEACHER, 3: STUDENT, 4:QUIT
	int Key;				//receive keyboard input
	User_Info **Arr = NULL;	//the array of pointer of User_Info
	int ArrSize = 0;		//the array size
	User_Info *Quit;        //the pointer to indicate end of array;
	int No;					//the index of the array
	int Operation = 1;      //1:REMOVE | 2:BAN | 3:RESET PASSWORD
	bool Approve = true;    //choose YES NO

							//print out the layout first
	switch (ViewMode)
	{
	case 1:         //VIEW ALL
		GoToXY(x, y);
		cout << "List of all Accounts: ";
		break;

	case 2:         //TYPE
		GoToXY(x, y);
		cout << "Type: ";
		SetColor(0 + 7 * 16);
		cout << "ADMIN";
		SetColor(7);
		cout << "/TEACHER/STUDENT/QUIT";
		break;

	case 3:         //STATUS
		GoToXY(x, y);
		cout << "Status: ";
		SetColor(0 + 7 * 16);
		cout << "DEACTIVATED";
		SetColor(7);
		cout << "/ACTIVATED/QUIT";
		break;

	case 4:         //USERNAME
		GoToXY(x, y);
		cout << "Username: ";
		cout << _Username;
		break;
	}

	while (true)
	{
		while (true)
		{
			if (ViewMode == 1)
			{
				break;
			}

			else if (ViewMode == 2)      //TYPE
			{
				Key = _getch();
				if (Key == 13)					//if key is [Enter], escape the loop
					break;
				if (Key != 224)					//if key is not arrow key, reset the loop
					continue;

				Key = _getch();
				if (Key == 72 || Key == 80)		//if key is up and down, reset the loop
					continue;

				GoToXY(x + 6, y);               //reprint the layout in default fashion in order to make highlight
				cout << "ADMIN/TEACHER/STUDENT/QUIT";
				SetColor(0 + 7 * 16);           //turn on the highlight mode

				switch (Key)
				{
				case 77:                        //if key is right, increment the Type
					_Type++;
					if (_Type > 4)              //if Type meets upper limit (4), reset Type to 1 (QUIT to ADMIN)
						_Type = 1;
					break;

				case 75:                    	//if key is left, decrement the Type
					_Type--;
					if (_Type < 1)              //if Type meets lower limit (1), reset Type to 4 (ADMIN to QUIT)
						_Type = 4;
					break;
				default: break;                 //handle any garbage
				}
				switch (_Type)                  //this highlight the new chosen Type
				{
				case 1: GoToXY(x + 6, y); cout << "ADMIN"; break;
				case 2: GoToXY(x + 12, y); cout << "TEACHER"; break;
				case 3: GoToXY(x + 20, y); cout << "STUDENT"; break;
				case 4: GoToXY(x + 28, y); cout << "QUIT"; break;
				}
				SetColor(7);
			}

			else if (ViewMode == 3)         //STATUS
			{
				Key = _getch();
				if (Key == 13)					//if key is [Enter], escape the loop
					break;
				if (Key != 224)					//if key is not arrow key, reset the loop
					continue;

				Key = _getch();
				if (Key == 72 || Key == 80)		//if key is up and down, reset the loop
					continue;

				GoToXY(x + 8, y);					//reprint the layout in default fashion in order to make highlight
				cout << "DEACTIVATED/ACTIVATED/QUIT";
				SetColor(0 + 7 * 16);	//turn on the highlight mode

				switch (Key)
				{
				case 77:			//if key is right, increment the _Status
					_Status++;
					if (_Status > 2)	//if _Status meets upper limit (2), reset _Status to 0 (QUIT to DEACTIVATED)
						_Status = 0;
					break;

				case 75:			//if key is left, decrement the _Status
					_Status--;
					if (_Status < 0) //if _Status meets lower limit (0), reset _Status to 2 (DEACTIVATED to QUIT)
						_Status = 2;
					break;
				default: break;		//handle any garbage
				}
				switch (_Status)		//this highlight the new chosen _Status
				{
				case 0: GoToXY(x + 8, y); cout << "DEACTIVATED"; break;
				case 1: GoToXY(x + 20, y); cout << "ACTIVATED"; break;
				case 2: GoToXY(x + 30, y); cout << "QUIT"; break;
				}
				SetColor(7);
			}

			else if (ViewMode == 4)         //SIMILAR USERNAME
			{
				Key = _getch();

				if (Key == 13)
					if (_Username != "")
						break;
					else
						continue;

				if (Key == 224)				//not allow arrow keys
				{
					Key = _getch();
					continue;
				}

				if (Key == 8)			//if backspace is found
				{
					if (_Username != "")		//if username string is not empty
					{
						_Username.pop_back();			//pop out one character
						cout << (char)Key;				//this only moves the cursor back one character, not deletes the current character
						cout << ' ';					//so this one does the job, but it moves the cursor to the right again
						cout << (char)Key;				//so this one moves the cursor back again
					}
				}
				else
				{
					cout << (char)Key;		//if things all good, echo the input
					_Username += (char)Key;	//and add it into the string Username
				}
			}
		}

		if (_Type == 4 || _Status == 2)         //if QUIT of TYPE or STATUS is choosen
			return;

		//this block of code is to copy the searched list into a dynamic array (put in block to remove the TmpArr)
		{
			ArrSize = 0;
			User_Info *TmpArr[100000];
			User_Info *Tmp = *(List->GetHead());
			while (Tmp != NULL)
			{
				if (ViewMode == 1)              //VIEW ALL
				{
					TmpArr[ArrSize] = Tmp;
					ArrSize++;
				}

				else if (ViewMode == 2)
				{
					if (Tmp->Type == _Type)     //TYPE
					{
						TmpArr[ArrSize] = Tmp;
						ArrSize++;
					}
				}
				else if (ViewMode == 3)
				{
					//if (Tmp->Status == _Status) //STATUS
					if (((Tmp->Status == true) && (_Status == 1)) || ((Tmp->Status == false) && (_Status == 0)))
					{
						TmpArr[ArrSize] = Tmp;
						ArrSize++;
					}
				}
				else if (ViewMode == 4)         //SIMILAR USERNAME
				{
					if (Tmp->Username.find(_Username) != string::npos)
					{
						TmpArr[ArrSize] = Tmp;
						ArrSize++;
					}
				}
				Tmp = Tmp->Next;
			}
			Arr = new User_Info*[ArrSize + 1];          //one extra slots to store the Quit pointer
			for (int i = 0; i < ArrSize; i++)
				Arr[i] = TmpArr[i];
		}
		Quit = new User_Info;
		Quit->Username = "Quit";            //init the Quit pointer
		Arr[ArrSize] = Quit;

		No = 0;			//set the index to default (0)

						//print out the list of Users satisfying the condition
		/*SetColor(10);*/ SetColor(0 + 7 * 16);
		for (int i = 0; i <= ArrSize; i++)
		{
			if (Arr[i] != NULL)
			{
				GoToXY(x, y + 2 + i);
				cout << Arr[i]->Username;
				SetColor(7);
			}
		}

		//traverse the list of searched Users
		GoToXY(x, y + 2 + No);
		while (true)
		{
			while (true)
			{
				Key = _getch();

				if (Key == 13)
					break;

				if (Key != 224)
					continue;
				Key = _getch();

				if (Key == 77 || Key == 75)
					continue;

				SetColor(7);
				GoToXY(x, y + 2 + No);
				if (Arr[No] != NULL)
					cout << Arr[No]->Username;
				/*SetColor(10);*/ SetColor(0 + 7 * 16);
				switch (Key)
				{
				case 72:		//up
					do
					{
						No--;
						if (No < 0)
							No = ArrSize;
					} while (Arr[No] == NULL);        //check if Arr[No] is already deleted

					GoToXY(x, y + 2 + No);
					cout << Arr[No]->Username;
					break;

				case 80:		//down
					do
					{
						No++;
						if (No == ArrSize + 1)
							No = 0;
					} while (Arr[No] == NULL);        //there is always a Quit pointer to reside on

					GoToXY(x, y + 2 + No);
					cout << Arr[No]->Username;
					break;
				}
			}

			if (No == ArrSize)          //if Quit is chosen
				break;          //return to the Choosing menu

								//if a User is choosen, print out the Information and Operations

								//Information
			SetColor(7);
			GoToXY(x, y + 2 + ArrSize + 2);
			cout << "==============================";
			GoToXY(x, y + 2 + ArrSize + 3);
			cout << "      Name : " << Arr[No]->Name;
			GoToXY(x, y + 2 + ArrSize + 4);
			cout << "       DOB : " << Arr[No]->Day << '/' << Arr[No]->Month << '/' << Arr[No]->Year;
			GoToXY(x, y + 2 + ArrSize + 5);
			cout << "      Type : ";
			switch (Arr[No]->Type)
			{
			case 1: cout << "ADMIN"; break;
			case 2: cout << "TEACHER"; break;
			case 3: cout << "STUDENT"; break;
			default: break;
			}
			GoToXY(x, y + 2 + ArrSize + 6);
			if (Arr[No]->Type != 2)
				SetColor(8);
			cout << "   Subject : ";
			SetColor(7);
			if (Arr[No]->Type == 2)
				cout << Arr[No]->Subject;
			GoToXY(x, y + 2 + ArrSize + 7);
			cout << "   Address : " << Arr[No]->Address;
			GoToXY(x, y + 2 + ArrSize + 8);
			cout << "       Sex : " << Arr[No]->Sex;
			GoToXY(x, y + 2 + ArrSize + 9);
			cout << "    Status : ";
			if (Arr[No]->Status == 0)
				cout << "DEACTIVATED";
			else
				cout << "ACTIVATED";
			GoToXY(x, y + 2 + ArrSize + 10);
			cout << "==============================";
			if (Arr[No] == Current_Admin)
			{
				GoToXY(x, y + 2 + ArrSize + 11);
				SetColor(0 + 7 * 16);
				cout << "Press [Enter] to continue\n";
				Pause();
				SetColor(7);
				GoToXY(x, y + 2 + ArrSize + 11);
				cout << "                           ";
				GoToXY(x, y + 2 + No);
				cout << Arr[No]->Username;
			}
			else
			{
				//Operations
				Operation = 1;
				GoToXY(x, y + 2 + ArrSize + 11);
				SetColor(0 + 7 * 16);
				cout << "REMOVE";
				SetColor(7);
				cout << "/BAN/RESET PASSWORD";

				while (true)
				{
					Key = _getch();
					if (Key == 13)					//if key is [Enter], escape the loop
						break;
					if (Key != 224)					//if key is not arrow key, reset the loop
						continue;

					Key = _getch();
					if (Key == 72 || Key == 80)		//if key is up and down, reset the loop
						continue;

					GoToXY(x, y + 2 + ArrSize + 11);					//reprint the layout in default fashion in order to make highlight
					cout << "REMOVE/BAN/RESET PASSWORD";
					SetColor(0 + 7 * 16);	//turn on the highlight mode

					switch (Key)
					{
					case 77:			//if key is right, increment the Type
						Operation++;
						if (Operation > 3)	//if Operation meets upper limit (3), reset Type to 1 (RESET PASS TO REMOVE)
							Operation = 1;
						break;

					case 75:			//if key is left, decrement the Type
						Operation--;
						if (Operation < 1) //if Type meets lower limit (1), reset Type to 3 (REMOVE TO RESETPASS)
							Operation = 3;
						break;
					default: break;		//handle any garbage
					}
					switch (Operation)		//this highlight the new chosen Operation
					{
					case 1: GoToXY(x, y + 2 + ArrSize + 11); cout << "REMOVE"; break;
					case 2: GoToXY(x + 7, y + 2 + ArrSize + 11); cout << "BAN"; break;
					case 3: GoToXY(x + 11, y + 2 + ArrSize + 11); cout << "RESET PASSWORD"; break;
					}
					SetColor(7);
				}

				//if a Operation is choosen
				GoToXY(x, y + 2 + ArrSize + 13);
				cout << "Are you sure? ";
				cout << '<';
				SetColor(0 + 7 * 16);
				cout << "YES";
				SetColor(7);
				cout << "> <NO>";

				while (true)
				{
					Key = _getch();
					if (Key == 13)
						break;
					if (Key != 224)
						continue;

					Key = _getch();
					if (Key == 72 || Key == 80)
						continue;

					GoToXY(x + 14, y + 2 + ArrSize + 13);
					SetColor(7);
					cout << "<YES> <NO>";
					SetColor(0 + 7 * 16);

					switch (Key)
					{
					case 77:
						if (Approve)
							Approve = false;
						else
							Approve = true;
						break;

					case 75:
						if (!Approve)
							Approve = true;
						else
							Approve = false;
						break;
					}
					switch (Approve)
					{
					case true: GoToXY(x + 15, y + 2 + ArrSize + 13); cout << "YES"; break;
					case false: GoToXY(x + 21, y + 2 + ArrSize + 13); cout << "NO"; break;
					}
					SetColor(7);
				}

				//if YES is choosen
				if (Approve)
				{
					switch (Operation)
					{
					case 1:         //REMOVE
						GoToXY(x, y + 2 + No);
						SetColor(8);
						cout << Arr[No]->Username;          //grey out the deleted Username
						List->Remove(Arr[No]);
						Arr[No] = NULL;
						GoToXY(x, y + 2 + ArrSize + 14);
						/*SetColor(10);*/ SetColor(0 + 7 * 16);
						cout << "User deleted. Press [Enter] to continue";
						break;

					case 2:         //BAN
						Arr[No]->Status = false;
						GoToXY(x, y + 2 + ArrSize + 14);
						/*SetColor(10);*/ SetColor(0 + 7 * 16);
						cout << "User banned. Press [Enter] to continue";
						break;

					case 3:         //RESET PASSWORD
						Arr[No]->Password = Encrypt(Arr[No]->Password);
						Arr[No]->Password = Encrypt(Arr[No]->Password);
						GoToXY(x, y + 2 + ArrSize + 14);
						SetColor(7);
						cout << "New password: ";
						/*SetColor(10);*/ SetColor(0 + 7 * 16);
						cout << Arr[No]->Password;
						SetColor(7);
						cout << " - Press [Enter] to continue";
						Arr[No]->Password = Encrypt(Arr[No]->Password);
						break;
					}
				}
				else
				{
					GoToXY(x, y + 2 + ArrSize + 14);
					/*SetColor(10);*/ SetColor(0 + 7 * 16);
					cout << "Cancelled - Press [Enter] to continue";
					SetColor(7);
				}

				cin.get();

				//if NO is choosen or Operation != REMOVE
				if (Approve == false || Operation != 1)
				{
					GoToXY(x, y + 2 + No);
					SetColor(7);
					cout << Arr[No]->Username;  //reprint the choosen Username in normal fashion
				}
			}
			SetColor(7);
			ClearScreen(x, y + 2 + ArrSize + 2);	//clear the screen
			Approve = true;             //reset YES NO to YES
			No = ArrSize;               //jump to Quit line		
			GoToXY(x, y + 2 + No);
			SetColor(0 + 7 * 16);
			cout << "Quit";
		}
		delete Quit;
		delete[] Arr;
		SetColor(7);
		ClearScreen(); //clear the screen

		if (ViewMode == 1)               //VIEW ALL
			return;
		else if (ViewMode == 2)          //TYPE
		{
			GoToXY(x, y);
			cout << "Type: ";
			GoToXY(x + 6, y);					//reprint the layout in default fashion in order to make highlight
			cout << "ADMIN/TEACHER/STUDENT/QUIT";
			SetColor(0 + 7 * 16);				//turn on the highlight mode
			switch (_Type)						//this highlight the new chosen Type
			{
			case 1: GoToXY(x + 6, y); cout << "ADMIN"; break;
			case 2: GoToXY(x + 12, y); cout << "TEACHER"; break;
			case 3: GoToXY(x + 20, y); cout << "STUDENT"; break;
			case 4: GoToXY(x + 28, y); cout << "QUIT"; break;
			}
		}
		else if (ViewMode == 3)      //STATUS
		{
			GoToXY(x, y);
			cout << "Status: ";
			GoToXY(x + 8, y);					//reprint the layout in default fashion in order to make highlight
			cout << "DEACTIVATED/ACTIVATED/QUIT";
			SetColor(0 + 7 * 16);				//turn on the highlight mode
			switch (_Status)					//this highlight the new chosen _Status
			{
			case 0: GoToXY(x + 8, y); cout << "DEACTIVATED"; break;
			case 1: GoToXY(x + 20, y); cout << "ACTIVATED"; break;
			case 2: GoToXY(x + 30, y); cout << "QUIT"; break;
			}
		}
		else if (ViewMode == 4)     //USERNAME
		{
			GoToXY(x, y);
			/*SetColor(10);*/ SetColor(0 + 7 * 16);
			cout << "Press [Enter] to continue, [q] to Quit";
			while (true)
			{
				Key = _getch();
				if (Key != 13 && Key != 113)
				{
					if (Key == 224)
						Key = _getch();
					continue;
				}
				else
					break;
			}

			if (Key == 113)
			{
				SetColor(7);
				return;
			}
			else
			{
				GoToXY(x, y);
				SetColor(7);
				cout << "                                                 ";
				SetColor(7);
				GoToXY(x, y);
				cout << "Username: ";
				cout << _Username;
			}
		}
		SetColor(7);
	}
}
//Modify a user information (User interfacte)
void ModAUserUI(User_Info *&User)
{
	CONSOLE_SCREEN_BUFFER_INFO CurrentPosPass;
	CONSOLE_SCREEN_BUFFER_INFO CurrentPos;	//use this to get the current position of the cursor
	short x = 50;							//the coordinates for the GoToXY function - move the cursor to a specific point
	short y = 8;							//the coordinates for the GoToXY function - move the cursor to a specific point
	int Menu = 1;                           //1:Password, 2:Name, 3: DOB, 4: Address, 5: Sex, 6: Quit;
	int Key;								//the key to receive unbuffered input from keyboard
	int dayTop = 31;						//the limit the Day section can reach in DOB
	int monthTop = 12;						//the limit the Month section can reach in DOB
	int State = 1;							//1 for day, 2 for month, 3 for year - this is used in DOB
	bool ShowFlag = false;

	//the layout for this menu is
	//What do you want to change?
	//Username
	//Password
	//Name
	//DOB
	//Address
	//Sex
	//Subject (will not be greyed out if ADMIN or STUDENT)

	SetColor(14);
	GoToXY(x + 5, y);
	cout << "OPTIONS";
	SetColor(7);
	SetColor(0 + 7 * 16);	//0 for font black, 7 for background grey
	GoToXY(x, y + 1);
	cout << "Password";
	SetColor(7);			//7 for font grey (the default)
	GoToXY(x + 4, y + 2);
	cout << "Name";
	GoToXY(x + 5, y + 3);
	cout << "DOB";
	GoToXY(x + 1, y + 4);
	cout << "Address";
	GoToXY(x + 5, y + 5);
	cout << "Sex";
	GoToXY(x + 6, y + 6);
	cout << "Back";
	GoToXY(x + 8, y + 1);	//set the cursor on the password line

	while (true)	//the outer loop to handle 2 things: select and initiate
	{
		//this inner loop handle the selection
		while (true)
		{
			Key = _getch();
			if (Key == 13)					//if key is [Enter], escape the loop
				break;
			if (Key != 224)					//if key is not arrow key, reset the loop
				continue;

			Key = _getch();
			if (Key == 77 || Key == 75)		//if key is right and left, reset the loop
				continue;

			SetColor(7);	//reprint the list in default fashion
			switch (Menu)
			{
			case 1:
				GoToXY(x, y + 1);
				cout << "Password";
				break;
			case 2:
				GoToXY(x + 4, y + 2);
				cout << "Name";
				break;
			case 3:
				GoToXY(x + 5, y + 3);
				cout << "DOB";
				break;
			case 4:
				GoToXY(x + 1, y + 4);
				cout << "Address";
				break;
			case 5:
				GoToXY(x + 5, y + 5);
				cout << "Sex";
				break;
			case 6:
				GoToXY(x + 6, y + 6);
				cout << "Back";
				break;
			}

			switch (Key)
			{
			case 80:			//if key is down, increment the Menu
				Menu++;
				if (Menu > 6)
					Menu = 1;
				break;

			case 72:			//if key is up, decrement the Menu
				Menu--;
				if (Menu < 1)
					Menu = 6;
				break;
			default: continue;		//handle any garbage
			}

			SetColor(0 + 7 * 16);	//turn on the highlight mode

			switch (Menu)		//this highlight and initiate the new chosen Menu
			{
			case 1:
				GoToXY(x, y + 1);
				cout << "Password";
				break;
			case 2:
				GoToXY(x + 4, y + 2);
				cout << "Name";
				break;
			case 3:
				GoToXY(x + 5, y + 3);
				cout << "DOB";
				break;
			case 4:
				GoToXY(x + 1, y + 4);
				cout << "Address";
				break;
			case 5:
				GoToXY(x + 5, y + 5);
				cout << "Sex";
				break;
			case 6:
				GoToXY(x + 6, y + 6);
				cout << "Back";
				break;
			}
			SetColor(7);	//reset color mode to default
		}
		if (Menu == 6) //Quit
			return;
		//this inner switch handle the initiation (escape the outer loop if Quit is met)
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);
		cout << " : ";
		switch (Menu)
		{
		case 1:	//Password
			User->Password = "";
			while (true)
			{
				Key = _getch();

				if (Key == 224)
				{
					Key = _getch();
					continue;
				}

				if (Key == 13)
					if (ShowFlag != true && User->Password != "")
						break;
					else
						continue;

				if (!isalnum((char)Key) && Key != (int) '_' && Key != 8)		//only alllow alphanum and _
					continue;

				if (Key == 8)
				{
					if (User->Password != "")
					{
						User->Password.pop_back();
						cout << (char)Key;
						cout << ' ';
						cout << (char)Key;
					}
				}
				else
				{
					cout << '*';
					User->Password += (char)Key;
				}

				if (ShowFlag == false && User->Password.length() < 6)		//this check if the password is longer than 6 characters
				{
					ShowFlag = true;
					GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPosPass);	//get the current cursor pos
					GoToXY(x, y + 10);		//jump down several lines of code
					/*SetColor(10);*/ SetColor(0 + 7 * 16);
					cout << "Password must be from 6 characters and up";
					SetColor(7);
					GoToXY(CurrentPosPass.dwCursorPosition.X, CurrentPosPass.dwCursorPosition.Y);	//get back to where it was previously
				}

				if (ShowFlag == true && User->Password.length() >= 6)
				{
					ShowFlag = false;
					GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPosPass);
					GoToXY(x, y + 10);
					cout << "                                           ";	//if nothing happens, print this
					GoToXY(CurrentPosPass.dwCursorPosition.X, CurrentPosPass.dwCursorPosition.Y);
				}
			}
			ShowFlag = false;
			User->Password = Encrypt(User->Password);
			break;

		case 2:	//Name
			cout << User->Name;
			while (true)
			{
				Key = _getch();

				if (Key == 224)
				{
					Key = _getch();
					continue;
				}

				if (Key == 13)
					if (User->Name != "")
						break;
					else
						continue;


				if (Key == 8)
				{
					if (User->Name != "")
					{
						User->Name.pop_back();
						cout << (char)Key;
						cout << ' ';
						cout << (char)Key;
					}
				}
				else
				{
					cout << (char)Key;
					User->Name += (char)Key;
				}
			}
			break;

		case 3:	//DOB
			SetColor(0 + 7 * 16); //highlight mode on
			if (User->Day < 10)
				cout << ' ';
			cout << User->Day; //highlight the Day first
			SetColor(7);
			cout << '/';
			if (User->Month < 10)
				cout << ' ';
			cout << User->Month << '/' << User->Year;	//print Month and Year in normal mode

			while (true)
			{
				Key = _getch();
				if (Key == 13 && validDate(User->Day, User->Month, User->Year)) //if day is not valid, cannot [enter]
					break;
				if (Key != 224)
					continue;
				Key = _getch();
				switch (Key)
				{
				case 72:			//Key is uppppp, incrementtttt
					switch (State)
					{
					case 1:		//DAY
						User->Day++;
						if (User->Day > dayTop)	//meet limit -> return to 1
							User->Day = 1;
						break;

					case 2:		//MONTH
						User->Month++;
						if (User->Month > monthTop)
							User->Month = 1;
						break;

					case 3:		//YEAR
						User->Year++;
						break;
					}
					break;

				case 80:			//Key is downnnn, decrementtttt
					switch (State)
					{
					case 1:
						User->Day--;
						if (User->Day < 1)
							User->Day = dayTop;
						break;

					case 2:
						User->Month--;
						if (User->Month < 1)
							User->Month = monthTop;
						break;

					case 3:
						User->Year--;
						break;
					}
					break;

				case 77:		//Key is right, this one change the State, so that it alters between Day, Month, Year
					State++;
					if (State > 3)
						State = 1;
					break;

				case 75:		//Key is left, the same as above
					State--;
					if (State < 1)
						State = 3;
					break;
				}

				//so these below are the phase to highlight or change the current section (Day/Month/Year)
				if (Key == 72 || Key == 80)		//if key is up or down
				{
					switch (State)
					{
					case 1:		//change the Day
						GoToXY(CurrentPos.dwCursorPosition.X + 3, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);
						if (User->Day < 10)
							cout << ' ';
						cout << User->Day;
						SetColor(7);
						break;

					case 2:		//change the Month
						GoToXY(CurrentPos.dwCursorPosition.X + 6, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);
						if (User->Month < 10)
							cout << ' ';
						cout << User->Month;
						SetColor(7);
						break;

					case 3:		//change the Year
						GoToXY(CurrentPos.dwCursorPosition.X + 9, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);
						cout << User->Year;
						SetColor(7);
						break;
					}
				}
				if (Key == 77 || Key == 75) //if key is right or left
				{
					//Reprint the layout in normal fashion first
					GoToXY(CurrentPos.dwCursorPosition.X + 3, CurrentPos.dwCursorPosition.Y);
					if (User->Day < 10)
						cout << ' ';
					cout << User->Day << '/';
					if (User->Month < 10)
						cout << ' ';
					cout << User->Month << '/';
					cout << User->Year;

					//then highlight the new section
					switch (State)
					{
					case 1:
						GoToXY(CurrentPos.dwCursorPosition.X + 3, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);

						if (User->Day < 10)
							cout << ' ' << User->Day;
						else
							cout << User->Day;
						SetColor(7);
						break;

					case 2:
						GoToXY(CurrentPos.dwCursorPosition.X + 6, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);

						if (User->Month < 10)
							cout << ' ' << User->Month;
						else
							cout << User->Month;
						SetColor(7);
						break;

					case 3:
						GoToXY(CurrentPos.dwCursorPosition.X + 9, CurrentPos.dwCursorPosition.Y);
						SetColor(0 + 7 * 16);
						cout << User->Year;
						SetColor(7);
						break;
					}
				}

				//the process below is like the check for taken Username above
				if (!validDate(User->Day, User->Month, User->Year))		//this check for valid date
				{
					GoToXY(CurrentPos.dwCursorPosition.X + 14, CurrentPos.dwCursorPosition.Y);
					cout << "Invalid DOB";
				}
				else
				{
					GoToXY(CurrentPos.dwCursorPosition.X + 14, CurrentPos.dwCursorPosition.Y);
					cout << "            ";
				}
			}
			State = 1;
			break;

		case 4:	//Address
			cout << User->Address;
			while (true)
			{
				Key = _getch();

				if (Key == 224)
				{
					Key = _getch();
					continue;
				}

				if (Key == 13)
					break;

				if (Key == 8)
				{
					if (User->Address != "")
					{
						User->Address.pop_back();
						cout << (char)Key;
						cout << ' ';
						cout << (char)Key;
					}
				}
				else
				{
					cout << (char)Key;
					User->Address += (char)Key;
				}
			}
			break;

		case 5:	//Sex
			cout << User->Sex;
			while (true)
			{
				Key = _getch();

				if (Key == 224)
				{
					Key = _getch();
					continue;
				}

				if (Key == 13)
					break;

				if (Key == 8)
				{
					if (User->Sex != "")
					{
						User->Sex.pop_back();
						cout << (char)Key;
						cout << ' ';
						cout << (char)Key;
					}
				}
				else
				{
					cout << (char)Key;
					User->Sex += (char)Key;
				}
			}
			break;

		default: break;
		}
		GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
		cout << "                                                   "; //clean any user input
		GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
	}
}

//Create a user (User interface)
void CreateUserUI(User_Info_List *&List)
{
	CONSOLE_SCREEN_BUFFER_INFO CurrentPos;	//use this to get the current position of the cursor
	short x = 45;							//the coordinates for the GoToXY function - move the cursor to a specific point
	short y = 8;							//the coordinates for the GoToXY function - move the cursor to a specific point
	User_Info *NewUser = new User_Info;     //this is the User node to add into the list
	int Key;								//the key to receive unbuffered input from keyboard
	int dayTop = 31;						//the limit the Day section can reach in DOB
	int monthTop = 12;						//the limit the Month section can reach in DOB
	int State = 1;							//1 for day, 2 for month, 3 for year - this is used in DOB
	bool ShowFlag = false;					//this is the flag used in displaying user input mistakes in USERNAME and PASSWORD

	if (NewUser) //if created successfully
	{
		//pre-print all of the categories first, then send the cursor back to the input position of Type: first
		//then continue to the below
		GoToXY(x, y);
		cout << "        Type : ";
		GoToXY(x, y + 1);
		cout << "    Username : ";
		GoToXY(x, y + 2);
		cout << "    Password : ";
		GoToXY(x, y + 3);
		cout << "        Name : ";
		GoToXY(x, y + 4);
		cout << "Day of birth : ";
		GoToXY(x, y + 5);
		cout << "     Address : ";
		GoToXY(x, y + 6);
		cout << "         Sex : ";
		GoToXY(x, y + 7);
		cout << "     Subject : ";
		GoToXY(x, y + 8);
		cout << "   Activate? ";

		//-----------------------------------TYPE-------------------------------------
		//----------------------------------------------------------------------------

		//the layout for this is ADMIN/TEACHER/STUDENT, user will use left and right key to choose

		GoToXY(x + 15, y);
		SetColor(0 + 7 * 16);	//0 for font black, 7 for background grey
		cout << "ADMIN";
		SetColor(7);			//7 for font grey (the default)
		cout << "/TEACHER/STUDENT";

		while (true)
		{
			Key = _getch();
			if (Key == 13)					//if key is [Enter], escape the loop
				break;
			if (Key != 224)					//if key is not arrow key, reset the loop
				continue;

			Key = _getch();
			if (Key == 72 || Key == 80)		//if key is up and down, reset the loop
				continue;

			SetColor(7);
			GoToXY(x + 15, y);					//reprint the layout in default fashion in order to make highlight
			cout << "ADMIN/TEACHER/STUDENT";
			SetColor(0 + 7 * 16);	//turn on the highlight mode

			switch (Key)
			{
			case 77:			//if key is right, increment the Type
				NewUser->Type++;
				if (NewUser->Type > 3)	//if Type meets upper limit (3), reset Type to 1 (STUDENT to ADMIN)
					NewUser->Type = 1;
				break;

			case 75:			//if key is left, decrement the Type
				NewUser->Type--;
				if (NewUser->Type < 1) //if Type meets lower limit (1), reset Type to 3 (ADMIN to STUDENT)
					NewUser->Type = 3;
				break;
			default: break;		//handle any garbage
			}
			switch (NewUser->Type)		//this highlight the new chosen Type
			{
			case 1: GoToXY(x + 15, y); cout << "ADMIN"; break;
			case 2: GoToXY(x + 21, y); cout << "TEACHER"; break;
			case 3: GoToXY(x + 29, y); cout << "STUDENT"; break;
			}
			SetColor(7);	//reset color mode to default
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		//SET THE COLOR OF THE SUBJECT SECTION TO GREY IF NOT TEACHER(BECAUSE ONLY TEACHER CAN USE)
		if (NewUser->Type != 2)
		{
			GoToXY(x, y + 7);
			SetColor(8);
			cout << "     Subject : ";
			SetColor(7);
		}




		//------------------------------USERNAME--------------------------------------
		//----------------------------------------------------------------------------
		GoToXY(x, y + 14);
		SetColor(10);
		if (NewUser->Type == 3)		//If NewUser is student
		{
			cout << "Only numeric ID is allowed";
		}
		else
		{
			cout << "Only alphanumberic characters and \"_\" is allowed";
		}
		SetColor(7);
		GoToXY(x + 15, y + 1);
		while (true)
		{
			Key = _getch();

			if (Key == 224)				//not allow arrow keys
			{
				Key = _getch();
				continue;
			}

			if (Key == 13)
				if (NewUser->Username != "" && !ShowFlag)	//if the username is already taken, [enter] is not allowed
					break;
				else
					continue;

			if (NewUser->Type == 3 && Key != 8 && (Key < 48 || Key > 57))		//if Type is STUDENT, only receives numeric values
				continue;

			if (!isalnum((char)Key) && Key != (int) '_' && Key != 8)			//only alllow alphanum and _
				continue;

			if (Key == 8)				//if backspace is found
			{
				if (NewUser->Username != "")	//if username string is not empty
				{
					NewUser->Username.pop_back();	//pop out one character
					cout << (char)Key;				//this only moves the cursor back one character, not deletes the current character
					cout << ' ';					//so this one does the job, but it moves the cursor to the right again
					cout << (char)Key;				//so this one moves the cursor back again
				}
			}
			else
			{
				cout << (char)Key;		//if things all good, echo the input
				NewUser->Username += (char)Key;	//and add it into the string Username
			}
			if ((ShowFlag == false) && (List->CreateAuth().find("|" + NewUser->Username + "|") != -1))	//this check if the username is already taken
			{
				ShowFlag = true;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);	//get the current cursor pos
				GoToXY(x, y + 10);		//jump down several lines of code
				SetColor(12);
				cout << "That username is already taken";
				SetColor(7);
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);	//get back to where it was previously
			}

			if (ShowFlag == true && (List->CreateAuth().find("|" + NewUser->Username + "|") == -1))
			{
				ShowFlag = false;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);
				GoToXY(x, y + 10);
				cout << "                                  ";	//if nothing happens, print this
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
			}
		}
		ShowFlag = false;
		GoToXY(x, y + 14);
		cout << "                                                    ";
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------




		//-------------------------------PASSWORD-------------------------------------
		//----------------------------------------------------------------------------
		//this one is pretty clear
		GoToXY(x, y + 14);
		SetColor(10);
		cout << "Only alphanumberic characters and \"_\" is allowed";
		SetColor(7);
		GoToXY(x + 15, y + 2);
		while (true)
		{
			Key = _getch();

			if (Key == 224)
			{
				Key = _getch();
				continue;
			}

			if (Key == 13)
				if (NewUser->Password.length() >= 6)
					break;
				else
					continue;

			if (!isalnum((char)Key) && Key != (int) '_' && Key != 8)		//only alllow alphanum and _
				continue;

			if (Key == 8)
			{
				if (NewUser->Password != "")
				{
					NewUser->Password.pop_back();
					cout << (char)Key;
					cout << ' ';
					cout << (char)Key;
				}
			}
			else
			{
				cout << '*';
				NewUser->Password += (char)Key;
			}

			if (ShowFlag == false && NewUser->Password.length() < 6)		//this check if the password is longer than 6 characters
			{
				ShowFlag = true;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);	//get the current cursor pos
				GoToXY(x, y + 10);		//jump down several lines of code
				SetColor(10);
				cout << "Password must be from 6 characters and up";
				SetColor(7);
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);	//get back to where it was previously
			}

			if (ShowFlag == true && NewUser->Password.length() >= 6)
			{
				ShowFlag = false;
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);
				GoToXY(x, y + 10);
				cout << "                                           ";	//if nothing happens, print this
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
			}
		}
		ShowFlag = false;
		GoToXY(x, y + 14);
		cout << "                                                    ";
		NewUser->Password = Encrypt(NewUser->Password); //pass the value of the new password into the encryption method

														//----------------------------------------------------------------------------
														//----------------------------------------------------------------------------





														//---------------------------------NAME---------------------------------------
														//----------------------------------------------------------------------------
														//this is clear too
		GoToXY(x + 15, y + 3);
		while (true)
		{
			Key = _getch();

			if (Key == 224)
			{
				Key = _getch();
				continue;
			}

			if (Key == 13)
				if (NewUser->Name != "")
					break;
				else
					continue;

			if (Key == 8)
			{
				if (NewUser->Name != "")
				{
					NewUser->Name.pop_back();
					cout << (char)Key;
					cout << ' ';
					cout << (char)Key;
				}
			}
			else
			{
				cout << (char)Key;
				NewUser->Name += (char)Key;
			}
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------





		//-----------------------------------DOB--------------------------------------
		//----------------------------------------------------------------------------
		GoToXY(x + 15, y + 4);
		SetColor(0 + 7 * 16); //highlight mode on
		cout << NewUser->Day; //highlight the Day first
		SetColor(7);
		cout << '/' << NewUser->Month << '/' << NewUser->Year;	//print Month and Year in normal mode

		while (true)
		{
			Key = _getch();
			if (Key == 13 && validDate(NewUser->Day, NewUser->Month, NewUser->Year)) //if day is not valid, cannot [enter]
				break;
			if (Key != 224)
				continue;
			Key = _getch();
			switch (Key)
			{
			case 72:			//Key is uppppp, incrementtttt
				switch (State)
				{
				case 1:		//DAY
					NewUser->Day++;
					if (NewUser->Day > dayTop)	//meet limit -> return to 1
						NewUser->Day = 1;
					break;

				case 2:		//MONTH
					NewUser->Month++;
					if (NewUser->Month > monthTop)
						NewUser->Month = 1;
					break;

				case 3:		//YEAR
					NewUser->Year++;
					break;
				}
				break;

			case 80:			//Key is downnnn, decrementtttt
				switch (State)
				{
				case 1:
					NewUser->Day--;
					if (NewUser->Day < 1)
						NewUser->Day = dayTop;
					break;

				case 2:
					NewUser->Month--;
					if (NewUser->Month < 1)
						NewUser->Month = monthTop;
					break;

				case 3:
					NewUser->Year--;
					break;
				}
				break;

			case 77:		//Key is right, this one change the State, so that it alters between Day, Month, Year
				State++;
				if (State > 3)
					State = 1;
				break;

			case 75:		//Key is left, the same as above
				State--;
				if (State < 1)
					State = 3;
				break;
			}

			//so these below are the phase to highlight or change the current section (Day/Month/Year)
			if (Key == 72 || Key == 80)		//if key is up or down
				switch (State)
				{
				case 1:		//change the Day
					GoToXY(x + 15, y + 4);
					SetColor(0 + 7 * 16);
					if (NewUser->Day < 10)
						cout << ' ';
					cout << NewUser->Day;
					SetColor(7);
					break;

				case 2:		//change the Month
					GoToXY(x + 18, y + 4);
					SetColor(0 + 7 * 16);
					if (NewUser->Month < 10)
						cout << ' ';
					cout << NewUser->Month;
					SetColor(7);
					break;

				case 3:		//change the Year
					GoToXY(x + 21, y + 4);
					SetColor(0 + 7 * 16);
					cout << NewUser->Year;
					SetColor(7);
					break;
				}

			if (Key == 77 || Key == 75) //if key is right or left
			{
				//Reprint the layout in normal fashion first
				GoToXY(x + 15, y + 4);
				if (NewUser->Day < 10)
					cout << ' ';
				cout << NewUser->Day << '/';
				if (NewUser->Month < 10)
					cout << ' ';
				cout << NewUser->Month << '/';
				cout << NewUser->Year;

				//then highlight the new section
				switch (State)
				{
				case 1:
					GoToXY(x + 15, y + 4);
					SetColor(0 + 7 * 16);

					if (NewUser->Day < 10)
						cout << ' ' << NewUser->Day;
					else
						cout << NewUser->Day;
					SetColor(7);
					break;

				case 2:
					GoToXY(x + 18, y + 4);
					SetColor(0 + 7 * 16);

					if (NewUser->Month < 10)
						cout << ' ' << NewUser->Month;
					else
						cout << NewUser->Month;
					SetColor(7);
					break;

				case 3:
					GoToXY(x + 21, y + 4);
					SetColor(0 + 7 * 16);
					cout << NewUser->Year;
					SetColor(7);
					break;
				}
			}

			//the process below is like the check for taken Username above
			if (!validDate(NewUser->Day, NewUser->Month, NewUser->Year))		//this check for valid date
			{
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);
				GoToXY(x, y + 10);
				cout << "Invalid DOB";
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
			}
			else
			{
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurrentPos);
				GoToXY(x, y + 10);
				cout << "            ";
				GoToXY(CurrentPos.dwCursorPosition.X, CurrentPos.dwCursorPosition.Y);
			}
		}
		//If the user has chosen a date, then print it out with no more highlight
		GoToXY(x + 15, y + 4);
		if (NewUser->Day < 10)
			cout << ' ';
		cout << NewUser->Day << '/';
		if (NewUser->Month < 10)
			cout << ' ';
		cout << NewUser->Month << '/' << NewUser->Year;
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------





		//--------------------------------ADDRESS-------------------------------------
		//----------------------------------------------------------------------------
		//this one is clear
		GoToXY(x + 15, y + 5);
		while (true)
		{
			Key = _getch();

			if (Key == 224)
			{
				Key = _getch();
				continue;
			}

			if (Key == 13)				//not obligatory
				break;

			if (Key == 8)
			{
				if (NewUser->Address != "")
				{
					NewUser->Address.pop_back();
					cout << (char)Key;
					cout << ' ';
					cout << (char)Key;
				}
			}
			else
			{
				cout << (char)Key;
				NewUser->Address += (char)Key;
			}
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------





		//----------------------------------SEX---------------------------------------
		//----------------------------------------------------------------------------
		//clear too
		GoToXY(x + 15, y + 6);
		while (true)
		{
			Key = _getch();

			if (Key == 224)
			{
				Key = _getch();
				continue;
			}

			if (Key == 13)
				break;				//not obligatory

			if (Key == 8)
			{
				if (NewUser->Sex != "")
				{
					NewUser->Sex.pop_back();
					cout << (char)Key;
					cout << ' ';
					cout << (char)Key;
				}
			}
			else
			{
				cout << (char)Key;
				NewUser->Sex += (char)Key;
			}
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------





		//---------------------------------SUBJECT------------------------------------
		//------------------------------(TEACHER ONLY)--------------------------------
		//if Type is ADMIN or STUDENT this one will be skipped
		if (NewUser->Type == 2) //If == teacher
		{
			GoToXY(x + 15, y + 7);
			while (true)
			{
				Key = _getch();

				if (Key == 224)
				{
					Key = _getch();
					continue;
				}

				if (Key == 13)
					if (NewUser->Subject != "|")
						break;
					else
						continue;
				if (Key == 32 && NewUser->Subject[NewUser->Subject.length() - 1] == 124)
				{
					continue;
				}

				if (Key == 8)
				{
					if (NewUser->Subject != "|")
					{
						NewUser->Subject.pop_back();
						cout << (char)Key;
						cout << ' ';
						cout << (char)Key;
					}
				}
				else
				{
					cout << (char)Key;
					if (Key == 44)		//note this, if ',' is met, Key will be changed to '|' to sastify the Subject format
						Key = 124;
					NewUser->Subject += toupper((char)Key);
				}
			}
		}
		if (NewUser->Subject != "|") //if there is at least one subject
			NewUser->Subject += '|';	//add the right bound, so that |sub|sub| is satisfied
										//----------------------------------------------------------------------------
										//----------------------------------------------------------------------------





										//--------------------------------STATUS--------------------------------------
										//----------------------------------------------------------------------------

		GoToXY(x + 15, y + 8);
		cout << '<';
		SetColor(0 + 7 * 16);
		cout << "YES";
		SetColor(7);
		cout << "> <NO>";

		while (true)
		{
			Key = _getch();
			if (Key == 13)
				break;
			if (Key != 224)
				continue;

			Key = _getch();
			if (Key == 72 || Key == 80)
				continue;

			GoToXY(x + 15, y + 8);
			cout << "<YES> <NO>";
			SetColor(0 + 7 * 16);

			switch (Key)
			{
			case 77:
				if (NewUser->Status)
					NewUser->Status = false;
				else
					NewUser->Status = true;
				break;

			case 75:
				if (!NewUser->Status)
					NewUser->Status = true;
				else
					NewUser->Status = false;
				break;
			}
			switch (NewUser->Status)
			{
			case true: GoToXY(x + 16, y + 8); cout << "YES"; break;
			case false: GoToXY(x + 22, y + 8); cout << "NO"; break;
			}
			SetColor(7);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		List->CreateUserDescending(NewUser);
	}
	else
		cout << "Cannot allocate memory\n";
}

//Compare 2 strings alphabetically
int CmpStringAlpha(string compared, string comparing)
{
	int l = compared.length();
	int lIng = comparing.length();

	if (compared == "" || comparing == "")
		return -2;			//cannot compare

	for (int i = 0;; i++)
	{
		if (tolower(compared[i]) < tolower(comparing[i]))
			return 1;				//"_" < number < alpha

		if (tolower(compared[i]) > tolower(comparing[i]))
			return -1;

		if (i == l - 1 && i == lIng - 1)
			return 0;				//equal
		if (i == l - 1)
			return 1;				//come earlier
		if (i == lIng - 1)
			return -1;
	}
}
//=======================================================================


//=======================SUPPORTING FUNCTIONS============================

//Check valid date
bool validDate(int day, int month, int year) //check if DOB is legit
{
	int days = daysInMonth(month, year);
	int curDay, curMonth, curYear;

	currentDay(curDay, curMonth, curYear);

	if (curYear - year < 0 || curYear - year >= curYear || month < 1 || month > 12)
		return 0;

	if (day < 1 || day > days)
		return 0;

	if ((curYear - year == 0 && curMonth < month) || (curYear - year == 0 && month == curMonth && curDay < day))
		return 0;

	return 1;
}
bool leapYear(const int &year) //check leap year
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1;

	return 0;
}
int daysInMonth(const int &month, const int &year) //find days in a specific month
{
	int leap = leapYear(year);
	int days;

	if (month == 2)
	{
		if (leap == 1)
			days = 29;
		else
			days = 28;
	}
	else if (month == 1 || month == 3 || month == 5 || month == 7
		|| month == 8 || month == 10 || month == 12)

		days = 31;
	else
		days = 30;

	return days;
}
void currentDay(int &day, int &month, int &year) //get the current date in computer
{
	time_t t = time(0);
	struct tm * now = localtime(&t);
	year = (now->tm_year + 1900);		//current year
	month = (now->tm_mon + 1);			//current month
	day = (now->tm_mday);				//current day
}

//=======================================================================