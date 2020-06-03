#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "Instruction.h"

/**/
/*
NAME

	ParseInstruction - Parses the Instruction and identifies its type.

SYNOPSIS

	InstructionType ParseInstruction(string a_buff);

DESCRIPTION

	This function will detect any comments or end statement and
	parse the instruction. It will determine the type of a given instruction.

RETURNS
	
	Returns either of the four values: "ST_Comment", "ST_End", "ST_MachineLanguage" or "ST_AssemblerInstr"
	for "m_type".

*/
/**/

Instruction::InstructionType Instruction::ParseInstruction(string a_buff)
{
	m_instruction = a_buff; //Make a copy of the original instruction

	//If a line starts with a semicolon, return the type as Comment
	if (a_buff[0] == ';') 
	{
		return m_type= ST_Comment;
	}
	
	//Get rid of everything after the semicolon in a line (get rid of comments, if any)
	size_t semi = a_buff.find(';');
	if (semi != string::npos) 
	{
		a_buff=a_buff.substr(0, semi);
	}

	//Make sure all strings within the statement are low-case to comply with case sensitivity
	ToLower(a_buff); 
		
	//If "end" is found within the line, return statement type as End
	size_t end_state = a_buff.find("end");
	if (end_state!=string::npos) 
	{
		return m_type= ST_End;
	}

	// Function to parse machine and assembly instruction into basic elements: label(L), op code(O), register (R), operand (O)
	GetElements(a_buff);

	//Function to get derived values of registers and operands.
	GetDerivedVal();

	//Check for assembly language instructions and return statement type as Assembly Language
	if (a_buff.find("org") != string::npos || a_buff.find("ds") != string::npos || a_buff.find("dc") != string::npos) 
	{
		m_NumOpCode = 0;
		m_NumRegister = 0;
		m_type = ST_AssemblerInstr;
	}
	else
		m_type = ST_MachineLanguage; //Return statement type as Machine Language after all possibilities are exhausted
			
	//Return whether a statement is Machine Language or Assembly Language
	return m_type;

}/*Instruction::InstructionType Instruction::ParseInstruction(string a_buff);*/


/**/
/*
NAME

	GetElements - Parses the Instruction into basic elements.

SYNOPSIS

	void GetElements(string &a_buff);

DESCRIPTION

	This function will parse the instructions into label, op code, register and operands.
	It will also derive numeric values for registers and operands, if they exist.

*/
/**/

void Instruction::GetElements(string &a_buff) 
{
	//Initialize all elements to NULL
	m_Label = m_OpCode = m_Register = m_Operand = ""; 

	string s1, s2, s3, s4, s5;

	//Parsing when there's a register in the instruction.
	if (a_buff.find(',') != string::npos)
	{
		//Replace commas with spaces to identify registers, if any.
		replace(a_buff.begin(), a_buff.end(), ',', ' ');

		istringstream input(a_buff);
		input >> s1 >> s2 >> s3 >> s4 >> s5;

		//To identify a label in a statement
		if (a_buff[0] != ' '&& a_buff[0] != '\t')
		{
			m_Label = s1;
			m_OpCode = s2;
			m_Register = s3;
			m_Operand = s4;
			
			//if statement has more than the elements specified, report an error
			if (s5 != "")
				Errors::RecordError("Statement contains more than a label, OpCode, Register and Operand");
			return;
		}
		m_OpCode = s1;
		m_Register = s2;
		m_Operand = s3;
		//if statement has more than the elements specified, report an error
		if (s4 != "")
			Errors::RecordError("Statement contains more than a label, OpCode, Register and Operand");
		return;
	}

	//Parsing an instruction without a specified register.
	istringstream input(a_buff);
	input >> s1 >> s2 >> s3 >> s4;

	//To identify a label in a statement
	if (a_buff[0] != ' '&& a_buff[0] != '\t')
	{
		m_Label = s1;
		m_OpCode = s2;
		m_Operand = s3;
		//if statement has more than the elements specified, report an error
		if (s4 != "")
			Errors::RecordError("Statement contains more than a label, OpCode, Register and Operand");
		return;
	}
	m_OpCode = s1;
	m_Operand = s2;
	
	//if statement has more than the elements specified, report an error
	if (s3 != "")
		Errors::RecordError("Statement contains more than a label, OpCode, Register and Operand");
	return;

}/*void Instruction::GetElements(string &a_buff); */


/**/
/*
NAME

	GetDerivedVal - Derives values of registers and operands.

SYNOPSIS

	void GetDerivedVal( );

DESCRIPTION

	This function will compute the numeric values for registers and operands, if they exist.

*/
/**/

void Instruction::GetDerivedVal()
{
	if (!m_Register.empty()) 
	{
		m_NumRegister = stoi(m_Register);
	}
	else
		m_NumRegister = 9;

	string::const_iterator it = m_Operand.begin();
	while (it != m_Operand.end() && (isdigit(*it))) 
	{
		it++;
	}
	if (!m_Operand.empty() && it == m_Operand.end()) 
	{
		m_IsNumericOperand = true;
		m_OperandValue = stoi(m_Operand);   // The value of the operand if it is numeric.
	}
	else 
		m_IsNumericOperand = false;

}/*void Instruction::GetDerivedVal( ); */


