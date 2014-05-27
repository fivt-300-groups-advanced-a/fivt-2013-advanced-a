from dijkstra.dijkstra_result import DijkstraResult

def inner_dijkstra(graph,
                   destination_vertex_extract,
                   weight_extract,
                   start,
                   compare_distance,
                   add_edge_to_distance,
                   backtrace_mode,
                   heap_structure):
    """
    Function that is called by dijkstra_launcher.
    Meaning of the params is equal to launcher's meanings.
    See documentation oo dijkstra_launcher.
    """
    destination = destination_vertex_extract
    weight = weight_extract
    black = set(start.keys())
    priority_queue = heap_structure(compare_distance, backtrace_mode)

    ans = DijkstraResult(start, backtrace_mode)
    for v in iter(start):
        for edge in graph[v]:
            if not destination(edge) in black:
                priority_queue.update_distance(destination(edge),
                                               add_edge_to_distance(start[v],weight(edge)),
                                               edge)
    while len(priority_queue) > 0:
        new_black_vertex, new_ans_distance = priority_queue.pop_nearest()
        black.add(new_black_vertex)
        ans.add_result(new_black_vertex, new_ans_distance, )


