#ifndef SETINCIDENCELIST_H
#define SETINCIDENCELIST_H

#include "model/incidencelist.h"

#include <set>

namespace graph
{
	class SetIncidenceListIterator : public IncidenceListIterator
	{
		public:
			SetIncidenceListIterator(std::set<vertex_t>::const_iterator from): it(from) {}

			vertex_t operator * () const override
			{
				return *it;
			}

			std::unique_ptr<IncidenceListIterator> next() const override
			{
				std::set<vertex_t>::const_iterator nextIt = it;
				return std::move(std::unique_ptr<IncidenceListIterator>(new SetIncidenceListIterator(++nextIt)));
			}

			void moveForward() override
			{
				++it;
			}

			bool operator == (const std::unique_ptr<IncidenceListIterator> &it) const override
			{
				SetIncidenceListIterator* vit =
						dynamic_cast<SetIncidenceListIterator*>(it.get()); // ohhh...
				assert(vit != nullptr);
				return this->it == vit->it;
			}

			bool operator != (const std::unique_ptr<IncidenceListIterator> &it) const override
			{
				SetIncidenceListIterator* vit =
						dynamic_cast<SetIncidenceListIterator*>(it.get()); // ohhh...
				assert(vit != nullptr);
				return this->it != vit->it;
			}

		private:
			std::set<vertex_t>::const_iterator it;
	};

	class SetIncidenceList : public IncidenceList
	{
		public:
			SetIncidenceList() {}
			SetIncidenceList(const std::initializer_list<vertex_t> &neighbours): adjList(neighbours) {}
			SetIncidenceList(const std::set<vertex_t> &neighbours): adjList(neighbours) {}

			std::size_t size() const override
			{
				return adjList.size();
			}

			std::unique_ptr<IncidenceListIterator> begin() const override
			{
				return std::unique_ptr<IncidenceListIterator>(new SetIncidenceListIterator(adjList.begin()));
			}

			std::unique_ptr<IncidenceListIterator> end() const override
			{
				return std::unique_ptr<IncidenceListIterator>(new SetIncidenceListIterator(adjList.end()));
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
