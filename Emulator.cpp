//
//	Implementation of the Emulator class.
//

#include "stdafx.h"
#include "Errors.h"
#include "Emulator.h"


/**/
/*
NAME

	insertMemory - Records instructions and data into Quack3200 memory.

SYNOPSIS

	bool insertMemory(int a_location, int a_contents);

DESCRIPTION

	This function will check if the specified location is within the memory range for the 
	Quack 3200 computer. If so, it'll store "a_contents" into the location "a_location" within memory.

RETURNS
	
	Returns true if the location is within the memory range. Else, it returns false.

*/
/**/

bool emulator::insertMemory(int a_location, int a_contents)
{
	if (a_location < 0 && a_location >= MEMSZ) {
		return false;
	}
	 m_memory[a_location] = a_contents;
	 return true;
}/*bool emulator::insertMemory(int a_location, int a_contents); */


 /**/
/*
NAME

	runProgram - Runs the Quack3200 program recorded in memory.

SYNOPSIS

	bool runProgram( );

DESCRIPTION

	This function will break down the machine language instruction into 
	a 2 digit opcode, a 1 digit register and a 5 digit address portion. 
	The first instruction is at location 100 and then, it will execute 
	a sequence of instructions stored in successive memory locations.
	It will check for one of the 13 opcodes within the instruction
	and	perform the operation specified for that opcode. 

RETURNS

	Returns true if the emulation terminates successfully. Else, it returns false.

*/
/**/

bool emulator::runProgram() 
{
	int loc = 100;	//Assume the first instruction starts at location 100.
	
	while(true) {
		int contents = m_memory[loc];		//Sample Instruction:01455555
		int opcode = contents / 1000000;	//extracting the opcode:01
		int reg = (contents / 100000) % 10;	//extracting the register:4
		int address = (contents % 100000);	//extracting the address:55555

		//13 cases for 13 opcodes
		switch (opcode) {
		case 1:
			m_reg[reg] += m_memory[address];
			loc += 1;
			continue;
		case 2:
			m_reg[reg] -= m_memory[address];
			loc += 1;
			continue;
		case 3:
			m_reg[reg] *= m_memory[address];
			loc += 1;
			continue;
		case 4:
			m_reg[reg] /= m_memory[address];
			loc += 1;
			continue;
		case 5:
			m_reg[reg] = m_memory[address];
			loc += 1;
			continue;
		case 6:
			m_memory[address] = m_reg[reg];
			loc += 1;
			continue;
		case 7:
			cout << "?";
			int input;
			cin >> input;
			m_memory[address] = input;
			loc += 1;
			continue;
		case 8:
			cout << m_memory[address] << endl;
			loc += 1;
			continue;
		case 9:
			loc = address;
			continue;
		case 10:
			if (m_reg[reg] < 0)
				loc = address;
			else
				loc += 1;
			continue;
		case 11:
			if (m_reg[reg] == 0) 
				loc = address;
			else
				loc += 1;
			continue;
		case 12:
			if (m_reg[reg] > 0)
				loc = address;
			else
				loc += 1;
			continue;
		case 13:
			cout << "End of emulation" << endl;
			return true;
		default:
			Errors::RecordError("Illegal OpCode");
			return false;
		}
	}
}/*bool emulator::runProgram();*/
