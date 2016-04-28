/*---Using Localtime---*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma once
#include <string>
using namespace std;

struct User_Info
{
	User_Info();
	string Subject;     //"|" if student and admin, |sub1|sub2|...|subn| if teacher
	string Username;	//only ALPHANUMERIC and "_"(if the user is student, username must be student id)
	string Password;    //only ALPHANUMERIC and "_", must contain at least 6 characters
	string Name;
	int Day;
	int Month;
	int Year;
	string Address;     //not obligatory
	string Sex;         //not obligatory
	int Type;			//Admin: 1 / Teacher: 2 / Student: 3
	bool Status;        //(activated: true / deactivated: false)
	User_Info *Next;
	User_Info *Prev;

	//show a specific User
	void showAUser();
};

class User_Info_List
{
public:
	User_Info_List();
	~User_Info_List();

	//Admin operations only
	void Remove(User_Info *DeleteUser);
	friend void ViewAccount(User_Info_List *&List, int ViewMode, User_Info *Current_Admin);	//Filter view, which includes removing, banning and and resetting password
	void CreateUser(User_Info* NewUser);							//put a User on top of the list
	void CreateUserDescending(User_Info* NewUser);					//put a User according to alphabetic order
	void ExportXML(string filename);								//export an XML													
	
	string CreateAuth();
	User_Info **GetHead();								//Return the Address of Head
private:
	string Auth;
	User_Info *Head;
};

//check valid date
bool validDate(int day, int month, int year);
bool leapYear(const int &year);
int daysInMonth(const int &month, const int &year);
void currentDay(int &day, int &month, int &year);


												//compare two string alphabetically (for sorting)
int CmpStringAlpha(string str1, string str2);

//create a user UI
void CreateUserUI(User_Info_List *&List);        //the UI for creating a User


void ModAUserUI(User_Info *&User);               //the UI for modding info of a user
