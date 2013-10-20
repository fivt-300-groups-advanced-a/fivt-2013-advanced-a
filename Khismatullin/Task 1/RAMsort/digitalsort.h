#ifndef DIGSORT

#define DIGSORT

#include<vector>
#include <cstring>


template<class T>
void digital_sort(std :: vector<T> &v)
{
	unsigned int c[256];
	std :: vector<T> new_v(v.size());
	for	(int stape = 0; stape < sizeof(T); stape++) 
	{
		memset(c, 0, sizeof(c));
		for (std :: size_t i = 0; i < v.size(); i++) 
		{
			c[(v[i] >> (stape << 3)) & 255]++;
		}
		unsigned int t = 0;
		for	(int i = 0; i < 256; i++) 
		{
			t += c[i];
			c[i] = t - c[i];
		}
		for (std :: size_t i = 0; i < v.size(); i++) 
		{
			new_v[c[(v[i] >> (stape << 3)) & 255]++] = v[i];
		}
		v.swap(new_v);
	}
	new_v.clear();
}


#endif