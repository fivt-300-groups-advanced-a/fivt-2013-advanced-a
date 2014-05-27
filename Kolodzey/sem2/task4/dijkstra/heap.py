#TODO write heap
class LinearPriorityQueue(dict):
    """
    simple realisation of priority queue
    searching for min O(N)
    adding element O(1)
    """
    from collections import namedtuple
    FullHold = namedtuple("FullTuple", "distance parent edge")
    OnlyParentHold = namedtuple("OnlyParentTuple", "distance parent")
    FullEdgeInfo = namedtuple("FullEdgeInfo", "parent edge")
    OnlyParentEdgeInfo = namedtuple("OnlyParentEdgeInfo", "parent")

    def __init__(self, comparator, backtrace_mode):
        self.cmp = comparator
        self.backtrace_mode = backtrace_mode

    def pop_nearest(self):
        #searching for value
        nearest_vertex = next(iter(self))
        for vertex in iter(self):
            if self.cmp(self.get_distance(vertex), self.get_distance(nearest_vertex)):
                nearest_vertex = vertex
        #set distance to return
        distance = self.get_distance(vertex)
        #set edge_info to return
        if self.backtrace_mode == "full":
            edge_info = self.FullEdgeInfo(self[vertex].parent, self[vertex].edge)
        elif self.backtrace_mode == "only_parent":
            edge_info = self.OnlyParentEdgeInfo(self[vertex].parent)
        else:
            edge_info = None
        #deleting value
        del self[vertex]
        #return!
        return [vertex, distance, edge_info]

    def get_distance(self, vertex):
        if self.backtrace_mode == "off":
            return self[vertex]
        else:
            return self[vertex].distance

    def update_distance(self, vertex, distance, parent, edge):
        old_distance = None
        if vertex in self:
            old_distance = self.get_distance(vertex)
        if vertex not in self or self.cmp(distance, old_distance):
            if self.backtrace_mode == "full":
                self[vertex] = self.FullHold(distance, parent, edge)
            elif self.backtrace_mode == "only_parent":
                self[vertex] = self.OnlyParentHold(distance, parent)
            else:
                self[vertex] = distance


#TODO put code above into tests with all backtrace mode variants!
queue = LinearPriorityQueue(lambda x, y: (x < y), "full")
queue.update_distance('A', 5, 'C', 2)
print(queue.pop_nearest())
queue.update_distance('B', 4, 'C', 3)
print(queue.pop_nearest())
queue.update_distance('A', 3, 'C', 1)
print(queue.pop_nearest())