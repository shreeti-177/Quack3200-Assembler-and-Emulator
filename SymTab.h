//
//		SymbolTable class - Records the symbols and their locations.
//
#pragma once

// This class is the symbol table.
class SymbolTable {

public:
  
	SymbolTable() {};
	~SymbolTable( ) {};

	const int multiplyDefinedSymbol = -999;

    // Adds a new symbol to the symbol table.
	void AddSymbol(string &a_symbol, int a_loc);

    // Displays the symbol table.
	void DisplaySymbolTable();

    // Looks up a symbol in the symbol table.
	bool LookupSymbol(string &a_symbol, int &a_loc);
		
private:

    // This is the actual symbol table. The symbol is the key to the map.
    map<string, int> m_symbolTable;

};
