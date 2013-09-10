#ifndef SORT_TEST_H
#define SORT_TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "token.h"

std::vector<std::string> parseString(const std::string &s);
std::vector<token> constructTokens(const std::vector<std::string> &v);

std::random_device rndDevice;	

void testAssert(const std::string &message, bool expr)
{
	if (expr) return;
	std::cerr << message << std::endl;
	abort();
}

std::string generateBlock(int len, bool type)
{
	char left = 'a', right = 'z'; 
	if (type) left = '0', right = '9';

	std::uniform_int_distribution<char> textGenerator(left, right);
	std::string result;
	for (int i = 0; i < len; i++) result += textGenerator(rndDevice);
	return result;
}

void testParsing()
{	
	std::vector< std::pair< std::string, std::vector< std::string> > > manualTests = 
	{
		std::make_pair("some", std::initializer_list<std::string>{"some"}),
		std::make_pair("1478", std::initializer_list<std::string>{"1478"}),
		std::make_pair("14asd78", std::initializer_list<std::string>{"14", "asd", "78"}),
		std::make_pair("/*(^#&@#&!^^#@**!#", std::initializer_list<std::string>{"/*(^#&@#&!^^#@**!#"})
	};
	
	for (auto test : manualTests)
		testAssert("Failed manual test '" + test.first + "'", test.second == parseString(test.first));

	const int randomTestNumber = 100, maxBlocks = 10, maxBlockLength = 10;
	std::uniform_int_distribution<int> blockNumberRnd(1, maxBlocks), blockLengthRnd(1, maxBlockLength);
	for (int i = 0; i < randomTestNumber; i++)
	{
		std::string raw;
		std::vector<std::string> answer;
		for (int j = 0, state = std::uniform_int_distribution<int>(0, 1)(rndDevice); j < blockNumberRnd(rndDevice); j++, state ^= 1)
		{
			answer.push_back(generateBlock(blockLengthRnd(rndDevice), state));
			raw += answer.back();
		}
		testAssert("Failed test '" + raw + "'!", answer == parseString(raw));
	}
	std::cerr << "Parsing tested" << std::endl;
}

void testTokenConstructing()
{
	std::vector< std::pair<std::string, bool> > manualTests =
	{
		std::make_pair("something", false),
		std::make_pair("7174732773882994778298854878299477299127759927784991794", true),
		std::make_pair("&@^^^^@*&$&@%%??@?!?!#", false)
	};
	
	for (auto test : manualTests)
		testAssert("Failed manual test '" + test.first + "'", token(test.first).isNumber() == test.second);
	
	const int randomTestNumber = 100, maxBlockLength = 20;
	std::uniform_int_distribution<int> textGenerator(1, maxBlockLength);
	for (int i = 0; i < randomTestNumber; i++)
	{
		std::string s = generateBlock(textGenerator(rndDevice), i & 1);
		testAssert("Failed test '" + s + "'", token(s).isNumber() == bool(i & 1));
	}
	std::cerr << "Constructing tested" << std::endl;
}

void testTokenComparator()
{
	std::vector< std::tuple<std::string, std::string, bool> > manualTests = 
	{
		std::make_tuple("asd", "asd", false),
		std::make_tuple("asd", "2848", true),
		std::make_tuple("385", "sq", false),
		std::make_tuple("7381", "73", false),
		std::make_tuple("58", "2848", true),
		std::make_tuple("78", "078", true),
		std::make_tuple("000078", "078", false),
		std::make_tuple("0", "00", true),
		std::make_tuple("0", "0", false)
	};
	
	for (auto test : manualTests)
	{
		std::string message = "Failed manual test ";
		message += "'" + std::get<0>(test) + "' ";
		message += std::get<2>(test) ? "<" : ">";
		message += " '" + std::get<1>(test) + "'";
		testAssert(message, token(std::get<0>(test)) < token(std::get<1>(test)));
	}
}

void unitTest()
{	
	testParsing();
	testTokenConstructing();
	testTokenComparator();
}

void integrationTest()
{
}

#endif
