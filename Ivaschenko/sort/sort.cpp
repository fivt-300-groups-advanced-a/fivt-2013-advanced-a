#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <map>

#include <cassert>

#include "token.h"
#include "test.h"

std::vector<std::string> parseString(const std::string &s)
{
	std::vector<std::string> result;
	for (auto it = s.begin(); it != s.end();)
	{
		auto jt = it;
		bool expectDigit = isdigit(*it);
		while (jt != s.end() && expectDigit == (bool) isdigit(*jt)) ++jt;
		result.push_back(std::string(it, jt));
		it = jt;
	}
	return result;
}

std::vector<std::string> sortStrings(const std::vector<std::string> &v)
{
	std::vector< std::vector<token> > data;
	std::vector<std::string> answer;
	
	for (auto line : v)
	{
		std::vector<std::string> tokens = parseString(line);
		data.push_back(std::vector<token>(tokens.begin(), tokens.end()));
	}
	
	sort(data.begin(), data.end());
	
	for (auto line : data)
	{
		answer.push_back("");
		for (auto part : line)
			answer.back() += part.toString();
	}
	return answer;
}

std::vector<std::string> sortStrings(std::istream &in)
{
	std::vector<std::string> lines;
	std::string line;
	while (in >> line) lines.push_back(line);
	return sortStrings(lines);
}

int main()
{
#ifdef UNIT_TEST
	unitTest();
	integrationTest();
#endif
	for (auto line : sortStrings(std::cin))
		std::cout << line << std::endl;
    return 0;
}
