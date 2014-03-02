#include <string>
#include <vector>
#include <cassert>



void get_result(const std::string& s, const std::string&t, 
			   int s_first, int s_last, int t_first, int t_last, 
			   std::vector<std::string>& list)
{
	if (t_first == t_last)
	{
		for (int i = s_last - 1; i >= s_first; i--)
		{
			std::string notation = "insert ";
			notation.push_back(s[i]);
			notation = notation + " to " + std::to_string((long long) t_first) + " position";
			list.push_back(notation);
		}
	} else
	if (s_first == s_last - 1)
	{
			bool flag = false;
			for (int i = t_last-1; i>=t_first; i--)
				if (flag || t[i] != s[s_first]) 
				{
					list.push_back("delete " + std::to_string((long long)i) + " symbol");
				} else if (t[i] == s[s_first]) 
					flag = true;

			if (!flag) 
			{
				list.pop_back();
				list.push_back("change "+std::to_string((long long) t_first) + " symbol to " + s[s_first]);
			}
	} else
	{
		int t_len = t_last - t_first + 1;
		int s_len = s_last - s_first + 1;
		std::vector<int> dp_left(t_len), dp_right(t_len), 
						 tmp_dp_left(t_len), tmp_dp_right(t_len);

		for (int i = 0; i < t_len; i++)
			tmp_dp_left[i] = dp_left[i] = i;

		int mid_s = s_len / 2;
		for (int i = 1; i <= mid_s; i++)
		{
			dp_left[0] = i;
			for (int j = 1; j < t_len; j++)
			{
				if (s[s_first + i - 1] == t[t_first + j - 1])
					dp_left[j] = tmp_dp_left[j - 1];
				else dp_left[j] = 1 + std::min(std::min(tmp_dp_left[j], tmp_dp_left[j - 1]), dp_left[j - 1]);
			}
			if (i < mid_s)
				tmp_dp_left.swap(dp_left);
		}

		for (int i = 0; i < t_len; i++)
			tmp_dp_right[i] = dp_right[i] = t_len - i - 1;

		for (int i = s_len - 1; i > mid_s; i--)
		{
			dp_right[t_len - 1] = s_len - i;
			for (int j = t_len - 2; j >= 0; j--)
			{
				if (s[s_first + i - 1] == t[t_first + j])
					dp_right[j] = tmp_dp_right[j + 1];
				else dp_right[j] = 1 + std::min(std::min(tmp_dp_right[j], tmp_dp_right[j + 1]), dp_right[j + 1]);
			}
			if ( i > mid_s + 1)
				tmp_dp_right.swap(dp_right);
		}

		int min = t_len + s_len;
		int mid_t = 0;
		for (int i = 0; i < t_len; i++)
			if (dp_left[i] + dp_right[i] < min) 
			{
				min = dp_left[i] + dp_right[i];
				mid_t = i;
			}
		mid_t += t_first;
		mid_s += s_first;

		dp_left.clear(); dp_right.clear();
		tmp_dp_left.clear(); tmp_dp_right.clear();

		get_result(s, t, mid_s, s_last, mid_t, t_last, list);
		get_result(s, t, s_first, mid_s, t_first, mid_t, list);
	}
}


void get_answer(const std::string& s, const std::string& t, std::vector<std::string>& result)
{
	result.clear();
	get_result(s, t, 0, s.length(), 0, t.length(), result);
}

// for informatics

/*
#include <iostream>
int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	std::string a,b;
	std::cin >> a >> b;
	std::vector<std::string> res;
	get_answer(a,b,res);
	std::cout << res.size();
}

*/