// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>
#include <utility>
#include <optional>

/*
byr (Birth Year)
iyr (Issue Year)
eyr (Expiration Year)
hgt (Height)
hcl (Hair Color)
ecl (Eye Color)
pid (Passport ID)
cid (Country ID)
*/

struct Record
{
	std::optional<std::string> birthYear = std::nullopt;
	std::optional<std::string> issueYear = std::nullopt;
	std::optional<std::string> expiryYear = std::nullopt;
	std::optional<std::string> height = std::nullopt;
	std::optional<std::string> hairColor = std::nullopt;
	std::optional<std::string> eyeColor = std::nullopt;
	std::optional<std::string> pid = std::nullopt;
	std::optional<std::string> cid = std::nullopt;
};

std::optional<std::string> GetValue(const std::string& record, const std::string& key)
{
	std::optional<std::string> ret = std::nullopt;

	if (auto pos = record.find(key + ":", 0); pos != std::string::npos)
	{
		pos += 4;
		std::string value = record.substr(pos, record.find_first_of(' ', pos) - pos);
		if (!value.empty())
			ret = value;
	}
	return ret;
}

Record ParseRecord(std::string record)
{
	Record ret;

	ret.birthYear = GetValue(record, "byr");
	ret.issueYear = GetValue(record, "iyr");
	ret.expiryYear = GetValue(record, "eyr");
	ret.height = GetValue(record, "hgt");
	ret.hairColor = GetValue(record, "hcl");
	ret.eyeColor = GetValue(record, "ecl");
	ret.pid = GetValue(record, "pid");
	ret.cid = GetValue(record, "cid");

	return ret;
}

bool ValidateRecord(const Record& record)
{
	int byr = std::atoi(record.birthYear.value().c_str());
	if (byr < 1920 || byr > 2002 || record.birthYear.value().size() != 4)
		return false;

	int iyr = std::atoi(record.issueYear.value().c_str());
	if (iyr < 2010 || iyr > 2020 || record.issueYear.value().size() != 4)
		return false;

	int eyr = std::atoi(record.expiryYear.value().c_str());
	if (eyr < 2020 || eyr > 2030 || record.expiryYear.value().size() != 4)
		return false;

	std::string hgt = record.height.value();
	bool cm = hgt.find("cm") != std::string::npos;
	bool in = hgt.find("in") != std::string::npos;
	if (cm == in)
		return false;

	int hgtVal = std::atoi(hgt.c_str());
	if (cm && (hgtVal < 150 || hgtVal > 193))
		return false;
	else if (in && (hgtVal < 59 || hgtVal > 76))
		return false;




	return true;
}

int main()
{
	std::filesystem::path inputFile("./input.txt");
	assert(std::filesystem::exists(inputFile));

	std::ifstream inputStrm(inputFile);

	std::string line;
	std::string record;
	std::vector<Record> processed;

	while (std::getline(inputStrm, line))
	{
		if (!line.empty())
		{
			record += line + " ";
		}
		else
		{
			processed.push_back(ParseRecord(record));
			record = "";
		}
	}

	if (!record.empty())
	{
		processed.push_back(ParseRecord(record));
		record = "";
	}

	int count1 = 0;
	int count2 = 0;
	// I could do this in the above loop, but I think this is better for the time being
	for (auto entry : processed)
	{
		if (
			entry.birthYear.has_value() &&
			entry.issueYear.has_value() &&
			entry.expiryYear.has_value() &&
			entry.height.has_value() &&
			entry.hairColor.has_value() &&
			entry.eyeColor.has_value() &&
			entry.pid.has_value() 
			//entry.cid.has_value()
			)
		{
			count1++;
			if (ValidateRecord(entry))
			{
				count2++;
			}
		}
	}

	std::cout << count1 << std::endl;
}
