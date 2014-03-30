#ifndef INCIDENCELIST_H
#define INCIDENCELIST_H

#include <utility>
#include <memory>

namespace graph
{
	typedef std::size_t vertex_t;

	class IncidenceListIterator
	{
		public:
			virtual vertex_t operator * () const = 0;

			virtual std::unique_ptr<IncidenceListIterator> next() const = 0;
			virtual void moveForward() = 0;

			virtual bool operator == (const std::unique_ptr<IncidenceListIterator> &) const = 0;
			virtual bool operator != (const std::unique_ptr<IncidenceListIterator> &) const = 0;

			virtual ~IncidenceListIterator() {}
	};


	class IncidenceList
	{
		public:
			virtual std::size_t size() const = 0;

			std::size_t length() const
			{
				return size();
			}

			virtual std::unique_ptr<IncidenceListIterator> begin() const = 0;
			virtual std::unique_ptr<IncidenceListIterator> end() const = 0;

			virtual bool connected(vertex_t v) const = 0;

			virtual ~IncidenceList() {}
	};
}

#endif // INCIDENCELIST_H
