#ifndef GENERALSEGMENTTREE_H
#define GENERALSEGMENTTREE_H

#include <utility>
#include <vector>

/**
 * Class, representing a segment tree - a data structure which maintains an array of certain type,
 * can answer queries to get an user-defined associative operation on a semgent of array [left..right]
 * and can perform used-defined modification on any segment.
 *
 * To use this class it it worth to have an idea on internal structure of a tree, mainly about lazy propagation
 * of update information.
 * Briefly, tree requires a structure/class for meta update information which can be applied
 * at any moment to any element of a tree and can be combined (that means a composition of two updates)
 * Both of the methods (update & merge) can depend on a left and right bound of segment of node which they are applied to
 *
 * In addition tree requires an identity value, i.e. a value E such as function of any element x and E is x, vice versa
 * f(x, E) = f(E, x) = x
 *
 * Template parameters:
 *	 - ReturnType			Return type of a function which will be calculated (Function must have operator() returning ReturnType).
 *
 *	 - MetaInformation		Structure for meta information (see above). Default constructor should create meta information which
 *							will not affect on any element
 *
 *	 - Function				A functor representing desired function for segment tree (for exapmle max or min).
 *							Should have ReturnType operator(ReturnType, ReturnType)
 *
 *	 - MetaUpdater			Functor which will be used to update a value in a tree with update information
 *							Should have void operator() (ReturnType &, MetaInformation, std::size_t, std::size_t)
 *							Where two last parameters are bounds of segment where value is updated
 *
 *	 - MetaMerger			Functor which will be used to calculate composition of two modifications
 *							Should have void operator() (MetaInformation &, const MetaInformation &)
 *							Where two last parameters are bounds of segment where information is updated
 */

template<typename ReturnType, typename MetaInformation,
		 typename Function, typename MetaUpdater, typename MetaMerger>

class GeneralSegmentTree
{
	public:
		/**
		 * @brief Creates segment tree of specified size, identity and functors for performing operations.
		 * All elements are identities by default.
		 * Complexity: O(n)
		 * @param size size of tree
		 * @param nIdentity identity element (such as f(E, x) = f(x, E) = x for any x)
		 * @param nFunctor functor to calculate desired f(x, y)
		 * @param nUpdater functor to update elements with meta infomations
		 * @param nMerger functor to calculate composition of two updates
		 */
		template<typename DataType>
			GeneralSegmentTree (std::size_t size,
								const DataType &nIdentity,
								const Function nFunctor = Function(),
								const MetaUpdater nUpdater = MetaUpdater(),
								const MetaMerger nMerger = MetaMerger()):
			identity(nIdentity), functor(nFunctor), updater(nUpdater), merger(nMerger)
		{
			buildTree(std::vector<DataType>(size, nIdentity));
		}

		/**
		 * @brief Creates segment tree from array with specified identity and functors for performing operations.
		 * Complexity: O(n)
		 * @param start iterator to the begin of data
		 * @param end iterator to the end of data
		 * @param nIdentity identity element (such as f(E, x) = f(x, E) = x for any x)
		 * @param nFunctor functor to calculate
		 * @param nUpdater functor to update elements with meta infomations
		 * @param nMerger functor to calculate composition of two updates
		 */
		template<typename DataType, typename ForwardIterator>
			GeneralSegmentTree (ForwardIterator start, ForwardIterator end,
								const DataType &nIdentity,
								const Function nFunctor = Function(),
								const MetaUpdater nUpdater = MetaUpdater(),
								const MetaMerger nMerger = MetaMerger()):
			identity(nIdentity), functor(nFunctor), updater(nUpdater), merger(nMerger)
		{
			buildTree(std::vector<DataType>(start, end));
		}

		/**
		 * @brief get Retunrs funtion on a segment, e.g. f(data[left], data[left + 1], ..., data[right])
		 * Complexity: O(log n)
		 * @param left left bound of segment
		 * @param right right bound of segment
		 * @return desired value of function on a segment
		 */
		ReturnType get(std::size_t left, std::size_t right)
		{
			assert(left <= right && right < n);
			return internalGet(0, 0, (tree.size() >> 1) + 1, left, right + 1);
		}

