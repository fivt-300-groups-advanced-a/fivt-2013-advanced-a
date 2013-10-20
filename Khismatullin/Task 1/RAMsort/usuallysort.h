#ifndef USORT

#define USORT

#include<vector>
#include<algorithm>

template<class T, class comp>
void usually_sort(std :: vector<T> &v)
{
	comp cmp;
	sort(v.begin(), v.end(), cmp);
}


#endif