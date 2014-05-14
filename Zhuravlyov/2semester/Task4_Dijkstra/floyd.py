__author__ = 'Алексей'

def Floyd(matrix, size, operation = lambda a, b: a + b, cmp = lambda w1, w2: w1 < w2):
    """
    Floyd algorithm to find shortest paths in graph
    Complexity O(V ^ 3), where V is number of vertices in graph
    :param matrix: adjacency matrix of graph.
                   matrix[i][j] - weight of edge between vertices i and j. None if there is no edge
                   at the end of algorithm turns into matrix of shortest paths. None if there is no path
    :param size: size of graph
    :param operation: f(a, b) function which returns weight of union of paths a and b
    :param cmp: cmp(weight1, weight2) Compares weights of two paths.
                Returns true if weight1 < weight2; false otherwise
    """
    for k in range(size):
        for i in range(size):
            for j in range(size):
                if not matrix[i][k] is None and not matrix[k][j] is None:
                    if matrix[i][j] is None or cmp(operation(matrix[i][k], matrix[k][j]), matrix[i][j]):
                        matrix[i][j] = operation(matrix[i][k], matrix[k][j])