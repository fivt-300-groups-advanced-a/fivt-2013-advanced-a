#ifndef STDINCIDENCELISTITERATOR_H
#define STDINCIDENCELISTITERATOR_H

#include "iterators/incidencelistiterator.h"

namespace graph
{
	template<typename Iterator> class StdIncidenceListIterator : public IncidenceListIterator
	{
		public:
			StdIncidenceListIterator(Iterator from, Iterator to): it(from), end(to) {}

			vertex_t operator * () const override
			{
				return *it;
			}

			std::unique_ptr<IncidenceListIterator> next() const override
			{
				Iterator nextIt = it;
				return std::move(std::unique_ptr<IncidenceListIterator>(new StdIncidenceListIterator(++nextIt, end)));
			}

			bool moveForward() override
			{
				if (!valid()) return false;
				++it;
				return valid();
			}

			bool valid() const override
			{
				return it != end;
			}

			virtual ~StdIncidenceListIterator() {}
		private:
			Iterator it, end;
	};
}

#endif // STDINCIDENCELISTITERATOR_H
