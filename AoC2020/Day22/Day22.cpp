// Day22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::string line;

	std::deque<std::uint32_t> player1, player2;

	std::getline(inputStrm, line);
	while (std::getline(inputStrm, line) && !line.empty())
	{
		player1.push_back(std::atoi(line.c_str()));
	}

	std::getline(inputStrm, line);
	while (std::getline(inputStrm, line) && !line.empty())
	{
		player2.push_back(std::atoi(line.c_str()));
	}

	std::deque<std::uint32_t> ogP1 = player1;
	std::deque<std::uint32_t> ogP2 = player2;

	while (!player1.empty() && !player2.empty())
	{
		auto card1 = player1.front();
		auto card2 = player2.front();
		player1.pop_front();
		player2.pop_front();

		if (card1 > card2)
		{
			player1.push_back(card1);
			player1.push_back(card2);
		}
		else
		{
			player2.push_back(card2);
			player2.push_back(card1);
		}
	}

	std::deque<std::uint32_t> winner = !player1.empty() ? player1 : player2;

	std::uint64_t score = 0;
	std::uint64_t multi = 1;

	while (!winner.empty())
	{
		score += winner.back() * multi;
		multi++;
		winner.pop_back();
	}

	std::cout << score << std::endl;
}
