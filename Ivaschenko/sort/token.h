#ifndef SORT_TOKEN_H
#define SORT_TOKEN_H

#include <string>

class token
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
			if (isNum) return compareNumbers(rawData, t.rawData);
			return rawData < t.rawData;
		}
};

#endif
