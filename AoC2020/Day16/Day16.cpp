// Day16.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <cassert>

struct Rule
{
	std::string name;
	std::pair<std::uint32_t, std::uint32_t> range1;
	std::pair<std::uint32_t, std::uint32_t> range2;
};

std::uint32_t GetVal(std::string in)
{
	std::stringstream sstrm(in);
	std::uint32_t ret;
	sstrm >> ret;
	return ret;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;
	std::vector<Rule> rules;

	std::regex ruleRegex("([a-z ]+): (\\d+)-(\\d+)[^\\d]+(\\d+)-(\\d+)");
	std::smatch m;

	while (std::getline(inputStrm, line) && !line.empty())
	{
		assert(std::regex_match(line, m, ruleRegex));
		Rule r;
		r.name = m[1].str();
		r.range1.first = GetVal(m[2].str());
		r.range1.second = GetVal(m[3].str());
		r.range2.first = GetVal(m[4].str());
		r.range2.second = GetVal(m[5].str());

		rules.push_back(r);
	}

	std::getline(inputStrm, line);
	std::getline(inputStrm, line);

	line.append(",");
	std::stringstream ticketStrm(line);
	std::uint64_t val;
	char seperator;
	std::vector<std::uint64_t> myTicket;
	while (ticketStrm >> val >> seperator)
	{
		myTicket.push_back(val);
	}

	std::getline(inputStrm, line);
	std::getline(inputStrm, line);

	std::vector<std::vector<std::uint32_t>> otherTickets;
	std::uint32_t scanError = 0;

	while (std::getline(inputStrm, line))
	{
		line.append(",");
		std::vector<std::uint32_t> ticket;
		std::stringstream sstrm(line);
		bool validTicket = true;
		while (sstrm >> val >> seperator)
		{
			bool success = false;
			for (Rule r : rules)
			{
				if ((val >= r.range1.first && val <= r.range1.second) ||
					(val >= r.range2.first && val <= r.range2.second))
				{
					success = true;
					break;
				}
			}

			if (!success)
			{
				validTicket = false;
				scanError += val;
			}

			ticket.push_back(val);
		}

		if (validTicket)
		{
			assert(ticket.size() == myTicket.size());
			otherTickets.push_back(ticket);
		}
	}

	std::cout << scanError << std::endl;

	//Part2
	std::vector<bool> rValid(myTicket.size(), true);
	std::vector<std::vector<bool>> validRules(rules.size(), rValid);

	for (auto ticket : otherTickets)
	{
		for (std::size_t i = 0; i < rules.size(); i++)
		{
			for (std::size_t j = 0; j < ticket.size(); j++)
			{
				if (validRules[i][j])
				{
					auto r = rules[i];
					auto val = ticket[j];
					validRules[i][j] = (val >= r.range1.first && val <= r.range1.second) ||
						(val >= r.range2.first && val <= r.range2.second);
				}
			}
		}
	}

	for (auto vr : validRules)
	{
		for (bool test : vr)
		{
			std::cout << test ? 1 : 0;
		}
		std::cout << std::endl;
	}

	//TODO: reduce the solution space automatically, this isn't generic
	std::cout << myTicket[1] << "\t" << myTicket[19] << "\t" << myTicket[2] << "\t" << myTicket[4] << "\t" << myTicket[14] << "\t" << myTicket[6] << std::endl;
	std::cout << myTicket[1] * myTicket[19] * myTicket[2] * myTicket[4] * myTicket[14] * myTicket[6] << std::endl;
}