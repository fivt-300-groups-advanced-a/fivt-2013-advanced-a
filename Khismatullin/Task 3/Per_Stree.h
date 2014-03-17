#ifndef PER_SEGMENT_TREE

#define PER_SEGMENT_TREE

#include "Stree.h"
#include <algorithm>
#include <climits>

class Per_Upd
{
	public:
		Per_Upd() {
			value = 0;
		}
		explicit Per_Upd(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const Per_Upd &right) {
			value = right.get();
		}
	private:
		int value;
};

class Per_Type
{
	public:
		Per_Type() {
			flag = true;
		}
		explicit Per_Type(int value) {
			l = value;
			r = value;
			ans = 1;
			flag = false;
		}
		int get() const{
			return ans;
		}
		int get_l() const{
			return l;
		}
		int get_r() const{
			return r;
		}
		int get_ans() const{
			return ans;
		}
		bool get_flag() const{
			return flag;
		}
		void merge(const Per_Type &right) {
			if (!right.get_flag()) {
				if (flag) {
					l = right.get_l();
					r = right.get_r();
					ans = right.get_ans();
					flag = false;
				} else {
					ans += right.get_ans();
					if (r == right.get_l())
						ans--;
					r = right.get_r();
				}
			}
		}
		void update(Per_Upd upd, int left, int right) {
			l = upd.get();
			r = upd.get();
			ans = 1;
			flag = false;
		}
	private:
		int l, r, ans;
		bool flag;
};

#endif