// Day07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <vector>
#include <set>
#include <map>
#include <cassert>

using Contents = std::map<std::string, std::uint32_t>;
using Rules = std::map<std::string, Contents>;

std::set<std::string> SearchForContainers(const std::string& target, const Rules& rules)
{
	std::set<std::string> ret;

	for (auto rule : rules)
	{
		Contents contents = rule.second;
		if (contents.find(target) != contents.end())
		{
			ret.insert(rule.first);
			ret.merge(SearchForContainers(rule.first, rules));
		}
	}

	return ret;
}

int GetInt(const std::string& in)
{
	int out = 0;
	std::stringstream sstrm(in);
	sstrm >> out;
	return out;
}

std::uint32_t GetContentCount(const std::string& container, const Rules& rules)
{
	std::uint32_t count = 0;

	if (auto rule = rules.find(container); rule != rules.end())
	{
		Contents contents = rule->second;
		for (auto bagRule : contents)
		{
			count += bagRule.second + bagRule.second * GetContentCount(bagRule.first, rules);
		}
	}

	return count;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	std::string line;
	Rules rules;

	std::regex ruleRegex("([a-z]+ [a-z]+) bags contain (.+)");
	std::regex contentRegex("(\\d+) ([a-z]+ [a-z]+)?");
	std::smatch m1, m2;

	while (std::getline(inputStrm, line))
	{
		assert(std::regex_match(line, m1, ruleRegex));
		std::string contentStr = m1[2];
		Contents bags;
		while (std::regex_search(contentStr, m2, contentRegex))
		{
			bags.emplace(m2[2], GetInt(m2[1]));
			contentStr = m2.suffix();
		}
		rules.emplace(m1[1], bags);
	}

	std::string targetBag = "shiny gold";
	auto containers = SearchForContainers(targetBag, rules);

	std::cout << containers.size() << std::endl;

	auto contained = GetContentCount(targetBag, rules);

	std::cout << contained << std::endl;
}
