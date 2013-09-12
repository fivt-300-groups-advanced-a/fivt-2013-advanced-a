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

std::vector<token> constructTokens(const std::vector<std::string> &v)
{
	return std::vector<token>(v.begin(), v.end());
}

int main()
{
#ifdef UNIT_TEST
	unitTest();
	integrationTest();
#endif
	std::string line;
	std::vector< std::vector<token> > data;
	
	while (std::cin >> line)
		data.push_back(constructTokens(parseString(line)));
	
	sort(data.begin(), data.end());
	
	for (auto line : data)
	{
		for (auto part : line)
			std::cout << part.toString();
		std::cout << std::endl;
	}
    return 0;
}
