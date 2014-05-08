#ifndef GEN_H
#define GEN_H
namespace graph {
namespace testtools {
Graph buildSimpleAdjacencyMatrix(const std::vector<std::vector<bool>>& matrix);

/** genMatrixByBoolMask(tuple<int,int> mask) Generates matrix by given mask
  *
  * get<0>(mask) -> size of graph
  * get<1>(mask) -> bitmask, that describes graph
  *
  * for example, mask = 749, size = 4
  *
  * 749 = 1011101101
  *      #9876543210
  * we enumerate bits from the lowest
  *
  *   bits correspond to the   |   so adjacency matrix for (4; 749)
  * adjacency matrix like this |        will look like this
  *    12  0  1  2             |              0 1 0 1
  *     6 13  3  4             |              1 0 1 0
  *     7  8 14  5             |              1 0 0 1
  *     9 10 11 15             |              1 0 0 0
  *
  * So, masks from [0; 2 ^ (size * (size - 1) / 2) ) = topsorted masks
  * all upper triangular adjacency matrixes with zeros on the main diagonal
  *
  * masks from [0; 2 ^ (size * (size - 1)) ) = masks without self-loops
  * all adjacency matrixes with zeros on the main diagonal */
std::vector<std::vector<bool>> genMatrixByBoolMask(std::tuple<int,int> mask);
}//namespace testtools
}//namespace graph
#endif //GEN_H