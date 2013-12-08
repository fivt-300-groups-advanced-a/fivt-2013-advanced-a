#ifndef ADVANCED
#define ADVANCED

#include <vector>
#include "specificTrees.h"

template <class ReturnType, class MetaInformation>
class AdvancedSegmentTree
{
private:
	typedef std::pair<ReturnType, MetaInformation*> internal_data;
	std::vector<internal_data> tree;

	void merge_information(MetaInformation*& old_info, MetaInformation& new_info)
	{
		if (old_info) 
			old_info->merge(&new_info);
		else 
			old_info=new MetaInformation(new_info);
	}

	void shove(const unsigned int& vertex,const int& len)
	{
		if (vertex < tree.size() / 2)
		{
			tree[vertex * 2].first.push(tree[vertex].second,len/2);
			tree[vertex * 2 + 1].first.push(tree[vertex].second,len/2);
			merge_information( tree[vertex * 2].second, *tree[vertex].second );
			merge_information( tree[vertex * 2 + 1].second, *tree[vertex].second );
		}
		delete tree[vertex].second;
		tree[vertex].second = NULL;
	}

	ReturnType get(int vertex, int left, int right, 
				   int vertex_left, int vertex_right)
	{
		if ( left > right ) 
			return ReturnType();
		else if (left == vertex_left && right == vertex_right)
			return tree[vertex].first;
		else {
			if (tree[vertex].second) 
				shove(vertex,vertex_right - vertex_left + 1);
			int mid = (vertex_left + vertex_right) / 2;
				return unite( get(vertex * 2, left, std::min(mid, right), vertex_left, mid),
					      get(vertex * 2 + 1, std::max(left, mid + 1), right, mid + 1, vertex_right));
		}
	}

	void change(int vertex, int left, int right,
				int vertex_left, int vertex_right, MetaInformation& new_change)
	{
		if (left>right) 
			return;
		if (left == vertex_left && right == vertex_right) 
		{
			tree[vertex].first.push(&new_change,vertex_right - vertex_left + 1);
			merge_information(tree[vertex].second, new_change);
		} 
  		else 
		{
			if (tree[vertex].second)
				shove(vertex, vertex_right - vertex_left + 1);
			int mid = (vertex_left + vertex_right) / 2;
			change(vertex * 2, left, std::min(mid, right), vertex_left, mid, new_change);
			change(vertex * 2 + 1, std::max(left, mid + 1), right, mid + 1, vertex_right, new_change);
			tree[vertex].first = unite(tree[vertex * 2].first, tree[vertex * 2 + 1].first);
		}
	}

public:
	template <class RandomAccessIterator>
	AdvancedSegmentTree (RandomAccessIterator first, 
			     RandomAccessIterator last)
	{
		int number_of_elements=last-first;
		int number_at_tree=1;
		while (number_at_tree < number_of_elements) 
			number_at_tree *= 2;
		tree.resize(2 * number_at_tree);
		RandomAccessIterator currentPosition = first;
		for ( int i = number_at_tree; currentPosition!=last; i++ )
		{
			tree[i].first = *currentPosition;
			tree[i].second = NULL;
			currentPosition++;
		}
		for (int i = number_at_tree - 1; i >= 1; i--)
		{
			tree[i].first = unite(tree[i * 2].first, tree[i * 2 + 1].first);
			tree[i].second = NULL;
		}
	}

	ReturnType get(int left,int right) 
	{
		return get(1, left, right, 1, tree.size() / 2);
	}

	void change(int left, int right, MetaInformation changes) 
	{
		change(1, left, right, 1, tree.size() / 2, changes);
	}
	
	~AdvancedSegmentTree()
	{
		for (unsigned int i = 0; i < tree.size(); i++)
			if (tree[i].second) 
				delete tree[i].second;
	}
};
#endif