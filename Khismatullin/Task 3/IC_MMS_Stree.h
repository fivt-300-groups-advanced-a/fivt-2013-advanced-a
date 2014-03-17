#ifndef IC_MMS_SEGMENT_TREE

#define IC_MMS_SEGMENT_TREE

#include "Stree.h"
#include <algorithm>
#include <climits>

const int maxlongint = INT_MAX;
const int minlongint = INT_MIN;

struct MMS
{
	MMS(int _min, int _max, int _sum) 
	{
		min = _min;
		max = _max;
		sum = _sum;
	}
	int min, max, sum;
};

class ICMMS_Upd
{
	public:
		ICMMS_Upd() {
			value = 0; //???
			change = false;
		}
		ICMMS_Upd(int val, bool ch = false) {
			value = val; //???
			change = ch;
		}
		int get() const{
			return value;
		}
		bool get_change() const{
			return change;
		}
		void merge(const ICMMS_Upd &right) {
			if (right.get_change()) {
				value = right.value;
				change = true;
			} else {
				value += right.value;
			}
		}
	private:
		int value;
		bool change;
};

class ICMMS_Type
{
	public:
		ICMMS_Type() {
			min = maxlongint;
			max = minlongint;
			sum = 0;
		}
		ICMMS_Type(int _min, int _max, int _sum) {
			min = _min;
			max = _max;
			sum = _sum;
		}
		ICMMS_Type(int value) {
			min = value;
			max = value;
			sum = value;
		}
		int get_min() const{
			return min;
		}
		int get_max() const{
			return max;
		}
		int get_sum() const{
			return sum;
		}
		MMS get_MMS() const{
			return MMS(min, max, sum);
		}
		void merge(const ICMMS_Type &right) {
			min = std::min(min, right.get_min());
			max = std::max(max, right.get_max());
			sum += right.get_sum();
		}
		void update(ICMMS_Upd upd, int l, int r) {
			if (upd.get_change()) {
				min = upd.get();
				max = upd.get();
				sum = upd.get() * (r - l + 1);
			} else {
				min += upd.get();
				max += upd.get();
				sum += upd.get() * (r - l + 1);
			}
		}
	
	private:
		int min, max, sum;
};

#endif