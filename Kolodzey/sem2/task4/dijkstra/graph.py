class Graph(dict):
    #TODO reset graph,
    #TODO adding multiple edges
    #TODO adjacency matrix and dict assignment
    def add_vertex(self, vertex):
        self[vertex] = [] #TODO check whether vertex is in graph

    def add_vertex_list(self, vertex_list):
        for vertex in vertex_list:
            self[vertex] = [] #TODO check whether vertex is in graph

    def add_edge(self, parent, child, weight):
        self[parent].append((child, weight)) #TODO check whether parent is in graph


def default_destination_vertex_extract(edge):
    """extracts destination vertex from edge hold in default graph"""
    return edge[0]


def default_weight_extract(edge):
    """extracts weight from edge hold in default graph"""
    return edge[1]