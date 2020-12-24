// Day22.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <unordered_set>
#include <deque>
#include <filesystem>
#include <cassert>

void PrintScore(std::deque<uint32_t> winner)
{
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

std::string GetDeckAsString(const std::deque<std::uint32_t>& deck)
{
	std::stringstream sstrm;
	for (auto card : deck)
	{
		sstrm << card << ",";
	}

	return sstrm.str();
}

bool PlaySubGame(std::deque<std::uint32_t>& player1, std::deque<std::uint32_t>& player2)
{
	std::unordered_set<std::string> prevP1Decks, prevP2Decks;
	while (!player1.empty() && !player2.empty())
	{
		std::string p1Str = GetDeckAsString(player1);
		std::string p2Str = GetDeckAsString(player2);
		if (prevP1Decks.find(p1Str) != prevP1Decks.end() && prevP2Decks.find(p2Str) != prevP2Decks.end())
		{
			return true;
		}

		prevP1Decks.insert(p1Str);
		prevP2Decks.insert(p2Str);

		auto card1 = player1.front();
		auto card2 = player2.front();
		player1.pop_front();
		player2.pop_front();

		bool p1Wins = true;

		if (player1.size() >= card1 && player2.size() >= card2)
		{
			std::deque<std::uint32_t>nextP1Deck(player1.begin(), player1.begin() + card1);
			std::deque<std::uint32_t>nextP2Deck(player2.begin(), player2.begin() + card2);
			p1Wins = PlaySubGame(nextP1Deck, nextP2Deck);
		}
		else
		{
			p1Wins = card1 > card2;
		}

		if (p1Wins)
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

	return !player1.empty();
}

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

	PrintScore(winner);

	//Part 2
	if (PlaySubGame(ogP1, ogP2))
	{
		PrintScore(ogP1);
	}
	else
	{
		PrintScore(ogP2);
	}

}
