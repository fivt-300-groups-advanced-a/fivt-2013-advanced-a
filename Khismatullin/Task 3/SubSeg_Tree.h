#ifndef SubS_SEGMENT_TREE

#define SubS_SEGMENT_TREE

#include "Stree.h"
#include <algorithm>
#include <climits>

class SubS_Upd
{
	public:
		SubS_Upd() {
			value = 0;
		}
		explicit SubS_Upd(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const SubS_Upd &right) {
			value = right.get();
		}
	private:
		int value;
};

class SubS_Type
{
	public:
		SubS_Type() {
			pref = 0;
			suff = 0;
			ans = 0;
			sum = 0;
		}
		explicit SubS_Type(int value) {
			pref = value;
			suff = value;
			sum = value;
			ans = value;
		}
		int get() const{
			return ans;
		}
		int get_ans() const{
			return ans;
		}
		int get_sum() const{
			return sum;
		}
		int get_pref() const{
			return pref;
		}
		int get_suff() const{
			return suff;
		}
		void merge(const SubS_Type &right) {
			ans = max(ans, max(right.get_ans(), suff + right.get_pref()));
			pref = max(pref, sum + right.get_pref());
			suff = max(right.get_suff(), right.get_sum() + suff);
			sum += right.get_sum();
		}
		void update(SubS_Upd upd, int l, int r) {
			sum = upd.get() * (r - l + 1);
			pref = max(0, sum);
			suff = max(0, sum);
			ans = max(0, sum);
		}
	private:
		int sum, ans, pref, suff;
};

#endif