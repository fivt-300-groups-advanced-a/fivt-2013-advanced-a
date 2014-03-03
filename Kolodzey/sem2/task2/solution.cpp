#include <iostream>
#include <vector>

int putsigns(std::vector<int> &input) {
  if (input.size() == 0)
    return 0;

  std::vector<int> dp;
  dp.resize(input.size());
  dp[0] = input[0];

  for (int i = 1; i < dp.size(); ++i) { 
    //last sign is +
    dp[i] = dp[i - 1] + input[i];
    //last sign is •
    int lastProduct = input[i];
    for (int j = i - 1; j > 0; --j) {
      lastProduct *= input[j];
      dp[i] = std::max(dp[i], dp[j - 1] + lastProduct);
    }
    //no + in expression
    lastProduct *= input[0];
    dp[i] = std::max(dp[i], lastProduct);
  }

  return dp.back();
}