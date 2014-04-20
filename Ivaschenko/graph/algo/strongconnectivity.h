#ifndef STRONGCONNECTIVITY_H
#define STRONGCONNECTIVITY_H

#include <vector>

#include "graph.h"

namespace graph
{
	class StrongConnectivity;

	namespace impl
	{
		struct vertex
		{
			friend class graph::StrongConnectivity;

			public:
				vertex(): used(false) {}

			private:
				vertex_t link, color;
				bool used;
		};
	}

	class StrongConnectivity
	{
		public:
			StrongConnectivity() = delete;

			static std::vector<vertex_t> findComponents(const Graph &g)
			{
				std::vector<vertex_t> stack;
				std::vector<impl::vertex> info(g.size());
				std::size_t cnt = 0;
				for (std::size_t v = 0; v < g.size(); ++v)
					dfs(g, stack, info, v, cnt);
				std::vector<vertex_t> color(g.size());
				for (vertex_t v = 0; v < g.size(); ++v)
					color[v] = info[v].color;
				return color;
			}

			static bool strongConnected(const Graph &g)
			{
				std::vector<vertex_t> color = findComponents(g);
				for (vertex_t v = 0; v < g.size(); ++v)
					if (color[v] != color[0]) return false;
				return true;
			}

		private:
			static void dfs(const Graph &g, std::vector<vertex_t> &stack,
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
					if (!info[to].used) dfs(g, stack, info, to, cnt);
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
	};
}

#endif // STRONGCONNECTIVITY_H
