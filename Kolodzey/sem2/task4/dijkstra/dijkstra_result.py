class DijkstraResult(dict):
    """
    simple class to hold and print result of dijkstra function
    """
    #TODO write good search print of path
    from collections import namedtuple
    FullTuple = namedtuple("FullTuple", "result parent edge")
    OnlyParentTuple = namedtuple("OnlyParentTuple", "result parent")

    def __init__(self, start, backtrace_mode):
        self.start = start
        self.backtrace_mode = backtrace_mode

    def add_result(self, vertex, result, parent, edge):
        if self.backtrace_mode == "full":
            self[vertex] = self.FullTuple(result, parent, edge)
        elif self.backtrace_mode == "only_parent":
            self[vertex] = self.OnlyParentTuple(result, parent)
        else:
            self[vertex] = result