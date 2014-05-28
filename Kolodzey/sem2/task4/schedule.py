from dijkstra import dijkstra #you can import dijkstra_launcher as dijkstra
from dijkstra import Graph
my_graph = Graph()
my_graph.add_vertex_list(['A', 'B', 'C'])
my_graph.add_edge('A', 'B', 1)
my_graph.add_edge('A', 'C', 5)
my_graph.add_edge('B', 'A', 3)
res = dijkstra(my_graph, 'A')
print(res)