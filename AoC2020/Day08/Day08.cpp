// Day08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "HandheldConsole.h"

#include <iostream>
#include <vector>

int main()
{
	auto program = HandheldConsole::ReadProgram("input.txt");
	HandheldConsole console(program);
	console.HaltOnLoop(true);
	console.Run();
	std::cout << console.GetAccumulator() << std::endl;

	//part 2
	// This vector was manually generated based on my input.
	// TODO generate this vector automatically, maybe from the debug output or a new method in HandheldConsole
	std::vector<std::size_t> possiblyCorrupted = {
		3, 367, 520, 464, 277, 186, 187, 174, 176, 552, 544, 546, 432,
		326, 436, 371, 86, 362, 625, 626, 137, 230, 129, 130, 270, 581,
		412, 205, 349, 172, 124, 419, 29, 32, 18, 227, 294, 606, 609,
		303, 305, 306, 311, 448, 237, 353, 80, 255, 484, 333, 202, 262,
		264, 374, 233, 523, 525, 528, 427, 594, 595, 299, 394, 406, 488,
		490, 491, 114, 23, 399, 402, 387, 556, 619, 589, 196, 535, 537,
		8, 10, 249, 63, 65, 442, 445, 316 };

	for (auto ip : possiblyCorrupted)
	{
		std::vector<Instruction> adjustedProgram(program.begin(), program.end());
		if (adjustedProgram[ip].op == Operation::JMP)
		{
			adjustedProgram[ip].op = Operation::NOP;
		}
		else
		{
			adjustedProgram[ip].op = Operation::JMP;
		}
		HandheldConsole con(adjustedProgram);
		con.HaltOnLoop(true);

		if (con.Run())
		{
			std::cout << con.GetAccumulator() << std::endl;
			return 0;
		}
		//else
		//{
		//	std::cout << "Loop Detected" << std::endl;
		//}
	}
}
