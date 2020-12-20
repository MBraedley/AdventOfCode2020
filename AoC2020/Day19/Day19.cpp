// Day19.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <filesystem>
#include <cassert>

const std::regex leaf("\"?([ab|()]+)\"?");
const std::regex search1("([^|]+)(\\|?)( .+)*");

std::string BuildRuleRegex(std::uint32_t ruleNum, std::unordered_map<std::uint32_t, std::string>& rules)
{
	assert(rules.find(ruleNum) != rules.end());
	if (ruleNum == 8)
	{
		return BuildRuleRegex(42, rules) + "+";
	}

	if (ruleNum == 11)
	{
		std::string rule42 = BuildRuleRegex(42, rules);
		std::string rule31 = BuildRuleRegex(31, rules);

		std::stringstream rule11Strm;
		rule11Strm << "(";

		for (std::uint32_t i = 1; i < 10; i++)
		{
			rule11Strm << rule42 << "{" << i << "}" << rule31 << "{" << i << "}" << "|";
		}
		rule11Strm << rule42 << "{" << 10 << "}" << rule31 << "{" << 10 << "}" << ")";

		return rule11Strm.str();
	}

	std::string rule = rules.at(ruleNum);
	std::smatch m;
	if (std::regex_match(rule, m, leaf))
	{
		return m[1];
	}

	std::regex_match(rule, m, search1);

	if (!m[2].str().empty())
	{
		std::stringstream extract1(m[1].str());
		std::uint32_t num;
		std::stringstream build;
		build << "(";
		while (extract1 >> num)
		{
			build << BuildRuleRegex(num, rules);
		}
		build << "|";
		std::stringstream extract2(m[3].str());
		while (extract2 >> num)
		{
			build << BuildRuleRegex(num, rules);
		}
		build << ")";

		rules[ruleNum] = build.str();
		return rules[ruleNum];
	}
	else
	{
		std::stringstream extract(m[0].str());
		std::uint32_t num;
		std::stringstream build;
		while (extract >> num)
		{
			build << BuildRuleRegex(num, rules);
		}

		rules[ruleNum] = build.str();
		return rules[ruleNum];
	}
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::unordered_map<std::uint32_t, std::string> rules;

	while (std::getline(inputStrm, line) && !line.empty())
	{
		std::uint32_t num = std::atoi(line.c_str());
		std::string rule = line.substr(line.find_first_of(' ') + 1);
		rules.emplace(num, rule);
	}

	//Preload rules 42 and 31
	BuildRuleRegex(42, rules);
	BuildRuleRegex(31, rules);

	std::string myRegex = BuildRuleRegex(0, rules);
	std::cout << myRegex << std::endl;
	std::regex ruleRegex(myRegex);
	std::smatch m;

	std::uint32_t count = 0;
	while (std::getline(inputStrm, line))
	{
		if (std::regex_match(line, m, ruleRegex))
			count++;
	}

	std::cout << count << std::endl;
}
