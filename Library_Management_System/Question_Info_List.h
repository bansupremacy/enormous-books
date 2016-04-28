#pragma once
#include "User_Info_List.h"
#include <string>
using namespace std;

struct Question_Info
{
	Question_Info();
	int ID;					/*---ID for Question in the linked list---*/
	string Subject;			/*---Always in Uppercase---*/
	string Author;			/*---Always in Uppercase---*/
	bool Viewable;			/*---Viewable or not---For student to view---*/
	bool isSingle;			/*---Multi answer or single answer---*/
	string Question;		
	string Options;			/*---Format: |A. abc|B. cde|C. ghi|---*/
	string Answer;			/*---Format: ACDE---*/
	Question_Info *Next;
	Question_Info *Prev;
};

class Question_Info_List
{
public:
	Question_Info_List();																/*---Constructor---Loading Data---*/
	~Question_Info_List();																/*---Destructor---Saving Data---*/
	void ViewQuestions(User_Info *&Current_User);										/*---Can search the question by key words, authors.---*/
	void ViewQuestions(string _Subject, string _Author, int Show_Answer, int Line);		/*---View Question with Filters at the line (Line)---*/
																						/*---Show_Answer:----------------------------*/
																						/*-------0: show answer of ALL question------*/
																						/*----(-1): show answer of NO question-------*/
																						/*-------n: show answer of question ID = n---*/
	void Create_Custom_List(User_Info *&Current_User);									/*---Create a List of Question for the Current_User---*/
	void Alter();																		/*---Altering a question---*/
	void Insert();																		/*---Insert a new question---*/
	void Delete();																		/*---Delete a question*/
	void Search();																		/*---Searching questions by KEYWORD, SUBJECT, AUTHOR---*/
	void ModSubject(User_Info *&Current_User);	//Modify your 
	Question_Info **GetHead();															/*---Return the Address of Head---*/
private:
	Question_Info *Head;																/*---Question Linked List---*/
	Question_Info *Custom_Question;														/*---Custom questions for the Current User logged in---*/
};
