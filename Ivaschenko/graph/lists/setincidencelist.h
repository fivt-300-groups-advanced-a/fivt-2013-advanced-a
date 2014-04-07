#ifndef SETINCIDENCELIST_H
#define SETINCIDENCELIST_H

#include "lists/incidencelist.h"
#include "iterators/stdincidencelistiterator.h"

#include <set>

namespace graph
{
	class SetIncidenceList : public IncidenceList
	{
		public:
			SetIncidenceList() {}

			template<typename ForwardIterator>
			explicit SetIncidenceList(ForwardIterator begin, ForwardIterator end):
				adjList(begin, end) {}

			std::size_t size() const override
			{
				return adjList.size();
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::unique_ptr<IncidenceListIterator>(
							new StdIncidenceListIterator<std::set<vertex_t>::const_iterator>(adjList.begin(), adjList.end()));
			}

			bool connected(vertex_t v) const override
			{
				return adjList.count(v);
			}

			virtual ~SetIncidenceList() {}

		private:
			std::set<vertex_t> adjList;
	};
}

#endif // SETINCIDENCELIST_H
