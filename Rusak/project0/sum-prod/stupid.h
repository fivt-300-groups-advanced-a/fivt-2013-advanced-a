#include<cstdlib>
#include<iostream>
#include<cmath>
#include<vector>

class Stupid {
  public:
    int solve() {
      int n = arr.size();
      int ans = -INF;
      long long to = (1ll<<(n-1));
      for (long long mask = 0; mask< to; mask++) {
        int cur = 0;
        int cur_prod = arr[0];
        for (int i=0;i<n-1;i++) {
          int bit = (mask>>i)&1;
          if (bit==0) {
            cur+=cur_prod;
            cur_prod = arr[i+1];
          }
          else {
            cur_prod *= arr[i+1];
          }
        }
        cur+=cur_prod;
        ans = std::max(cur, ans);
      }
      return ans;
    }

    Stupid(const std::vector<int> &get_arr) {
      arr = get_arr;
    }
  
  private:
  
    const int INF = (int)(2e9);
    std::vector<int> arr;
};
