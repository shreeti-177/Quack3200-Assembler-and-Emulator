//
//	Implementation of Error Class.

#include "stdafx.h"
#include"Errors.h"

//Accessing the static members.
vector <string> Errors::m_ErrorMsgs;

/**/
/*
NAME

	InitErrorReporting - Initializes error reports.

SYNOPSIS

	void InitErrorReporting( ); 

DESCRIPTION

	This function will clear all the errors previously recorded and set "m_WasErrorMessages" to false.

*/
/**/

void Errors::InitErrorReporting() 
{
	m_ErrorMsgs.clear();
}/*void Errors::InitErrorReporting( );*/


/**/
/*
NAME

	RecordError - Records an error message.

SYNOPSIS

	void RecordError(const string a_emsg); 

DESCRIPTION

	This function will add error message "a_emsg" to the vector "m_ErrorMsgs" 
	and set "m_WasErrorMessages" to true to indicate that there are errors collected.

*/
/**/
void Errors::RecordError(const string a_emsg) 
{
	m_ErrorMsgs.push_back(a_emsg);
}/*void Errors::RecordError(const string a_emsg); */


/**/
/*
NAME

	DisplayErrors - Displays the collected error message.

SYNOPSIS

	void DisplayErrors( );

DESCRIPTION

	This function will display all the errors collected within the vector "m_ErrorMsgs"
	and then clear those messages from the vector.

*/
/**/
void Errors::DisplayErrors() 
{
	int i = 1;
	cout << endl;
	for (auto it = m_ErrorMsgs.begin(); it != m_ErrorMsgs.end(); it++) 
	{
		cout <<"!!ERROR "<<i<<" : "<< *it << endl;
		i++;
	}
	m_ErrorMsgs.clear();
}/*void Errors::DisplayErrors( ); */
