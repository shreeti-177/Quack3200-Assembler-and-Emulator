//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"


/**/
/*
NAME

	Constructor -  Opens the file.

SYNOPSIS

	FileAccess( int argc, char *argv[] );

DESCRIPTION

	This constructor will open up a source file to read instruction and translate it to machine code.

*/
/**/

FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file. 
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}/*FileAccess::FileAccess( int argc, char *argv[] )*/


/**/
/*
NAME

	Destructor -  Closes the file.

SYNOPSIS

	~FileAccess( );

DESCRIPTION

	This destructor will close a previously opened source file. 

*/
/**/

FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );

}/*FileAccess::~FileAccess( )*/


/**/
/*
NAME

	GetNextLine -  Get the next line from the file.

SYNOPSIS

	bool GetNextLine( string &a_buff );

DESCRIPTION

	This function will get the next line from the source file.

RETURNS
	
	Returns true when it gets the next line successfully. 
	If it reaches the end of file, it returns false.

*/
/**/

bool FileAccess::GetNextLine( string &a_buff )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_buff );
    
    // Return indicating success.
    return true;
}/*bool FileAccess::GetNextLine( string &a_buff )*/


/**/
/*
NAME

	rewind -  Rewind to the beginning of the file.

SYNOPSIS

	void rewind();

DESCRIPTION

	This function will clear all file flags and put the file pointer back to the beginning of the file.

*/
/**/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}/*void FileAccess::rewind( )*/
