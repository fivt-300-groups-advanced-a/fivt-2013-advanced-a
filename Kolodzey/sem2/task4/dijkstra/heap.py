#TODO write heap
class LinearPriorityQueue(dict):
    def __init__(self, comparator, backtrace_mode):
        self.cmp = comparator
        self.backtrace_mode = backtrace_mode

    def pop_nearest(self):
        minvalue = self.popitem()
        self.update([minvalue])
        for item in iter(self.items()):
            if self.cmp(item[1], minvalue[1]):
                minvalue = item
        del self[minvalue[0]]
        return minvalue

    def update_distance(self, key, value):
        if key in self:
            self[key] = value if self.cmp(value, self.get(key)) else self.get(key)
        else:
            self[key] = value

#TODO put code above into tests
queue = LinearPriorityQueue(lambda x, y: (x < y))
queue.update_distance('A', 4)
queue.update_distance('B', 3)
queue.update_distance('A', 1)
queue.update_distance('A', 2)
queue.update_distance('A', 5)
print(queue.pop_nearest())
print(queue.pop_nearest())