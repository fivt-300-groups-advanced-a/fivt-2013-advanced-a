template <class T, class Compare>
void checkHeapnessTree (const std::unique_ptr<BiTree<T> >& tree, Compare cmp)
{
	TestAccess<BiTree<T>,T> accesstree;
	accesstree._bitree = tree.get();
	typename std::vector <std::unique_ptr <BiTree <T> > >::iterator it;
	for (it = accesstree._child().begin(); it != accesstree._child().end(); ++it)
	{
		EXPECT_EQ(cmp(tree->get_val(), (*it)->get_val()), 1);
		checkHeapnessTree(*it, cmp);
	}	
}

template <class T, class Compare>
void checkHeapnessHeap(BiHeap<T, Compare>& heap)
{
	TestAccess<BiHeap<T,Compare>, T> accessheap;
	accessheap._biheap = &heap;
	typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
	for (it = accessheap._forest().begin(); it != accessheap._forest().end(); ++it)
	{
		checkHeapnessTree(*it, accessheap._cmp());
	}
}

template <class T>
void checkChildAndLevel (const std::unique_ptr<BiTree<T> >& tree)
{
	TestAccess<BiTree<T>,T> accesstree;
	accesstree._bitree = tree.get();
	EXPECT_EQ(accesstree._child().size(), tree->get_level() - 1);
	typename std::vector <std::unique_ptr <BiTree <T> > >::iterator it;
	int i;
	for (i = 1, it = accesstree._child().begin(); it != accesstree._child().end(); ++it, ++i)
	{
		EXPECT_EQ((*it)->get_level(), i);
		checkChildAndLevel(*it);
	}	
}

template <class T, class Compare>
void checkChildAndLevelHeap(BiHeap<T, Compare>& heap)
{
	TestAccess<BiHeap<T,Compare>, T> accessheap;
	accessheap._biheap = &heap;
	typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
	for (it = accessheap._forest().begin(); it != accessheap._forest().end(); ++it)
	{
		checkChildAndLevel(*it);
	}
}

template <class T>
void checkRefsToParent(const std::unique_ptr<BiTree<T> >& tree, BiTree<T>* refparent)
{
	TestAccess<BiTree<T>,T> accesstree;
	accesstree._bitree = tree.get();
	EXPECT_EQ(accesstree._parent(), refparent);
	typename std::vector <std::unique_ptr <BiTree <T> > >::iterator it;
	for (it = accesstree._child().begin(); it != accesstree._child().end(); ++it)
	{
		checkRefsToParent(*it,tree.get());
	}	
}

template <class T, class Compare>
void checkRefsToParentHeap(BiHeap<T, Compare>& heap)
{
	TestAccess<BiHeap<T,Compare>, T> accessheap;
	accessheap._biheap = &heap;
	typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
	for (it = accessheap._forest().begin(); it != accessheap._forest().end(); ++it)
	{
		checkRefsToParent<int>(*it, nullptr);
	}
}

template <class T, class Compare>
void checkSize(BiHeap<T, Compare>& heap)
{
	TestAccess<BiHeap<T,Compare>, T> accessheap;
	accessheap._biheap = &heap;
	typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
	size_t sum = 0;
	for (it = accessheap._forest().begin(); it != accessheap._forest().end(); ++it)
	{
		int k = (*it)->get_level() - 1;
		size_t tmp = 1;
		tmp <<= k;
		sum += tmp;
	}
	EXPECT_EQ(sum, heap.size());
}

template <class T, class Compare>
void checkAllInvariants(BiHeap<T, Compare>& heap)
{
	checkSize<T, Compare>(heap);
	checkRefsToParentHeap<T, Compare>(heap);
	checkChildAndLevelHeap<T, Compare>(heap);
	checkHeapnessHeap<T, Compare>(heap);

}