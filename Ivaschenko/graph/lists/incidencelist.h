#ifndef INCIDENCELIST_H
#define INCIDENCELIST_H

#include <utility>
#include <memory>

#include "iterators/incidencelistiterator.h"

namespace graph
{
	class IncidenceList
	{
		public:
			virtual std::size_t size() const = 0;

			std::size_t length() const
			{
				return size();
			}

			virtual std::unique_ptr<IncidenceListIterator> getIterator() const = 0;

			virtual bool connected(vertex_t v) const = 0;

			virtual ~IncidenceList() {}
	};
}

#endif // INCIDENCELIST_H
