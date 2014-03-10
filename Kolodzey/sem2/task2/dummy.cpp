#include <vector>
#include <iostream>

int dummy(std::vector<int> &input) {
  //check input
  if (input.size() == 0)
    return 0;
  if (input.size() > 20) {
    std::cerr << "Input too long for dummy" << std::endl;
    exit(EXIT_FAILURE);
  }

  //initial ans
  int ans = 0;
  for (size_t i = 0; i < input.size(); ++i)
    ans += input[i];

  unsigned int numberOfMasks = 1 << (input.size() - 1);

  //in mask 0 = +, 1 = •
  //try all masks (0…00 mask is already tried)
  //doesn't cycle when 1 number :) 
  for (unsigned int mask = 1; mask < numberOfMasks; ++mask) {
    int tmpAns = 0;
    int lastProduct = input[0];
    for (size_t i = 1; i < input.size(); ++i) {
      if (mask & (1 << (i - 1)))
        lastProduct *= input[i];
      else {
        tmpAns += lastProduct;
        lastProduct = input[i];
      }
    }
    tmpAns += lastProduct;
    ans = std::max(tmpAns, ans);
  }

  return ans;
}