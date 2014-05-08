#ifndef COLORING_H
#define COLORING_H
#include <vector>
namespace graph {
namespace algo {
struct Coloring {
 public:
  std::vector<int> color;
  std::vector<int> delegate;
  int getColorOf(int vertex_id) const { return color[vertex_id]; }
  int getRepresentativeOf(int color_id) const { return delegate[color_id]; }
  int getNumberOfColors() const { return delegate.size(); }
  int getNumberOfVertexes() const { return color.size(); }
};
}//namespace algo
}//namespace graph
#endif //COLORING_H