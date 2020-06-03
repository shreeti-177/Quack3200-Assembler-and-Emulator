//
// Instruction Class - Helps parse and provide information about instructions.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction being processed. 
    enum InstructionType {
        ST_MachineLanguage, 		// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   	// end instruction.
    };

    // Parse the Instruction.
	InstructionType ParseInstruction(string a_buff);

    // Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

    // To access the label.
    inline string &GetLabel( ) 
	{
        return m_Label;
    };

    // To determine if a label is blank.
    inline bool isLabel( ) 
	{
        return !m_Label.empty();
    };

	//To determine if an OpCode is blank.
	inline bool isOpCode() 
	{
		return !m_OpCode.empty();
	}
	
	//To determine if an operand is blank.
	inline bool isOperand() 
	{
		return !m_Operand.empty();
	}
	
	//To access an operand.
	inline string &GetOperand() 
	{
		return m_Operand;
	};

	// Checks if a label is valid.
	bool ValidLabel(string m_Label);

	//Looks for a valid OpCode and derives the numeric value.
	bool OpCodeLookup();

	//Checks if an operand is valid and derives a numeric value, if it exists. 
	bool ValidOperand();

	//Pads the 5 digit address portion or the 2 digit opcode with leading Os.
	void PadZero(string &a_buff, int a_num);
	
	//Translates an instruction to its machine language equivalent.
	string GetTranslation(string &a_buff, int &a_loc);

private:

	//Converts a given line to lowercase strings
	string ToLower(string &a_buff);

	//Function to get label, op code, register and operand
	void GetElements(string &a_buff);

	//Function to get derived values of registers and operands.
	void GetDerivedVal();
		
    // The elements of an instruction
    string m_Label;				// The label.
    string m_Register;			// The register specified.  Use -1 if there is none.
    string m_OpCode;			// The symbolic op code.
    string m_Operand;			// The operand.


    string m_instruction;		// The original instruction.

    // Derived values.
    int m_NumOpCode;			// The numerical value of the op code.
    int m_NumRegister;			// the numeric value for the register.
    InstructionType m_type;		// The type of instruction.

    bool m_IsNumericOperand;	// == true if the operand is numeric.
    int m_OperandValue;			// The value of the operand if it is numeric.

};


