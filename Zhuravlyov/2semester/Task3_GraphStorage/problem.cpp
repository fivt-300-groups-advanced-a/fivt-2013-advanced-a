#include "graph.h"
#include <vector>
#include <stack>
#include <cassert>

void TarjanAlgoDfs(const Graph& graph, unsigned int vertex, std::vector<unsigned int>& colors,
					unsigned int& current_color, std::stack<unsigned int>& used_verteces,
					unsigned int& current_time, std::vector<unsigned int>& f,
					std::vector<unsigned int>& tin)
{
	used_verteces.push(vertex);
	f[vertex] = tin[vertex] = ++current_time;
	auto it = graph.getIncedents(vertex).getIterator();
	while (it->isValid())
	{
		unsigned int incedent_vertex = it->getCurrentVertex();
		if (colors[incedent_vertex] == 0)
		{
			if (tin[incedent_vertex] == 0)
				TarjanAlgoDfs(graph, incedent_vertex, colors, current_color, used_verteces, current_time, f, tin);
			f[vertex] = std::min(f[vertex], f[incedent_vertex]);
		}
		it->next();
	}
	if (f[vertex] == tin[vertex])
	{
		int new_vertex;
		do {
			new_vertex = used_verteces.top();
			colors[new_vertex] = current_color;
			used_verteces.pop();
		} while (new_vertex != vertex);
		current_color++;
	}
}

void GetStronglyConnectedComponents(const Graph& graph,
									std::vector<unsigned int>& colors, unsigned int& current_color)
{
	current_color = 1;
	colors.assign(graph.getsize(), 0);
	std::stack<unsigned int> used_verteces;
	std::vector<unsigned int> tin(graph.getsize(), 0);
	std::vector<unsigned int> f(graph.getsize());
	unsigned int current_time = 0;
	for (unsigned int i = 0; i < graph.getsize(); i++)
		if (colors[i] == 0)
			TarjanAlgoDfs(graph, i, colors, current_color, used_verteces, current_time, f, tin);
}

int find_compared(unsigned int vertex, const Graph& graph, 
					std::vector<unsigned int>& colors, std::vector<char>& has_output,
					std::vector<char>& used)
{
	used[vertex] = 1;
	if (!has_output[colors[vertex]])
		return vertex;
	auto it = graph.getIncedents(vertex).getIterator();
	int result = -1;
	while (it->isValid())
	{
		unsigned int incedent_vertex = it->getCurrentVertex();
		if (!used[incedent_vertex])
		{
		   if ((result = find_compared(incedent_vertex, graph, colors, has_output, used)) != -1)
			   return result;
		}
		it->next();
	}
	return result;
}

void StrongConnectivityAugmentation(const Graph& graph, 
									std::vector<std::pair<unsigned int, unsigned int>>& result)
{
	std::vector<unsigned int> colors;
	unsigned int max_color;
	GetStronglyConnectedComponents(graph, colors, max_color);
	if (max_color == 2) // Already Strong Connected
		return;
	std::vector<char> has_input(max_color, 0), has_output(max_color, 0);
	for (unsigned int i = 0; i < graph.getsize(); i++)
	{
		auto it = graph.getIncedents(i).getIterator();
		while (it->isValid())
		{
			unsigned int incedent_vertex = it->getCurrentVertex();
			if (colors[i] != colors[incedent_vertex])
				has_output[colors[i]] = has_input[colors[incedent_vertex]] = 1;
			it->next();
		}
	}
	std::vector<std::pair<unsigned int, unsigned int>> pairs;
	std::vector<char> used(graph.getsize(), 0);
	for (unsigned int i = 0; i < graph.getsize(); i++)
		if (!has_input[colors[i]])
		{
			int q = find_compared(i, graph, colors, has_output, used);
			if (q != -1)
			{
				pairs.push_back(std::make_pair(i, q));
				has_input[colors[i]] = 1;
				has_output[colors[q]] = 1;
			}
		}
	for (unsigned int i = 0; i < pairs.size(); i++)
		result.push_back(std::make_pair(pairs[i].second, pairs[(i + 1) % pairs.size()].first));
	std::vector<unsigned int> still_out_first, still_out_second;
	for (unsigned int i = 0; i < graph.getsize(); i++)
	{
		if (!has_input[colors[i]])
		{
			still_out_first.push_back(i);
			has_input[colors[i]] = 1;
		}
		if (!has_output[colors[i]])
		{
			still_out_second.push_back(i);
			has_output[colors[i]] = 1;
		}
	}
	while (still_out_first.size() && still_out_second.size())
	{
		result.push_back(std::make_pair(still_out_second.back(), still_out_first.back()));
		still_out_first.pop_back();
		still_out_second.pop_back();
	}
	while (still_out_first.size())
	{
		result.push_back(std::make_pair(result.back().first, still_out_first.back()));
		still_out_first.pop_back();
	}
	while (still_out_second.size())
	{
		result.push_back(std::make_pair(still_out_second.back(), result.back().second));
		still_out_second.pop_back();
	}
}