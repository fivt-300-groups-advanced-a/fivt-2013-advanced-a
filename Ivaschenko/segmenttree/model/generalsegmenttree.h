#ifndef GENERALSEGMENTTREE_H
#define GENERALSEGMENTTREE_H

#include <utility>
#include <vector>

/**
 * Class, representing a segment tree - a data structure which maintain an array of certain type,
 * can answer queries to get an user-defined associative operation on a semgent of array [left..right]
 * and can perform used-defined modification on any segment.
 *
 * To use this class it it worth to have an idea on internal structure of a tree, mainly about lazy propagation
 * of update information.
 * Briefly, tree requires a structure/class for meta update information wich can be applied
 * at any moment to any element of a tree and can be combined (that means a composition of two updates)
 *
 * In addition tree requires an identity value, e.g. a value E such as function of any element x and E is x, vice versa
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
 *							Should have ReturnType operator() (ReturnType, MetaInformation)
 *
 *	 - MetaMerger			Functor which will be used to calculate composition of two modifications
 *							Should have MetaInformation operator() (MetaInformation, MetaInformation)
 */

template<typename ReturnType, typename MetaInformation,
		 typename Function, typename MetaUpdater, typename MetaMerger>

class GeneralSegmentTree
{
	public:
		/**
		 * @brief Creates segment tree of specified size, identity and functors for performing operations.
		 * All elements are identities be default.
		 * @param size size of tree
		 * @param nIdentity identity element (such as f(E, x) = f(x, E) = x for any x)
		 * @param nFunctor functor to calculate
		 * @param nUpdater functor to update elements with meta infomations
		 * @param nMerger functor to calculate composition of two updates
		 */
		template<typename DataType>
			GeneralSegmentTree (std::size_t size,
								const DataType &nIdentity,
								const Function nFunctor = Function(),
								const MetaUpdater nUpdater = MetaUpdater(),
								const MetaMerger nMerger = MetaMerger()):
			n(size), identity(nIdentity), functor(nFunctor), updater(nUpdater), merger(nMerger)
		{
			buildTree(std::vector<DataType>(n, nIdentity));
		}

		/**
		 * @brief Creates segment tree from array with specified identity and functors for performing operations.
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
			n(size), identity(nIdentity), functor(nFunctor), updater(nUpdater), merger(nMerger)
		{
			buildTree(std::vector<DataType>(start, end));
		}

		/**
		 * @brief get Retunrs funtion on a segment, e.g. f(data[left], data[left + 1], ..., data[right])
		 * @param left left bound of segment
		 * @param right right bound of segment
		 * @return desired value
		 */
		const ReturnType& get(std::size_t left, std::size_t right)
		{
			assert(left <= right && right < n);
			return identity;
		}

		/**
		 * @brief update Applies modification on a segment, e.g. data[i] = update(data, info) for all i, left <= i <= right
		 * @param left left bound of segment
		 * @param right right bound of segment
		 * @param info update information
		 */
		void update(std::size_t left, std::size_t right, const MetaInformation &info)
		{
			assert(left <= right && right < n);
		}

		/**
		 * @brief size returns size of tree
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
		 * @param data initial array
		 */
		template<typename DataType> void buildTree(const std::vector<DataType> &data)
		{
			std::size_t size = 1;
			while (size < data.size()) size <<= 1;
			tree.assign(2 * size - 1, identity);
			std::copy(data.begin(), data.end(), tree.begin() + size - 1);
		}

		/**
		 * @brief getValue returns actual value in vertex
		 * @param v id of vertex
		 * @return value in vertex
		 */
		ReturnType getValue(std::size_t v) const
		{
			return updater(tree[v], toPush[v]);
		}

		/**
		 * @brief push
		 * @param v
		 */
		void push(std::size_t v)
		{

		}

		/**
		 * @brief internalGet query to a tree
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
			push(v);
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
			push(v);
			if (tleft == left && tright == right)
			{
				toPush[v] = merger(toPush[v], info);
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
			tree[v] = functor(getValue((v << 1) + 1), getValue((v << 1) + 2));
		}
};

#endif // GENERALSEGMENTTREE_H