		/**
		 * @brief update Applies modification on a segment, e.g. data[i] = update(data, info) for all i, left <= i <= right
		 * Complexity: O(log n)
		 * @param left left bound of segment
		 * @param right right bound of segment
		 * @param info update information
		 */
		void update(std::size_t left, std::size_t right, const MetaInformation &info)
		{
			assert(left <= right && right < n);
			internalUpdate(0, 0, (tree.size() >> 1) + 1, left, right + 1, info);
		}

		/**
		 * @brief size returns size of tree
		 * Complexity: O(1)
		 * @return size of tree
		 */
		std::size_t size() const
		{
			return n;
		}

	private:
		std::size_t n;
		ReturnType identity;
		Function functor;
		MetaUpdater updater;
		MetaMerger merger;

		std::vector<ReturnType> tree;
		std::vector<MetaInformation> toPush;

		/**
		 * @brief Builds a tree from array
		 * Complexity: O(n)
		 * @param data initial array
		 */
		template<typename DataType> void buildTree(const std::vector<DataType> &data)
		{
			n = data.size();
			assert(n > 0); // data should be non-empty
			std::size_t size = 1;
			while (size < data.size()) size <<= 1; // size of tree is power of 2

			tree.assign((size << 1) - 1, identity);
			toPush.resize(tree.size());
			std::copy(data.begin(), data.end(), tree.begin() + size - 1);

			for (std::size_t v = size - 2; ~v; --v) // from size - 2 to 0 (internal verticies)
				tree[v] = functor(tree[(v << 1) + 1], tree[(v << 1) + 2]);
		}

		/**
		 * @brief push propagates update information to the sons of vertex
		 * Complexity: O(1)
		 * @param v id of vertex
		 */
		void push(std::size_t v, std::size_t tleft, std::size_t tright)
		{
			if ((v << 1) + 2 < tree.size())
			{
				std::size_t middle = (tleft + tright) >> 1;
				merger(toPush[(v << 1) + 1], toPush[v], tleft, middle - 1);
				merger(toPush[(v << 1) + 2], toPush[v], middle, tright - 1);
			}
			updater(tree[v], toPush[v], tleft, tright - 1);
			toPush[v] = MetaInformation();
		}

		/**
		 * @brief internalGet query to a tree
		 * Complexity: O(log n)
		 * @param v vertex
		 * @param tleft leftest son of v
		 * @param tright rightest son of v
		 * @param left left bound of query
		 * @param right right bound of query
		 * @return desired value
		 */
		ReturnType internalGet(std::size_t v, std::size_t tleft, std::size_t tright,
							   std::size_t left, std::size_t right)
		{
			push(v, tleft, tright);
			if (tleft == left && tright == right)
				return tree[v];
			std::size_t middle = (tleft + tright) >> 1;
			if (right <= middle)
				return internalGet((v << 1) + 1, tleft, middle, left, right);
			if (left >= middle)
				return internalGet((v << 1) + 2, middle, tright, left, right);
			return functor(internalGet((v << 1) + 1, tleft, middle, left, middle),
						   internalGet((v << 1) + 2, middle, tright, middle, right));
		}

		/**
		 * @brief internalUpdate modification query
		 * Complexity: O(log n)
		 * @param v vertex
		 * @param tleft leftest son of v
		 * @param tright rightest son of v
		 * @param left left bound of query
		 * @param right right bound of query
		 * @param info update inforamtion
		 */
		void internalUpdate(std::size_t v, std::size_t tleft, std::size_t tright,
							std::size_t left, std::size_t right, const MetaInformation &info)
		{
			push(v, tleft, tright);
			if (tleft == left && tright == right)
			{
				merger(toPush[v], info, tleft, tright - 1);
				return;
			}
			std::size_t middle = (tleft + tright) >> 1;
			if (right <= middle)
				internalUpdate((v << 1) + 1, tleft, middle, left, middle, info);
			else if (left >= middle)
				internalUpdate((v << 1) + 2, middle, tright, left, right, info);
			else
			{
				internalUpdate((v << 1) + 1, tleft, middle, left, middle, info);
				internalUpdate((v << 1) + 2, middle, tright, middle, right, info);
			}
			push((v << 1) + 1, tleft, middle);
			push((v << 1) + 2, middle, tright);
			tree[v] = functor(tree[(v << 1) + 1], tree[(v << 1) + 2]);
		}
};

#endif // GENERALSEGMENTTREE_H
