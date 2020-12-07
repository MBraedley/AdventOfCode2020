// Day07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <vector>
#include <set>
#include <unordered_map>
#include <cassert>

std::set<std::string> SearchForContainers(const std::string& target, const std::unordered_map<std::string, std::string>& rules)
{
	std::set<std::string> ret;

	for (auto rule : rules)
	{
		std::string contents = rule.second;
		if (contents.find(target) != contents.npos)
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

std::uint64_t GetContentCount(const std::string& container, const std::unordered_map<std::string, std::string>& rules, std::unordered_map<std::string, std::uint64_t> totalBags)
{
	std::uint64_t count = 0;
	std::regex search("(\\d+) ([a-z]+ [a-z]+) bag[s]?");
	std::smatch m;

	assert(rules.find(container) != rules.end());
	std::string contents = rules.at(container);
	while (std::regex_search(contents, m, search))
	{
		int bagCount = GetInt(m[1]);
		std::uint64_t containedBags = 0;
		if (totalBags.find(m[2]) != totalBags.end())
		{
			containedBags = totalBags[m[2]];
		}
		else
		{
			containedBags = GetContentCount(m[2], rules, totalBags);
			totalBags[m[2]] = containedBags;
		}
		count += bagCount + bagCount * containedBags;
		contents = m.suffix();
	}

	return count;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	std::string line;
	std::unordered_map<std::string, std::string> rules;

	while (std::getline(inputStrm, line))
	{
		std::regex parser("([a-z]+ [a-z]+) bags contain (.+)");
		std::smatch m;
		assert(std::regex_match(line, m, parser));
		rules.emplace(m[1], m[2]);
	}

	std::string targetBag = "shiny gold";
	auto containers = SearchForContainers(targetBag, rules);

	std::cout << containers.size() << std::endl;

	std::unordered_map<std::string, std::uint64_t> totalBags;
	auto contained = GetContentCount(targetBag, rules, totalBags);

	std::cout << contained << std::endl;
}
