#ifndef SORT_TOKEN_H
#define SORT_TOKEN_H

#include <string>

class Token
{
	private:
		std::string rawData;
		bool isNum;
		
		bool compareNumbers(const std::string &a, const std::string &b) const
		{
			std::string::size_type aZeros = 0, bZeros = 0;
			while (aZeros < a.length() && a[aZeros] == '0') aZeros++;
			while (bZeros < b.length() && b[bZeros] == '0') bZeros++;
			
			std::string first = a.substr(aZeros, a.length() - aZeros);
			std::string second = b.substr(bZeros, b.length() - bZeros);
			
			if (first.length() != second.length()) return first.length() < second.length();
			if (first != second) return first < second;
			return aZeros < bZeros;			
		}

	public:
		explicit Token(const std::string &ndata, int nvalue, bool flag): rawData(ndata), isNum(flag) {}
		explicit Token(const std::string &s): rawData(s), isNum((bool) isdigit(s[0])) {}

		const bool isNumber() const
		{
			return isNum;
		}

		const std::string &toString() const
		{
			return rawData;
		}

		bool operator < (const Token &t) const
		{
			if (isNum != t.isNum) return isNum < t.isNum;
			if (isNum) return compareNumbers(rawData, t.rawData);
			return rawData < t.rawData;
		}
};

#endif
