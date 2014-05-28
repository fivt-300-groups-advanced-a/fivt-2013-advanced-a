class DijkstraResult(dict):
    """
    class to hold and print result of dijkstra function
    """
    from collections import namedtuple
    FullHold = namedtuple("FullHold", "distance parent edge")
    OnlyParentHold = namedtuple("OnlyParentHold", "distance parent")
    FullEdgeInfo = namedtuple("FullEdgeInfo", "parent edge")

    def __init__(self, start, backtrace_mode):
        self.start = start
        self.backtrace_mode = backtrace_mode

    def add_result(self, vertex, result, edge_info):
        if self.backtrace_mode == "full":
            self[vertex] = self.FullHold(result, edge_info.parent, edge_info.edge)
        elif self.backtrace_mode == "only_parent":
            self[vertex] = self.OnlyParentHold(result, edge_info.parent)
        else:
            self[vertex] = result

    def get_distance(self, vertex):
        if self.backtrace_mode == "off":
            return self[vertex]
        else:
            return self[vertex].distance

    def get_vertex_path(self, vertex):
        if self.backtrace_mode == "off":
            print("Can't get vertex path because backtrace is off")
            return
        ans = []
        while vertex in self:
            ans.append(self[vertex].parent)
            vertex = self[vertex].parent
        ans.reverse()
        return ans

    def get_full_path(self, vertex):
        if self.backtrace_mode != "full":
            print("Can't get full path because backtrace mode isn't full")
            return
        ans = []
        while vertex in self:
            ans.append(self.FullEdgeInfo(self[vertex].parent, self[vertex].edge))
            vertex = self[vertex].parent
        ans.reverse()
        return ans