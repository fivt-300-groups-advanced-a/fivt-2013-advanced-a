#ifndef DEFAULTLISTBUILDER_H
#define DEFAULTLISTBUILDER_H

#include <climits>
#include <memory>

#include "lists/consecutiveincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/emptyincidencelist.h"
#include "lists/singlevertexlist.h"

#include "graph.h"

namespace graph
{
	class DefaultListBuilder
	{
		public:
			DefaultListBuilder(std::size_t size): n(size), unique(true), dense(false) {}

			std::size_t size() const
			{
				return n;
			}

			std::unique_ptr<IncidenceList> getList()
			{
				if (dense) return std::move(std::unique_ptr<IncidenceList>(new BitsetIncidenceList(connected)));
				if (!is_sorted(adjList.begin(), adjList.end()))
					sort(adjList.begin(), adjList.end());

				bool unique = true;
				for (std::size_t i = 0; i + 1 < adjList.size(); ++i)
					unique &= adjList[i] != adjList[i + 1];

				if (adjList.empty())
					return std::move(std::unique_ptr<IncidenceList>(
										 new EmptyIncidenceList()));

				if (adjList.size() == 1)
					return std::move(std::unique_ptr<IncidenceList>(
										 new SingleVertexList(adjList[0])));

				if (unique && adjList.back() - adjList[0] + 1 == adjList.size())
					return std::move(std::unique_ptr<IncidenceList>(
										 new ConsecutiveIncidenceList(adjList[0], adjList.back())));

				return std::move(std::unique_ptr<IncidenceList>(
									 new VectorIncidenceList(adjList.begin(), adjList.end())));
			}

			void addEdge(vertex_t to)
			{
				assert(to < n);
				if (dense)
				{
					if (connected[to])
					{
						unique = false;
						convertToList();
						adjList.push_back(to);
					}
					else connected[to] = true;
				}
				else
				{
					adjList.push_back(to);
					if (adjList.size() * sizeof(vertex_t) > n / CHAR_BIT && convertToBitset())
					{
						dense = true;
						adjList = std::vector<vertex_t>();
						adjList.shrink_to_fit();
					}
				}
			}

			~DefaultListBuilder() {}

		private:
			std::vector<vertex_t> adjList;
			std::vector<bool> connected;
			std::size_t n;
			bool unique, dense;

			void convertToList()
			{
				assert(dense);
				for (size_t i = 0; i < n; ++i)
					if (connected[i])
						adjList.push_back(i);
				connected = std::vector<bool>();
				connected.shrink_to_fit();
				dense = false;
			}

			bool convertToBitset()
			{
				assert(!dense);
				if (!unique) return false;
				connected.assign(n, false);
				connected.shrink_to_fit();
				for (int v : adjList)
				{
					if (connected[v])
					{
						unique = false;
						connected.clear();
						connected.shrink_to_fit();
						return false;
					}
					connected[v] = true;
				}
				return true;
			}
	};
}

#endif // DEFAULTLISTBUILDER_H
