#ifndef INC_MMS_SEGMENT_TREE

#define INC_MMS_SEGMENT_TREE

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

class IMMS_Upd
{
	public:
		IMMS_Upd() {
			value = 0;
		}
		explicit IMMS_Upd(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const IMMS_Upd &right) {
			value += right.get();
		}
	private:
		int value;
};

class IMMS_Type
{
	public:
		IMMS_Type() {
			min = maxlongint;
			max = minlongint;
			sum = 0;
		}
		IMMS_Type(int _min, int _max, int _sum) {
			min = _min;
			max = _max;
			sum = _sum;
		}
		IMMS_Type(int value) {
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
		void merge(const IMMS_Type &right) {
			min = std::min(min, right.get_min());
			max = std::max(max, right.get_max());
			sum += right.get_sum();
		}
		void update(IMMS_Upd upd, int l, int r) {
			min += upd.get();
			max += upd.get();
			sum += upd.get() * (r - l + 1);
		}
	private:
		int min, max, sum;
};

#endif