#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <map>

#include <cassert>
#include <cstring>

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
	std::vector< std::vector<Token> > data;
	std::vector<std::string> answer;
	
	for (auto line : v)
	{
		std::vector<std::string> tokens = parseString(line);
		data.push_back(std::vector<Token>(tokens.begin(), tokens.end()));
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

void showHelp()
{
	std::cout << "USAGE: sort [-t|--test] [-h|--help]" << std::endl;
}

void processArguments(int argc, char **argv)
{
	bool launchTest = false;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp("--test", argv[i]) || !strcmp("-t", argv[i])) launchTest = true;
		else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) 
		{
			showHelp();
			exit(0);
		}
		else std::cerr << "Warning: unknown argument " << argv[i] << std::endl;
	}
	if (launchTest)
	{
		unitTest();
		integrationTest();
	}
}

int main(int argc, char **argv)
{
	processArguments(argc, argv);
	for (auto line : sortStrings(std::cin))
		std::cout << line << std::endl;
    return 0;
}
