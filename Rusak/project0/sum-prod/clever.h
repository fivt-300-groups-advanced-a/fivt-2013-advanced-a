class Clever {
  public:
    int solve() {
      int n = arr.size();

      std::vector<int> dp;
      dp.resize(n);
      dp[0] = arr[0];
      for (int i=1;i<n;i++) {
        int prod = arr[i];
        dp[i] = -INF;
        for (int j=i-1;j>=0;j--) {
          dp[i] = std::max( dp[j]+prod, dp[i]);
          prod*=arr[j];
        }
        dp[i] = std::max(dp[i], prod);
        //std::cout << "DP " << i << " " << dp[i] << "\n";
      }
      return dp[n-1];
    }

    Clever(const std::vector<int> &get_arr) {
      arr = get_arr;
    }
  
  private:
    const int INF = (int)(2e9);
    std::vector<int> arr;
};
