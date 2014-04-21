#ifndef INCIDENCELISTITERATOR_H
#define INCIDENCELISTITERATOR_H

#include <memory>

namespace graph
{
	typedef std::size_t vertex_t;

	class IncidenceListIterator
	{
		public:
			virtual vertex_t operator * () const = 0;
			vertex_t getVertex() const
			{
				return this->operator * ();
			}

			virtual bool moveForward() = 0;

			virtual bool valid() const = 0;

			virtual ~IncidenceListIterator() {}
	};
}

#endif // INCIDENCELISTITERATOR_H
