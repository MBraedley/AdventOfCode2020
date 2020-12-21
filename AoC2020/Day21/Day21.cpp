// Day21.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;

	std::multiset<std::string> allIngredients;
	std::map<std::string, std::set<std::string>> allergenPossibilities;
	std::regex allergensRegex("\\(contains ([^\\)]+)\\)");
	std::regex splitAllergens("([a-z]+),?");
	std::smatch m;

	while (std::getline(inputStrm, line))
	{
		std::set<std::string> productIngredients;
		std::stringstream ingredStrm(line.substr(0, line.find_first_of('(') - 1));
		std::string ingredient;
		while (ingredStrm >> ingredient)
		{
			productIngredients.insert(ingredient);
			allIngredients.insert(ingredient);
		}

		assert(std::regex_search(line, m, allergensRegex));
		std::string allergensToParse = m[1].str();
		while (std::regex_search(allergensToParse, m, splitAllergens))
		{
			std::string allergen = m[1];
			allergensToParse = m.suffix();

			if (allergenPossibilities.find(allergen) == allergenPossibilities.end())
			{
				allergenPossibilities.emplace(allergen, productIngredients);
			}
			else
			{
				std::vector<std::string> ingredientIntersection;
				std::set_intersection(productIngredients.begin(), productIngredients.end(), allergenPossibilities[allergen].begin(), allergenPossibilities[allergen].end(), std::back_inserter(ingredientIntersection));
				allergenPossibilities[allergen].clear();
				for (auto ingred : ingredientIntersection)
				{
					allergenPossibilities[allergen].insert(ingred);
				}
			}
		}
	}

	auto safeIngredients = allIngredients;
	for (auto& [allergen, ingredients] : allergenPossibilities)
	{
		for (auto i : ingredients)
		{
			safeIngredients.erase(i);
		}
	}

	std::cout << safeIngredients.size() << std::endl;

	std::map<std::string, std::string> definitiveList;

	while (!allergenPossibilities.empty())
	{
		std::set<std::string> solved;

		for (auto& [a, ins] : allergenPossibilities)
		{
			if (ins.size() == 1)
			{
				auto remove = *ins.begin();
				for (auto& [a2, ins2] : allergenPossibilities)
				{
					ins2.erase(remove);
				}
				solved.insert(a);
				definitiveList.emplace(a, remove);
				break;
			}
		}

		for (auto s : solved)
		{
			allergenPossibilities.erase(s);
		}
	}

	for (auto& [a, i] : definitiveList)
	{
		std::cout << i << ",";
	}
}