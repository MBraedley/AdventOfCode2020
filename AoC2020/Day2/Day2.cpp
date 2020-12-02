// Day2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <cassert>

struct Rule
{
	char requiredChar;
	std::uint32_t min;
	std::uint32_t max;
};

void ParseLine(const std::string& line, Rule& rule, std::string& password)
{
	std::stringstream sstrm(line);
	char sep1, sep2;
	sstrm >> rule.min >> sep1 >> rule.max >> rule.requiredChar >> sep2 >> password;
}

bool ValidPassword1(const Rule& rule, const std::string& password)
{
	int count = 0;
	for (char c : password)
	{
		if (c == rule.requiredChar)
		{
			count++;
		}
	}

	return count >= rule.min && count <= rule.max;
}

bool ValidPassword2(const Rule& rule, const std::string& password)
{
	if (rule.min > password.length() || rule.max > password.length())
		return false;

	return (password[rule.min - 1] == rule.requiredChar) != (password[rule.max - 1] == rule.requiredChar);
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	int count1 = 0;
	int count2 = 0;

	std::string line;
	while (std::getline(inputStrm, line))
	{
		Rule rule;
		std::string password;
		ParseLine(line, rule, password);
		if (ValidPassword1(rule, password))
		{
			count1++;
		}

		if (ValidPassword2(rule, password))
		{
			count2++;
		}
	}

	std::cout << count1 << std::endl << count2 << std::endl;
}