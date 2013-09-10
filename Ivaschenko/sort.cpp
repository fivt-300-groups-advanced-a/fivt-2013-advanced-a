#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <map>

#include <cassert>

class token
{
	private:
		std::string rawData;
		bool isNum;

	public:
		explicit token(const std::string &ndata, int nvalue, bool flag): rawData(ndata), isNum(flag) {}
		explicit token(const std::string &s): rawData(s), isNum((bool) isdigit(s[0])) {}

		const bool isNumber() const
		{
			return isNum;
		}

		const std::string &toString() const
		{
			return rawData;
		}

		bool operator < (const token &t) const
		{
			if (isNum != t.isNum) return isNum < t.isNum;
			if (isNum && rawData.length() != t.rawData.length()) return rawData.length() < t.rawData.length();
			return rawData < t.rawData;
		}
};

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


std::random_device rndDevice;	

void testAssert(const std::string &message, bool expr)
{
	if (expr) return;
	std::cerr << message << std::endl;
	abort();
}

std::string generateBlock(int len, int type)
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
	std::vector<std::string> manualTests;
	std::vector< std::vector <std::string> > manualAnswers;

	manualTests.push_back("some");
	manualAnswers.push_back(std::vector<std::string>(1, "some"));

	manualTests.push_back("1478");
	manualAnswers.push_back(std::vector<std::string>(1, "1478"));

	manualTests.push_back("/*(^#&@#&!^^#@**!#");
	manualAnswers.push_back(std::vector<std::string>(1, "/*(^#&@#&!^^#@**!#"));

	assert(manualAnswers.size() == manualTests.size());
	for (std::vector<std::string>::size_type i = 0; i < manualTests.size(); i++)
		testAssert("Failed manual test '" + manualTests[i] + "'", manualAnswers[i] == parseString(manualTests[i]));

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
	const int randomTestNumber = 100, maxBlockLength = 20;
	std::uniform_int_distribution<int> textGenerator(1, maxBlockLength);
	for (int i = 0; i < randomTestNumber; i++)
	{
		std::string s = generateBlock(textGenerator(rndDevice), i & 1);
		testAssert("Failed test '" + s + "'", token(s).isNumber() == bool(i & 1));
	}
	std::cerr << "Constructing tested" << std::endl;
}

void unitTest()
{	
	testParsing();
	testTokenConstructing();
	//testTokenComparator();
}

void integrationTest()
{
}

int main()
{
	freopen("output.txt", "w", stderr);
	unitTest();
	integrationTest();
    return 0;
}