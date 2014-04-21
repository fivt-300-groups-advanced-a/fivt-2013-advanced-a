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
			typedef StdIncidenceListIterator< std::set<vertex_t>::const_iterator > iterator_type;

			SetIncidenceList() {}

			template<typename ForwardIterator>
			SetIncidenceList(ForwardIterator begin, ForwardIterator end):
				adjList(begin, end) {}

			std::size_t size() const override
			{
				return adjList.size();
			}

			iterator_pointer getIterator() const override
			{
				return iterator_pointer(new iterator_type(adjList.begin(), adjList.end()));
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
