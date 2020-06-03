//
//	Implementation of the SymbolTable class. 
//

#include "stdafx.h"
#include "SymTab.h"
#include<iomanip>

/**/
/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( string &a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
/**/

void SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    map<string, int>::iterator st= m_symbolTable.find( a_symbol );
	
	// If the symbol is already in the symbol table, record it as multiply defined.
	if( st != m_symbolTable.end() ) {
		st->second = multiplyDefinedSymbol;
        return;
    }

    // Record a new symbol and its location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;

}/*void SymbolTable::AddSymbol( string &a_symbol, int a_loc );*/


/**/
/*
NAME

	DisplaySymbolTable - Displays the entire symbol table.

SYNOPSIS

	void DisplaySymbolTable( );

DESCRIPTION

	This function will display all the entries (symbols and their respective locations) 
	of the map "m_symbolTable" where the symbol is the key to the map.
*/
/**/

void SymbolTable::DisplaySymbolTable() 
{
	
	cout << setw(35)<<"SYMBOL TABLE" << endl;
	cout << setw(10) << "Symbol#" << setw(20) << "Symbol" << setw(20) << "Location" << endl;
	
	int i = 0;
	
	for (const auto &p: m_symbolTable){
		std::cout << setw(6) << i << setw(22) << p.first << setw(20) << p.second << endl;
		i++;
	}

}/*void SymbolTable::DisplaySymbolTable();*/


/**/
/*
NAME

	LookupSymbol - Lookup a symbol in the symbol table.

SYNOPSIS

	bool LookupSymbol(string &a_symbol, int &a_loc);

DESCRIPTION

	This function will check whether a given "a_symbol" is stored within the symbol table, and determine
	its location "a_loc".

RETURNS

	Returns true if "a_symbol" is found within the symbol table map m_symbolTable and
	assigns "a_loc" the location of the particular symbol.
	Returns false if "a_symbol" is not found within the symbol table.
*/
/**/

bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc)
{
	map<string, int>::iterator st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {
		a_loc = st->second;
		return true;
	}
	return false;
}/*bool SymbolTable::LookupSymbol(string &a_symbol, int &a_loc);*/
