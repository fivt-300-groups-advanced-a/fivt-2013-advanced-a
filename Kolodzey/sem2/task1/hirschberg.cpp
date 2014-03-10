#include <vector>
#include <string>

namespace hirschberg {
  
  enum commandIdentifier {
    kChange,
    kDelete,
    kInsertAfter,
    kInsertBefore,
  };
  
  struct Cmd {
    commandIdentifier cmd;
    char ch;
    int init_pos;
    int res_pos;
  };

  std::pair<std::vector<int>>
  getLastStepsOfWagnerFischer (string source_str,
                               string result_str) {
    source_str.insert(0, '0');
    result_str.insert(0, '0');

    vector<int> step_data[2];

    step_data[0].resize(result_str.length());
    for (int i = 0; i < step_data[0].size(); ++i)
      step_data[0][i] = i;

    step_data[1].resize(result_str.length());

    for (size_t src_i = 1; src_i < source_str.length(); ++src_i) {
      size_t nowsrc = src_i % 2;
      size_t prevsrc = (src_i - 1) % 2;
      step_data[nowsrc][0] = src_i;
      for (size_t res_i = 1; res_i < result_str.length(); ++res_i) {
        //number of prescriptions in case of changing current character
        step_data[nowsrc][res_i] = step_data[prevsrc][res_i - 1] + 1;
        //number of prescriptions in case of inserting after current
        step_data[nowsrc][res_i] = min(step_data[nowsrc][res_i],
                                       step_data[nowsrc][res_i - 1] + 1);
        //number of prescriptions in case of deleting current character
        step_data[nowsrc][res_i] = min(step_data[nowsrc][res_i],
                                       step_data[prevsrc][res_i] + 1);
        //if characters in src and res are the same try to do nothing
        if (source_str[src_i] == result_str[res_i]) {
          step_data[nowsrc][res_i] = min(step_data[nowsrc][res_i],
                                         step_data[prevsrc][res_i - 1]);
        }
      }  
    }

    std::pair<std::vector<int>> ans;

    size_t last_src_id = source_str.length() - 1;
    ans.fisrt = std::move(step_data[last_src_id % 2]);
    ans.second = std::move(step_data[(last_src_id - 1) % 2]);
    return ans;
  }
}