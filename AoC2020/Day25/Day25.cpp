// Day25.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);
	std::uint64_t cardPub, doorPub;
	inputStrm >> cardPub >> doorPub;

	std::uint64_t m = 20201227;
	std::uint64_t cardLoop = 0;
	std::uint64_t doorLoop = 0;

	std::uint64_t pubSubjectNum = 7;
	std::uint64_t value = 1;

	while (value != cardPub)
	{
		value = (value * pubSubjectNum) % m;
		cardLoop++;
	}

	value = 1;
	while (value != doorPub)
	{
		value = (value * pubSubjectNum) % m;
		doorLoop++;
	}

	std::cout << cardLoop << "\t" << doorLoop << std::endl;

	std::uint64_t encryptionKeyCard = 1;
	for (std::uint64_t i = 0; i < cardLoop; i++)
	{
		encryptionKeyCard = (encryptionKeyCard * doorPub) % m;
	}

	std::uint64_t encryptionKeyDoor = 1;
	for (std::uint64_t i = 0; i < doorLoop; i++)
	{
		encryptionKeyDoor = (encryptionKeyDoor * cardPub) % m;
	}

	std::cout << encryptionKeyCard << "\t" << encryptionKeyDoor << std::endl;
}
