#ifndef CH_MMS_SEGMENT_TREE

#define CH_MMS_SEGMENT_TREE

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

class CMMS_Upd
{
	public:
		CMMS_Upd() {
			value = 0; //???
		}
		explicit CMMS_Upd(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const CMMS_Upd &right) {
			value = right.get();
		}
	private:
		int value;
};

class CMMS_Type
{
	public:
		CMMS_Type() {
			min = maxlongint;
			max = minlongint;
			sum = 0;
		}
		CMMS_Type(int _min, int _max, int _sum) {
			min = _min;
			max = _max;
			sum = _sum;
		}
		CMMS_Type(int value) {
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
		void merge(const CMMS_Type &right) {
			min = std::min(min, right.get_min());
			max = std::max(max, right.get_max());
			sum += right.get_sum();
		}
		void update(CMMS_Upd upd, int l, int r) {
			min = upd.get();
			max = upd.get();
			sum = upd.get() * (r - l + 1);
		}
	private:
		int min, max, sum;
};

#endif