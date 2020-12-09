#include "HandheldConsole.h"

#include <iostream>
#include <fstream>
#include <cassert>

HandheldConsole::HandheldConsole(const std::vector<Instruction>& program) :
	m_Program(program)
{
}

const std::vector<Instruction> HandheldConsole::ReadProgram(std::string filename)
{
	std::ifstream fileIn(filename);
	std::vector<Instruction> program;

	std::string operation;
	std::int64_t value;
	while (fileIn >> operation >> value)
	{
		Operation op;
		if (operation == "nop")
		{
			op = Operation::NOP;
		}
		else if (operation == "acc")
		{
			op = Operation::ACC;
		}
		else if (operation == "jmp")
		{
			op = Operation::JMP;
		}
		else
		{
			assert(false);
		}
		program.emplace_back(op, value);
	}

	return program;
}

bool HandheldConsole::Run()
{
	if (m_HaltOnLoop)
	{
		m_LoopDetector.insert(m_IP);
	}
	while (!m_Halted)
	{
		RunNextInstruction();
	}

	return m_NormalHalt;
}

void HandheldConsole::Debug()
{
	m_HaltOnLoop = true;
	while (!m_Halted)
	{
		if (m_IP != m_Program.size())
		{
			Instruction instruct = m_Program[m_IP];
			if (instruct.op != Operation::ACC)
			{
				std::string opName;
				switch (instruct.op)
				{
				case Operation::ACC:
					opName = "ACC";
					break;
				case Operation::NOP:
					opName = "NOP";
					break;
				case Operation::JMP:
					opName = "JMP";
					break;
				default:
					break;
				}
				std::cout << m_IP << "\t" << opName << "\t" << instruct.value << "\t| ";
			}
			RunNextInstruction();

			if (instruct.op != Operation::ACC)
				std::cout << m_Acc << std::endl;
		}
		else
		{
			std::cout << "Halted normally" << std::endl;
			return;
		}
	}

	std::cout << "Loop detected, exiting" << std::endl;
}

void HandheldConsole::RunNextInstruction()
{
	if (m_IP == m_Program.size())
	{
		m_Halted = true;
		m_NormalHalt = true;
		return;
	}

	Instruction instruct = m_Program[m_IP];
	switch (instruct.op)
	{
	case Operation::ACC:
		m_Acc += instruct.value;
		m_IP++;
		break;
	case Operation::NOP:
		m_IP++;
		break;
	case Operation::JMP:
		m_IP += instruct.value;
		break;
	default:
		m_Halted = true;
		break;
	}

	if (m_HaltOnLoop)
	{
		auto [iter, inserted] = m_LoopDetector.insert(m_IP);
		m_Halted = !inserted;
	}
}
