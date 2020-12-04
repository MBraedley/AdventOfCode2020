// Day4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>
#include <utility>
#include <optional>
#include <regex>

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

std::optional<int> GetInt(std::string in)
{
	std::optional<int> ret = std::nullopt;

	std::stringstream sstrm(in);
	int out;
	if (sstrm >> out)
		ret = out;

	return ret;
}

bool ValidateRecord(const Record& record)
{
	std::regex year("\\d{4}");
	std::smatch m;

	if (!record.birthYear.has_value() || !std::regex_match(record.birthYear.value(), m, year))
		return false;

	auto byr = GetInt(record.birthYear.value());
	if (!byr.has_value() || byr.value() < 1920 || byr.value() > 2002)
		return false;

	if (!record.issueYear.has_value() || !std::regex_match(record.issueYear.value(), m, year))
		return false;

	auto iyr = GetInt(record.issueYear.value());
	if (!iyr.has_value() || iyr.value() < 2010 || iyr.value() > 2020)
		return false;

	if (!record.expiryYear.has_value() || !std::regex_match(record.expiryYear.value(), m, year))
		return false;

	auto eyr = GetInt(record.expiryYear.value());
	if (!eyr.has_value() || eyr.value() < 2020 || eyr.value() > 2030)
		return false;

	std::regex height("(\\d+)(cm|in)");

	if (!record.height.has_value() || !std::regex_match(record.height.value(), m, height))
		return false;

	auto hgt = GetInt(m[1]);
	if (!hgt.has_value())
		return false;

	if (m[2] == "cm")
	{
		if (hgt.value() < 150 || hgt.value() > 193)
			return false;
	}
	else if(m[2] == "in")
	{
		if (hgt.value() < 59 || hgt.value() > 76)
			return false;
	}
	else
	{
		return false;	//Something went really wrong
	}

	std::regex hair("#[0-9a-fA-F]{6}");
	if (!record.hairColor.has_value() || !std::regex_match(record.hairColor.value(), m, hair))
		return false;

	std::regex eye("amb|blu|brn|gry|grn|hzl|oth");
	if (!record.eyeColor.has_value() || !std::regex_match(record.eyeColor.value(), m, eye))
		return false;

	std::regex pid("\\d{9}");
	if (!record.pid.has_value() || !std::regex_match(record.pid.value(), m, pid))
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

	std::cout << count1 << std::endl << count2 << std::endl;
}
