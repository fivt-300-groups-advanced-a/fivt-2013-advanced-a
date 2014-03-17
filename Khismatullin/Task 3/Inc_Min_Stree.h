#ifndef INC_MIN_SEGMENT_TREE

#define INC_MIN_SEGMENT_TREE

#include "Stree.h"
#include <algorithm>

const int maxlongint = 2147483647;

class IMn_Upd
{
	public:
		IMn_Upd() {
			value = 0;
		}
		explicit IMn_Upd(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const IMn_Upd &right) {
			value += right.get();
		}
	private:
		int value;
};

class IMn_Type
{
	public:
		IMn_Type() {
			value = maxlongint;
		}
		explicit IMn_Type(int new_value) {
			value = new_value;
		}
		int get() const{
			return value;
		}
		void merge(const IMn_Type &right) {
			value = std::min(value, right.get());
		}
		void update(IMn_Upd upd, int l, int r) {
			value += upd.get();
		}
	private:
		int value;
};

#endif