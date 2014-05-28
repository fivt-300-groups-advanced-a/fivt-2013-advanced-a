from dijkstra.inner_dijkstra import inner_dijkstra

from dijkstra.graph import Graph
from dijkstra.graph import default_destination_vertex_extract
from dijkstra.graph import default_weight_extract

from dijkstra.algo import default_add_edge_to_distance
from dijkstra.algo import default_compare_distance

from dijkstra.heap import LinearPriorityQueue

def dijkstra_launcher(graph,
                      start,
                      *,
                      graph_mode='default',
                      extract_destination_vertex=None,
                      extract_weight=None,
                      start_mode='one_vertex',
                      backtrace_mode='full',
                      algo_mode='default',
                      compare_distance=None,
                      add_edge_to_distance=None,
                      heap_mode='off',
                      heap_structure=None):
        """Launch dijkstra

        Arguments are
        * graph: a graph to launch dijkstra on.
                 By default it's supported to be an instance
                 of Graph (dijkstra.graph.Graph)
        * start: a vertex/set of vertexes to start with.
                 By default supported to be name of single start vertex,
                 So graph[start] should be an adjacency list of start
        * graph_mode: specifies what graph you want to use.
                      DEFAULT VALUE = 'default'
                      'default' -> graph should be an instance of Graph,
                                   extract_destination_vertex should be not specified
                                   extract_weight should be not specified
                      'manual' -> graph may be any structure which supports
                                    1. graph[vertex] access to iterable adjacency list
                                    2. Let elem be an element of graph[vertex]
                                       So you should specify extract_weight(elem)
                                       and extract_destination_vertex(elem)
                                       which return weight and destination_vertex
                                       weight should be summable with distance
        * extract_destination_vertex: should be specified to work with custom graph
        * extract_weight: should be specified to work with custom graph
        * start_mode: specifies the way you give the information about start vertexes.
                      DEFAULT VALUE = 'one_vertex'
                      'one_vertex'  -> start is a name of single start vertex
                                       graph[start] is an adjacency list of start
                                       distance to start is set to be int(0)
                      'vertex_list' -> start is a list of start vertexes
                                       distance to them is set to be int(0)
                      'vertex_dict' -> start is a dict {vertex1: distance,
                                                        vertex2: distance, … }
        * backtrace_mode: specifies which backtrace information to save about backtrace
                          DEFAULT VALUE = 'full'
                          'full' -> save vertex and edge you come from
                          'only_parent' -> save only vertex you come from
                          'off' -> result will hold only distances, no backtrace info
        *algo_mode: what to use to compare distances and add edge to distance
                    DEFAULT VALUE = 'default'
                    'default' -> comparison = dist1 < dist2, add = dist + edge
                                 compare_distance and add_edge_to_distance should
                                 be not specified
                    'manual' -> you should specify both compare_distance and
                                add_edge_to_distance
        *compare_distance: should be specified to work with custom algo
        *add_edge_to_distance: should be specified to work with custom algo
        *heap_mode: Dijkstra can use heap. Dijkstra can not to use heap.
            DEFAULT VALUE = 'off'
            'off' -> adding to heap o(1), extracting o(len)
            'default' -> feature under construction :)
            'manual' -> you set your own structure to deliver with priority queue requests
                        signature of methods see in heap.py
        *heap_structure: should be specified to work with custom heap

        Base example of using dijkstra
        >>> from dijkstra import dijkstra #you can import dijkstra_launcher as dijkstra
        >>> from dijkstra import Graph
        >>> my_graph = Graph()
        >>> my_graph.add_vertex_list(['A', 'B', 'C'])
        >>> my_graph.add_edge('A', 'B', 1)
        >>> my_graph.add_edge('A', 'C', 5)
        >>> my_graph.add_edge('B', 'C', 3)
        >>> res = dijkstra(my_graph, 'A')
        >>> print("Total distance: {}".format(res.get_distance('C')))
        Total distance: 4
        >>> print("Vertex path: {}".format(res.get_vertex_path('C')))
        Vertex path: ['A', 'B']
        >>> print("Full path: {}".format(res.get_full_path('C')))
        Full path: [FullEdgeInfo(parent='A', edge=1), FullEdgeInfo(parent='B', edge=3)]

        You can change number of starting vertexes
        For example you'd like to choose where to go on Sunday to sleep more on Monday
        >>> from dijkstra import dijkstra
        >>> from dijkstra import Graph
        >>> moscow = Graph()
        >>> moscow.add_vertex_list(['home',
        ...                         'Timiryasevskaya',
        ...                         'Otradnoe',
        ...                         'MIPT'])
        >>> moscow.add_edge('home', 'Otradnoe', 15)
        >>> moscow.add_edge('Otradnoe', 'home', 15)
        >>> moscow.add_edge('Otradnoe', 'Timiryasevskaya', 15)
        >>> moscow.add_edge('Timiryasevskaya', 'Otradnoe', 15)
        >>> moscow.add_edge('Timiryasevskaya', 'MIPT', 25)
        >>> moscow.add_edge('MIPT', 'Timiryasevskaya', 25)
        >>> res = dijkstra(moscow, ['home', 'MIPT'], start_mode='vertex_list')
        >>> print("Full path: {}".format(res.get_full_path('Timiryasevskaya')))
        Full path: [FullEdgeInfo(parent='MIPT', edge=25)]

        You can change init distance of start vertex(es)
        >>> from dijkstra import dijkstra
        >>> from dijkstra import Graph
        >>> my_graph = Graph()
        >>> my_graph.add_vertex_list(['A', 'B', 'C'])
        >>> my_graph.add_edge('A', 'B', 1)
        >>> my_graph.add_edge('A', 'C', 5)
        >>> my_graph.add_edge('B', 'C', 3)
        >>> res = dijkstra(my_graph, {'A': 5}, start_mode='vertex_dict')
        >>> print("Total distance: {}".format(res.get_distance('C')))
        Total distance: 9

        Also you can change functions add_edge_to_distance and compare_distance
        and make your dijkstra solve similar task
        For example, minimize maximal edge on the path
        >>> from dijkstra import dijkstra
        >>> from dijkstra import Graph
        >>> my_graph = Graph()
        >>> my_graph.add_vertex_list(['A', 'B', 'C'])
        >>> my_graph.add_edge('A', 'B', 2)
        >>> my_graph.add_edge('A', 'C', 3)
        >>> my_graph.add_edge('B', 'C', 2)
        >>> res = dijkstra(my_graph, 'A', algo_mode='manual',
        ...                add_edge_to_distance=lambda dist, w: max(dist, w),
        ...                compare_distance=lambda dist1, dist2: dist1 < dist2)
        >>> print("Total distance: {}".format(res.get_distance('C')))
        Total distance: 2
        >>> print("Full path: {}".format(res.get_full_path('C')))
        Full path: [FullEdgeInfo(parent='A', edge=2), FullEdgeInfo(parent='B', edge=2)]
        """

        #checking graph and graph_mode
        if not(graph_mode == 'default' or
               graph_mode == 'manual'):
            print("""Confusing graph_mode.
                     It should be 'default' or 'manual'""")
            print("Now graph_mode = {}".format(graph_mode))
            return

        if graph_mode == 'default':
            if not isinstance(graph, Graph):
                print("""Your graph is not an instance of graph.
                         Set graph_mode to 'manual' to use your own graph.
                         Also it's possible that you mismatched the order of params.
                         Graph should be the first param.""")
                return
            if not (extract_destination_vertex is None or extract_weight is None):
                print("""You specified an extractor using default graph.
                         It is possible that you wanted to use graph_mode = 'manual'""")
                return
            extract_destination_vertex = default_destination_vertex_extract
            extract_weight = default_weight_extract

        if graph_mode == 'manual':
            if extract_destination_vertex is None or extract_weight is None:
                print("""graph_mode is manual but not all extractors are specified.""")
                return

        #checking start and start_mode
        if not (start_mode == 'one_vertex' or
                start_mode == 'vertex_list' or
                start_mode == 'vertex_dict'):
            print("""Confusing start_mode.
                     It should be 'one_vertex' or 'vertex_list' or vertex_dict'""")
            print("Now start_mode = {}".format(start_mode))
            return

        start_dict = {}

        if start_mode == 'one_vertex':
            if not start in graph:
                print("""start_mode is set to 'one_vertex'.
                         But there isn't a start in graph.
                         May be you wanted to use start_mode = 'vertex_list'
                         or start_mode='vertex_dict'?""")
                return
            start_dict = {start: 0}

        if start_mode == 'vertex_list':
            for vertex in start:
                if not vertex in graph:
                    print("""start_mode is set to 'vertex_list'.
                             But one elem of list isn't in graph.
                             May be you wanted to use start_mode = 'one_vertex'
                             or start_mode='vertex_dict'?""")
                    print("Missed elem = {}".format(vertex))
                    return
                start_dict[vertex] = 0

        if start_mode == 'vertex_dict':
            for vertex in iter(start.keys()):
                if not vertex in graph:
                    print("""start_mode is set to 'vertex_dict'.
                             But one key of dict isn't in graph.
                             May be you wanted to use start_mode = 'one_vertex'
                             or start_mode='vertex_list'?""")
                    print("Missed elem = {}".format(vertex))
                    return
            start_dict = start

        #checking backtrace_mode
        if not (backtrace_mode == 'full' or
                backtrace_mode == 'only_parent' or
                backtrace_mode == 'off'):
            print("""Confusing backtrace_mode.
                     backtrace_mode should be 'full' or 'only_parent' or 'off'""")
            print("Now backtrace_mode = {}".format(backtrace_mode))
            return

        #checking algo_mode
        if not (algo_mode == 'default' or
                algo_mode == 'manual'):
            print("""Confusing algo_mode.
                     algo_mode should be 'default' or 'manual''""")
            print("Now algo_mode = {}".format(algo_mode))
            return

        if algo_mode == 'default':
            if not (compare_distance is None or add_edge_to_distance is None):
                print("""You specified an add or compare function using default algo_mode.
                         It is possible that you wanted to use algo_mode = 'manual'""")
                return
            compare_distance = default_compare_distance
            add_edge_to_distance = default_add_edge_to_distance

        if algo_mode == 'manual':
            if compare_distance is None or add_edge_to_distance is None:
                print("""algo_mode is manual but not all algo functions are specified.""")
                return

        #checking heap_mode
        if not(heap_mode == 'default' or
               heap_mode == 'manual' or
               heap_mode == 'off'):
            print("""Confusing heap_mode.
                     It should be 'default' or 'manual' or 'off'""")
            print("Now heap_mode = {}".format(heap_mode))
            return

        if heap_mode == 'default':
            print("Sorry, I haven't written heap yet :(")
            return

        if heap_mode == 'off':
            if not heap_structure is None:
                print("""heap_mode is 'off' but heap_structure is specified.
                         Use heap_mode 'manual' instead.""")
                return
            heap_structure = LinearPriorityQueue

        if heap_mode == 'manual':
            if heap_structure is None:
                print ("""heap_mode is 'manual' but no heap_structure specified.
                          Use heap_mode 'off' or 'default' instead.""")
                return

        #Launch inner_dijkstra
        return inner_dijkstra(graph=graph,
                              extract_destination_vertex=extract_destination_vertex,
                              extract_weight=extract_weight,
                              start=start_dict,
                              add_edge_to_distance=add_edge_to_distance,
                              compare_distance=compare_distance,
                              backtrace_mode=backtrace_mode,
                              heap_structure=heap_structure)

if __name__ == '__main__':
    import doctest
    doctest.testmod()