#ifndef STRONGCONNECTIVITY_H
#define STRONGCONNECTIVITY_H

#include <vector>

#include "graph.h"

namespace graph
{
	namespace strong_connectivity
	{

		namespace impl
		{
			struct vertex
			{
				vertex(): used(false) {}

				vertex_t link, color;
				bool used;
			};

			void findComponents(const Graph &g, std::vector<vertex_t> &stack,
							std::vector<impl::vertex> &info, vertex_t v, std::size_t &cnt)
			{
				if (info[v].used) return;
				info[v].used = true;
				info[v].link = cnt++;
				stack.push_back(v);
				bool isRoot = true;
				for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
				{
					int to = it->getVertex();
					if (!info[to].used) findComponents(g, stack, info, to, cnt);
					if (info[v].link > info[to].link)
					{
						info[v].link = info[to].link;
						isRoot = false;
					}
				}
				while (isRoot)
				{
					vertex_t u = stack.back();
					stack.pop_back();
					info[u].color = v;
					info[u].link = std::numeric_limits<vertex_t>::max();
					if (u == v) isRoot = false;
				}
			}

			vertex_t findSinkPair(const Graph &g, std::vector<bool> &used, vertex_t v)
			{
				if (used[v]) return g.size();
				used[v] = true;
				if (!g.getEdgesFrom(v)->size()) return v;
				for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
				{
					vertex_t result = findSinkPair(g, used, it->getVertex());
					if (result != g.size()) return result;
				}
				return g.size();
			}
		}

		std::vector<vertex_t> findComponents(const Graph &g)
		{
			std::vector<vertex_t> stack;
			std::vector<impl::vertex> info(g.size());
			std::size_t cnt = 0;
			for (std::size_t v = 0; v < g.size(); ++v)
				impl::findComponents(g, stack, info, v, cnt);
			std::vector<vertex_t> color(g.size());
			for (vertex_t v = 0; v < g.size(); ++v)
				color[v] = info[v].color;
			return color;
		}

		bool isStrongConnected(const Graph &g)
		{
			std::vector<vertex_t> color = findComponents(g);
			for (vertex_t v = 1; v < g.size(); ++v)
				if (color[v] != color[0]) return false;
			return true;
		}

		std::vector< std::pair<vertex_t, vertex_t> > strongConnectivityAugmentation(const Graph &g)
		{
			std::vector<vertex_t> color = findComponents(g);

			std::vector<DefaultListBuilder> builders(g.size(), DefaultListBuilder(g.size()));
			std::vector<std::size_t> edgesTo(g.size(), 0);
			for (vertex_t v = 0; v < g.size(); ++v)
				for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
					if (color[v] != color[it->getVertex()])
					{
						builders[color[v]].addEdge(color[it->getVertex()]);
						++edgesTo[color[it->getVertex()]];
					}

			std::vector< std::unique_ptr<IncidenceList> > lists;
			std::vector<vertex_t> sinks, sources, alone;
			for (vertex_t v = 0; v < g.size(); ++v)
			{
				lists.emplace_back(builders[v].getList());
				if (color[v] == v)
				{
					if (!lists.back()->size() && !edgesTo[v]) alone.push_back(v);
					else if (!edgesTo[v]) sources.push_back(v);
				}
			}

			Graph condensation(lists);

			std::vector<bool> used(g.size()), isGoodSink(g.size(), false);
			std::size_t common = 0;
			for (std::size_t ptr = 0; ptr < sources.size(); ++ptr)
			{
				vertex_t matched = impl::findSinkPair(condensation, used, sources[ptr]);
				if (matched == condensation.size()) continue;
				sinks.push_back(matched);
				std::swap(sources[common], sources[ptr]);
				isGoodSink[matched] = true;
				++common;
			}
			for (vertex_t v = 0; v < g.size(); ++v)
				if (!condensation.getEdgesFrom(v)->size() && edgesTo[v] && !isGoodSink[v])
					sinks.push_back(v);

			std::vector< std::pair<vertex_t, vertex_t> > answer;
			for (size_t i = 0; i + 1 < common; ++i)
				answer.emplace_back(sinks[i], sources[i + 1]);
			if (alone.empty()) answer.emplace_back(sinks[common - 1], sources[0]);
			else if (!sinks.empty())
			{
				assert(!sources.empty());
				answer.emplace_back(sinks[common - 1], alone[0]);
				for (std::size_t i = 0; i + 1 < alone.size(); ++i)
					answer.emplace_back(alone[i], alone[i + 1]);
				answer.emplace_back(alone.back(), sources[0]);
			}
			else
			{
				for (std::size_t i = 0; i + 1 < alone.size(); ++i)
					answer.emplace_back(alone[i], alone[i + 1]);
				if (alone.size() > 1)
					answer.emplace_back(alone.back(), alone[0]);
			}

			for (std::size_t i = common; i < std::min(sources.size(), sinks.size()); ++i)
				answer.emplace_back(sinks[i], sources[i]);
			for (std::size_t i = sinks.size(); i < sources.size(); ++i)
				answer.emplace_back(sinks.back(), sources[i]);
			for (std::size_t i = sources.size(); i < sinks.size(); ++i)
				answer.emplace_back(sinks[i], sources.back());

			if (!sinks.empty())
				assert(answer.size() == std::max(sinks.size(), sources.size()) + alone.size());
			else
				assert(answer.size() == (alone.size() == 1 ? 0 : alone.size()));

			return answer;
		}
	}
}

#endif // STRONGCONNECTIVITY_H
