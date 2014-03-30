#ifndef VECTORINCIDENCELIST_H
#define VECTORINCIDENCELIST_H

#include "model/IncidenceList.h"

#include <algorithm>
#include <vector>

namespace graph
{
	class VectorIncidenceListIterator : public IncidenceListIterator
	{
		public:
			VectorIncidenceListIterator(std::vector<vertex_t>::const_iterator from): it(from) {}

			vertex_t operator * () const override
			{
				return *it;
			}

			std::unique_ptr<IncidenceListIterator> next() const override
			{
				return move(std::unique_ptr<IncidenceListIterator>
							(new VectorIncidenceListIterator(it + 1)));
			}

			void moveForward() override
			{
				++it;
			}

			bool operator == (const std::unique_ptr<IncidenceListIterator> &it) const
			{
				VectorIncidenceListIterator* vit =
						dynamic_cast<VectorIncidenceListIterator*>(it.get()); // ohhh...
				assert(vit != nullptr);
				return this->it == vit->it;
			}

			bool operator != (const std::unique_ptr<IncidenceListIterator> &it) const
			{
				VectorIncidenceListIterator* vit =
						dynamic_cast<VectorIncidenceListIterator*>(it.get()); // ohhh...
				assert(vit != nullptr);
				return this->it != vit->it;
			}

			virtual ~VectorIncidenceListIterator() {}

		private:
			std::vector<vertex_t>::const_iterator it;
	};

	class VectorIncidenceList : public IncidenceList
	{
		public:
			VectorIncidenceList() {}
			VectorIncidenceList(std::vector<vertex_t> neighbours): adjList(neighbours) {}
			VectorIncidenceList(const std::initializer_list<vertex_t> &neighbours): adjList(neighbours) {}

			size_t size() const override
			{
				return adjList.size();
			}

			std::unique_ptr<IncidenceListIterator> begin() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new VectorIncidenceListIterator(adjList.begin())));
			}

			std::unique_ptr<IncidenceListIterator> end() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new VectorIncidenceListIterator(adjList.end())));
			}

			bool connected(vertex_t v) const override
			{
				return std::find(adjList.begin(), adjList.end(), v) != adjList.end();
			}

			virtual ~VectorIncidenceList() {}
		private:
			std::vector<vertex_t> adjList;
	};
}

#endif // VECTORINCIDENCELIST_H
