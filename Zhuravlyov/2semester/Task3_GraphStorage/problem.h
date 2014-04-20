#ifndef PROBLEM
#define PROBLEM

#include <vector>

void GetStronglyConnectedComponents(const Graph& graph,
									std::vector<unsigned int>& colors, unsigned int& current_color);

void StrongConnectivityAugmentation(const Graph& graph, 
									std::vector<std::pair<unsigned int, unsigned int>>& result);

#endif