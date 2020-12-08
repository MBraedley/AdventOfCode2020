#include "HandheldConsole.h"

#include <fstream>

HandheldConsole::HandheldConsole(const std::vector<Instruction>& program) :
	m_Program(program)
{
}

const std::vector<Instruction> HandheldConsole::ReadProgram(std::string filename)
{
	return std::vector<Instruction>();
}

void HandheldConsole::Run()
{
}

void HandheldConsole::RunNextInstruction()
{
}
