from dijkstra.dijkstra_result import DijkstraResult


def inner_dijkstra(graph,
                   extract_destination_vertex,
                   extract_weight,
                   start,
                   compare_distance,
                   add_edge_to_distance,
                   backtrace_mode,
                   heap_structure):

    """
    Function that is called by dijkstra_launcher.
    Meaning of the params is equal to launcher's meanings.
    See documentation on dijkstra_launcher.
    """

    #setting shorter names for extractors
    destination = extract_destination_vertex
    weight = extract_weight

    #setting initial values
    black = set(start.keys())
    priority_queue = heap_structure(compare_distance, backtrace_mode)
    counted = DijkstraResult(start, backtrace_mode)

    for v in iter(start):
        for edge in graph[v]:
            if not destination(edge) in black:
                priority_queue.update_distance(vertex=destination(edge),
                                               distance=add_edge_to_distance(start[v],
                                                                             weight(edge)),
                                               parent=v,
                                               edge=weight(edge))

    #running algorithm
    while not priority_queue.empty():
        v, v_distance, v_edge_info = priority_queue.pop_nearest()
        black.add(v)
        counted.add_result(v, v_distance, v_edge_info)
        for edge in graph[v]:
            if not destination(edge) in black:
                priority_queue.update_distance(vertex=destination(edge),
                                               distance=add_edge_to_distance(counted.get_distance(v),
                                                                             weight(edge)),
                                               parent=v,
                                               edge=weight(edge))
    return counted


