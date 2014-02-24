#ifndef supply
#define supply
#include <string>
#include <vector>

std::string random_string_generator(int length);

bool test_checker(const std::string& s, std::string t, const std::vector<std::string>& instructions);

#endif