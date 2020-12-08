#pragma once

#include <vector>
#include <set>
#include <string>

enum class Operation
{
	NOP,
	ACC,
	JMP,
};

struct Instruction
{
	Operation op;
	std::int64_t value;
};

class HandheldConsole
{
public:
	HandheldConsole(const std::vector<Instruction>& program);
	~HandheldConsole() = default;

	static const std::vector<Instruction> ReadProgram(std::string filename);

	void HaltOnLoop(bool enable) { m_HaltOnLoop = enable; }

	void Run();

	std::int64_t GetAccumulator() { return m_Acc; }

private:
	void RunNextInstruction();

	std::size_t m_IP = 0;
	std::int64_t m_Acc = 0;
	std::vector<Instruction> m_Program;
	std::set<std::size_t> m_LoopDetector;
	bool m_HaltOnLoop = false;
	bool m_Halted = false;
};
