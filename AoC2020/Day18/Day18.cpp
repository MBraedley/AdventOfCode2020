// Day18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

enum class Operation
{
	ADD,
	MULTI,
};

std::uint64_t Evaluate(const std::string& equation, std::string::iterator& pos)
{
	std::uint64_t val = 0;
	Operation op = Operation::ADD;
	
	while (pos != equation.end())
	{
		if (*pos == ' ')
		{
			pos++;
		}
		else if (*pos == '(')
		{
			pos++;
			if (op == Operation::ADD)
			{
				val += Evaluate(equation, pos);
			}
			else
			{
				val *= Evaluate(equation, pos);
			}
		}
		else if (*pos == ')')
		{
			pos++;
			return val;
		}
		else if (*pos == '+')
		{
			op = Operation::ADD;
			pos++;
		}
		else if (*pos == '*')
		{
			op = Operation::MULTI;
			pos++;
		}
		else
		{
			if (op == Operation::ADD)
			{
				val += (static_cast<std::uint64_t>(*pos) - '0');
			}
			else
			{
				val *= Evaluate(equation, pos);
			}

			if (pos != equation.end())
			{
				pos++;
			}
		}
	}

	return val;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<std::string> equations;

	std::uint64_t sum = 0;
	while (std::getline(inputStrm, line))
	{
		equations.push_back(line);
		auto pos = line.begin();
		sum += Evaluate(line, pos);
	}

	std::cout << sum << std::endl;
}