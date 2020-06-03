//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] ) : m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}


/**/
/*
NAME

	PassI - Establishes the location of the symbols.

SYNOPSIS

	void PassI( );

DESCRIPTION

	This function will get the addresses for all the labels. It will successively read
	lines from the source file, detect comments and end statement and parse the remaining
	instruction to labels, operands, registers and operation codes. It'll then add a label 
	and its location to the symbol table and locate the address of the next instruction.

*/
/**/

void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; )
	{

        // Read the next line from the source file.
        string line; 

        if( ! m_facc.GetNextLine( line ) ) 
		{
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
       
		// Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );
       
		// If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) 
			return;
		
        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) 
        {
        	continue;
		}
        
		// If the instruction has a label, record it and its location in the symbol table.
        if( m_inst.isLabel( ) ) 
		{
			m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction( loc );
    }
}/*void Assembler::PassI( ); */


/**/
/*
NAME

	PassII - Generates a translation of an instruction to its machine language equivalent

SYNOPSIS

	void PassII( );

DESCRIPTION

	This function will successively re-read	lines from the source file, 
	parse each instruction to labels, operands, registers and operation codes.
	It'll then generate a translation for each instruction and display 
	the original instruction along with its machine language equivalent.
	It'll also record and display errors encountered and then, get the location of
	the next instruction to be executed.
*/
/**/

void Assembler::PassII( )
{
	//Initialize the error reporting
	Errors::InitErrorReporting();

	// Rewind to the beginning of the source file
	m_facc.rewind();

	cout << "_______________________________________________________" << endl;
	system("pause");
	cout << setw(10) << '\t' << left << "Translation of the Program: " << endl;
	cout << setw(12)<<right<<"Location" <<setw(12)<< '\t' << left<< "Contents" << setw(10) << '\t' << left<< "Original Statement" << endl;

	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (; ; )
	{
		string line;   // Read the next line from the source file.

		// If there are no more lines, we are missing an end statement.
		if (!m_facc.GetNextLine(line))
		{
			Errors::RecordError("Missing end statement.");
			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// Make sure the end statement is the last statement in the program.
		//If not, report an error.
		if (st == Instruction::ST_End)
		{
			if (m_facc.GetNextLine(line))
				Errors::RecordError("There must be no statement after the end statement.");
			cout << setw(34) << '\t'<< left << line << endl;
			return;
		}

		//Skip the translation if a statement is either a comment or an end statement.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr)
		{
			cout << setw(45) << '\t'<< left << line << endl;
			continue;
		}

		//Report error if a LABEL is invalid.
		if (m_inst.isLabel() && !m_inst.ValidLabel(m_inst.GetLabel()))
		{
			Errors::RecordError("Invalid Label. Labels must start with an alphabet and can only be upto 10 characters in length");
		}

		//Report error if an OPCODE within a machine language instruction is illegal.
		if (st == Instruction::ST_MachineLanguage)
		{
			if (m_inst.isOpCode() && !m_inst.OpCodeLookup()) {
				Errors::RecordError("Illegal OpCode");
			}
		}

		//Report error if an OPERAND is invalid.
		if (m_inst.isOperand() && (!m_inst.ValidOperand()))
		{
			Errors::RecordError("Invalid Operand. Symbolic operands must start with a letter and can only be upto 10 characters in length.");
		}

		//Lookup a symbol in the symbol table. Report an error if it does not exist.
		if (m_inst.isLabel() && !m_symtab.LookupSymbol(m_inst.GetLabel(), loc))
		{
			Errors::RecordError("Symbol not found in the Symbol Table.");
		}

		// Isntructions with "ORG" and "DS" just point to a start location or set aside storage.
		//No translation required for these statements.
		if (line.find("org") != string::npos || line.find("ORG") != string::npos || line.find("ds") != string::npos || line.find("DS") != string::npos) {
			cout <<'\t'<< left << loc << setw(34)<< '\t' << left << line << endl;
		}
		else {
			int a_locate;		//Stores the address bit portion of the translated machine code

			//Check the location of an operand within the symbol table.
			m_symtab.LookupSymbol(m_inst.GetOperand(), a_locate);

			//Get the contents after translating a given instruction to its machine equivalent.
			string contents = m_inst.GetTranslation(line, a_locate);

			cout << '\t' << left << loc << setw(10)<<'\t' << left << contents << setw(10) << '\t' << left << line << endl;

			//Buffer that stores location and translation to be used by the emulator.
			m_buff.insert(pair<int, int>(loc, stoi(contents)));
		}

		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
	
}/*void Assembler::PassII( );*/

/**/
/*
NAME

	RunProgramInEmulator -  Runs emulator on the translation.

SYNOPSIS

	void RunProgramInEmulator();

DESCRIPTION

	This function will display the collected errors, if any.
	If no errors are present, it will insert data and instructions 
	into memory and run the program recorded into memory.
	It will also record any errors collected during emulation.

*/
/**/
void Assembler::RunProgramInEmulator()
{
	cout << "_______________________________________________________" << endl;
	
	//Display all collected errors from Pass II, if any.
	//If errors are present, emulator is unable to run.
	if (Errors::WasThereErrors())
	{
		Errors::DisplayErrors();
		cout << "Errors during compilation. Please fix the listed errors and try again." << endl;
		cout << "Exiting emulation..." << endl;
		exit(1);
	}
	

	system("pause");

	Errors::InitErrorReporting();

	//Record errors that may be collected while inserting instruction into memory.
	map<int, int>::iterator it;
	for (it = m_buff.begin(); it != m_buff.end(); ++it) 
	{
		if (!m_emul.insertMemory(it->first, it->second)) {
			Errors::RecordError("Error inserting into emulator memory. Outside Memory Range");
			Errors::DisplayErrors();
			exit(1);
		}
	}

	cout << "Results from emulating program" << endl;

	//Record errors collected while running the program.
	if (!m_emul.runProgram())
	{
		Errors::RecordError("Error running the emulator");
		Errors::DisplayErrors();
		exit(1);
	}
	
}/*void Assembler::RunProgramInEmulator();*/