//
// Error class - Manages error reporting. Note: all members are static so we can access them anywhere.
//

#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>

class Errors {

public:
    Errors(){}
	~Errors() {}

    // Initializes error reports.
	static void InitErrorReporting();

    // Records an error message.
	static void RecordError(const string a_emsg);

	//Checks for collected error messages. 
	inline static bool WasThereErrors() 
	{ 
		return (!m_ErrorMsgs.empty()); 
	}

    // Displays the collected error message.
    static void DisplayErrors( );

private:

    static vector<string> m_ErrorMsgs;		//To store collected error messages.
};
#endif