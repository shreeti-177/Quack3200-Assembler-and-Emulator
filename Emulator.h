//
//		Emulator class - Supports the emulation of Quack3200 programs.
//

#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:
	
	// The size of the memory of the Quack3200.
	const static int MEMSZ = 100000;	
    
	// The constructor sets specified bytes of memory pointed to by m_memory and m_reg to 0.
	emulator() 
	{
		memset(m_memory, 0, MEMSZ * sizeof(int));
		memset(m_reg, 0, 10 * sizeof(int));
    }

    // Records instructions and data into Quack3200 memory.
	bool insertMemory(int a_location, int a_contents); 
    
    // Runs the Quack3200 program recorded in memory.
	bool runProgram();

private:

    int m_memory[MEMSZ];       // The memory of the Quack3200.
    int m_reg[10];			   // The accumulator for the Quack3200.
};

#endif

