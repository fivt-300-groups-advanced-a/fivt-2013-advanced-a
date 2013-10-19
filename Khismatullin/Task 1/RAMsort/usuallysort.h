#ifndef USORT

#define USORT

#include<vector>
#include<algorithm>

template<class comp, class T>
void usually_sort(std :: vector<T> &v, comp cmp)
{
	sort(v.begin(), v.end(), cmp);
}


#endif