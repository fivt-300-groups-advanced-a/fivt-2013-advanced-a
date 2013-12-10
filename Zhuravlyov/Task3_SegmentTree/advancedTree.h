#ifndef ADVANCED
#define ADVANCED

#include <vector>

template <class ReturnType, class MetaInformation, 
			class Push, class Merge, class Unite>
class AdvancedSegmentTree
{
private:
	typedef std::pair<ReturnType, MetaInformation*> internal_data;
	std::vector<internal_data> tree;
	Push pusher;
	Merge merger;
	Unite unifier;

	void merge_information(MetaInformation*& old_info, MetaInformation& new_info)
	{
		if (old_info) 
			merger(old_info, &new_info);
		else 
			old_info=new MetaInformation(new_info);
	}

	void shove(unsigned int vertex, unsigned int len)
	{
		if (vertex < tree.size() / 2)
		{
			pusher(tree[vertex * 2].first, tree[vertex].second, len/2);
			pusher(tree[vertex * 2 + 1].first, tree[vertex].second, len/2);
			merge_information( tree[vertex * 2].second, *tree[vertex].second );
			merge_information( tree[vertex * 2 + 1].second, *tree[vertex].second );
		}
		delete tree[vertex].second;
		tree[vertex].second = NULL;
	}

	ReturnType get(unsigned int vertex, unsigned int left, unsigned int right, 
				   unsigned int vertex_left, unsigned int vertex_right)
	{
		if ( left > right ) 
			return ReturnType();
		else if (left == vertex_left && right == vertex_right)
			return tree[vertex].first;
		else {
			if (tree[vertex].second) 
				shove(vertex,vertex_right - vertex_left + 1);
			unsigned int mid = (vertex_left + vertex_right) / 2;
				return unifier( get(vertex * 2, left, std::min(mid, right), vertex_left, mid),
							    get(vertex * 2 + 1, std::max(left, mid + 1), right, mid + 1, vertex_right));
		}
	}

	void change(unsigned int vertex, unsigned int left, unsigned int right,
				unsigned int vertex_left, unsigned int vertex_right, MetaInformation& new_change)
	{
		if (left>right) 
			return;
		if (left == vertex_left && right == vertex_right) 
		{
			pusher(tree[vertex].first, &new_change, vertex_right - vertex_left + 1);
			merge_information(tree[vertex].second, new_change);
		} 
  		else 
		{
			if (tree[vertex].second)
				shove(vertex, vertex_right - vertex_left + 1);
			unsigned int mid = (vertex_left + vertex_right) / 2;
			change(vertex * 2, left, std::min(mid, right), vertex_left, mid, new_change);
			change(vertex * 2 + 1, std::max(left, mid + 1), right, mid + 1, vertex_right, new_change);
			tree[vertex].first = unifier(tree[vertex * 2].first, tree[vertex * 2 + 1].first);
		}
	}

public:
	template <class RandomAccessIterator>
	AdvancedSegmentTree (RandomAccessIterator first, 
						 RandomAccessIterator last): pusher(Push()), merger(Merge()), unifier(Unite())
	{
		int number_of_elements=last-first;
		int number_at_tree=1;
		while (number_at_tree < number_of_elements) 
			number_at_tree *= 2;
		tree.resize(2 * number_at_tree);
		RandomAccessIterator currentPosition=first;
		for ( int i = number_at_tree; currentPosition!=last; i++ )
		{
			tree[i].first = *currentPosition;
			tree[i].second = NULL;
			currentPosition++;
		}
		for (int i = number_at_tree - 1; i >= 1; i--)
		{
			tree[i].first = unifier(tree[i * 2].first, tree[i * 2 + 1].first);
			tree[i].second = NULL;
		}
	}

	ReturnType get(unsigned int left,unsigned int right) 
	{
		return get(1, left, right, 1, tree.size() / 2);
	}

	void change(unsigned int left, unsigned int right, MetaInformation& changes) 
	{
		change(1, left, right, 1, tree.size() / 2, changes);
	}
	
	~AdvancedSegmentTree()
	{
		for (unsigned int i=0; i < tree.size(); i++)
			if (tree[i].second) 
				delete tree[i].second;
	}
};
#endif