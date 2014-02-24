#include <string>
#include <vector>

std::string random_string_generator(int length)
{
	std::string result;
	for (int i = 0; i < length; i++)
		result.push_back('A' + rand()%26);
	return result;
}

std::string pop_first_word(std::string& text)
{
	std::string word = text.substr(0, text.find(" "));
	text.erase(0, text.find(" ") + 1);
	return word;
}

bool test_checker(const std::string& s, std::string t, const std::vector<std::string>& instructions)
{
	for (int i = 0; i < instructions.size(); i++)
	{
		std::string instruction = instructions[i];
		std::string action = pop_first_word(instruction);
		if (action == "change") {

			std::string s_number = pop_first_word(instruction);
			int number = atoi(s_number.c_str());
			pop_first_word(instruction); pop_first_word(instruction);
			std::string symbol = pop_first_word(instruction);
			t[number] = symbol[0];

		} else if (action == "delete") {

			std::string s_number = pop_first_word(instruction);
			int number = atoi(s_number.c_str());
			t.erase(number, 1);

		} else if (action == "insert") {

			std::string symbol = pop_first_word(instruction);
			pop_first_word(instruction);
			std::string s_number = pop_first_word(instruction);
			int number = atoi(s_number.c_str());
			t.insert(number, symbol);

		} else return false;
	}
	return (s == t);
}