/**/
/*
NAME

	LocationNextInstruction - Computes the location of the next instruction.

SYNOPSIS

	int LocationNextInstruction(int a_loc);

DESCRIPTION

	This function will locate the address of the next instruction to be executed.

*/
/**/

int Instruction::LocationNextInstruction(int a_loc) 
{
	if ((m_instruction.find("ds")!=string::npos)||(m_instruction.find("org") != string::npos))
		a_loc = a_loc + m_OperandValue;
	else
		a_loc++;
	return a_loc;
}/*int Instruction::LocationNextInstruction(int a_loc);*/


/**/
/*
NAME

	ValidLabel - Checks if a given label is valid.

SYNOPSIS

	bool ValidLabel(string m_Label);

DESCRIPTION

	This function makes sure "m_Label" is valid.

RETURNS
	
	Returns true if label name starts with an alphabet and the name size lies within 0-10.
	Else, returns false.

*/
/**/

bool Instruction::ValidLabel(string m_Label) 
{
		if (!isalpha(m_Label[0]) || m_Label.size() < 1 || m_Label.size() > 10)
			return false;
		return true;
}/*bool Instruction::ValidLabel(string m_Label); */


/**/
/*
NAME

	ValidOperand - Checks if a given operand is valid.

SYNOPSIS

	bool ValidOperand( );

DESCRIPTION

	This function makes sure a string operand has a valid operand name.

RETURNS

	Returns true if operand name starts with an alphabet and the name size lies within 1-10.
	Else, returns false.

*/
/**/

bool Instruction::ValidOperand()
{
	if (m_IsNumericOperand)
		return true;
	else if (isalpha(m_Operand[0]) && m_Operand.size() >= 1 && m_Operand.size() <= 10)
		return true;
	else
		return false;
}/*bool Instruction::ValidOperand();*/


/**/
/*
NAME

	OpCodeLookup - Looks for a valid OpCode and derives the numeric value.

SYNOPSIS

	bool OpCodeLookup( );

DESCRIPTION

	This function checks for a valid opcode by matching its numeric value with numbers from 1-13.

RETURNS

	Returns true if the numeric value lies within 1-13.
	Else, returns false.

*/
/**/

bool Instruction::OpCodeLookup()
{
	ToLower(m_OpCode);
	map<string, int> op_code = {
		{"add", 1},
		{"sub",2},
		{"mult",3},
		{"div",4},
		{"load",5},
		{"store",6},
		{"read",7},
		{"write",8},
		{"b" ,9},
		{"bm", 10},
		{"bz", 11},
		{"bp", 12},
		{"halt", 13}
	};
	m_NumOpCode = op_code[m_OpCode];
	if (m_NumOpCode >= 1 && m_NumOpCode <= 13)
		return true;
	return false;
}/*bool Instruction::OpCodeLookup()*/


/**/
/*
NAME

	GetTranslation - Translates an instruction to its machine language equivalent.

SYNOPSIS

	string GetTranslation(string &a_buff, int &a_loc);

DESCRIPTION

	This function translates the instruction to machine language code with a 
	2 digit OpCode, 1 digit register and a 5 digit address portion.

RETURNS

	Returns the translation appended as a string.

*/
/**/
string Instruction::GetTranslation(string &a_buff, int &a_loc) 
{
	ToLower(a_buff);

	//Padding the string with leading zero in case of a 1-digit opcode
	string op = to_string(m_NumOpCode);
	PadZero(op, 2);

	string reg = to_string(m_NumRegister);

	int con;
	SymbolTable sym;

	//Assign the constant value to the address portion
	if (a_buff.find("dc") != string::npos)
		con = m_OperandValue;
	else
		con = a_loc;
	
	//Padding the string with leading 0s in case of < 5 bit address portion.
	string locate = to_string(con);
	PadZero(locate, 5);

	//Append the translation as a string
	return (op + reg + locate);
}/*string Instruction::GetTranslation(string &a_buff, int &a_loc);*/


/**/
/*
NAME

	ToLower - Converts a given string to lowercase

SYNOPSIS

	string ToLower(string &a_buff);

DESCRIPTION

	This function converts all alphabets of a string to lowercase to comply with case sensitivity.

RETURNS

	Returns the lowercase string.

*/
/**/
string Instruction::ToLower(string &a_buff) 
{
	int len = a_buff.size();
	for (int i = 0; i < len; i++)
	{
		a_buff[i] = tolower(a_buff[i]);
	}
	return a_buff;
}/*string Instruction::ToLower(string &a_buff); */


/**/
/*
NAME

	PadZero - Pads a string with leading 0s.

SYNOPSIS

	void PadZero(string &a_buff, int a_num);

DESCRIPTION

	This function pads the 5 digit address portion and the 2 digit opcode with leading Os, where needed.

*/
/**/
void Instruction::PadZero(string &a_buff, int a_num) {
	stringstream s1;
	s1 << setw(a_num) << setfill('0') << a_buff;
	a_buff = s1.str();
}/*void Instruction::PadZero(string &a_buff, int a_num);*/