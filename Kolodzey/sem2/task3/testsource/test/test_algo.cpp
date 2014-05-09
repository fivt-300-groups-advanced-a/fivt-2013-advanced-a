#include "gtest/gtest.h"
#include "graph.h"
#include "algo.h"
#include "gen.h"
#include "dummyalgo.h"
#include "print.h"

using namespace std;
using namespace graph;
using namespace graph::algo;
using namespace graph::testtools;
using namespace ::testing;

//  Testing accessory algorithms  //
//  ============================  //
TEST(func, getSource) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                          {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
     /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
     /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* v     |           vv v  */
     /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
     /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
     /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
     /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> source = getSource(graph);
  sort(source.begin(), source.end());
  EXPECT_EQ(2, source.size());
  EXPECT_EQ(5, source[0]);
  EXPECT_EQ(6, source[1]);
}
TEST(func, getSink) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                          {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
     /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
     /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* v     |           vv v  */
     /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
     /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
     /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
     /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> sink = getSink(graph);
  sort(sink.begin(), sink.end());
  EXPECT_EQ(2, sink.size());
  EXPECT_EQ(8, sink[0]);
  EXPECT_EQ(9, sink[1]);
}
TEST(func, getIsolated) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                                          {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* 0 <-- 1    4    5  6    */        {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
     /* |     ^          \ |\   */        {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
     /* |     |           \| \  */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
     /* v     |           vv v  */
     /* 2 --> 3           7  8  */        {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
     /*                   |     */        {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
     /*                   v     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
     /*                   9     */        {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}}));
  vector<int> isolated = getIsolated(graph);
  sort(isolated.begin(), isolated.end());
  EXPECT_EQ(1, isolated.size());
  EXPECT_EQ(4, isolated[0]);
}
TEST(func, reverseGraph) {
  vector<vector<bool>> matrix = {{0, 0, 1, 1},
                                 {1, 0, 0, 0},
                                 {0, 1, 1, 0},
                                 {1, 1, 0, 0}};
  Graph g = buildSimpleAdjacencyMatrix(matrix);
  Graph rg = reverseGraph(g);
  vector<vector<bool>> rmatrix(4, vector<bool>(4, 0));
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      rmatrix[i][j] = rg.isConnected(i, j);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_EQ(matrix[i][j], rmatrix[j][i]) << i << " " << j;
}
TEST(func, condenceGraph) {                                    /*  -->4---  */
  Graph graph = buildSimpleAdjacencyMatrix({{0, 0, 1, 0, 1},   /* |       | */ 
  /* expected 012-->4  aka  0-->2  */       {1, 0, 0, 1, 0},   /* |       v */
  /*           |    |       |   |  */       {0, 1, 0, 1, 0},   /* 0<--1-->3 */
  /*           |    v       |   v  */       {0, 0, 0, 0, 0},   /* |   ^   ^ */ 
  /*            --->3        -->1  */       {0, 0, 0, 1, 0}}); /* |   |   | */
  Coloring components;                                         /*  -->2---  */
  components.color = {0, 0, 0, 1, 2};
  components.delegate = {0, 3, 4};
  Graph condenced = condenceGraph(graph, components);
  EXPECT_EQ(3, condenced.size());
  vector<vector<bool>> condenced_matrix = {{0, 1, 1},
                                           {0, 0, 0},
                                           {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_EQ(condenced_matrix[i][j], condenced.isConnected(i, j));
}
//  hasLoop  //
//  =======  //
TEST(hasLoop, Manual) {
  Graph graph0(buildSimpleAdjacencyMatrix({{0, 0, 0},
         /* 1    0    2 */                 {0, 0, 0},
                                           {0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph0));
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 1},
         /* 1<---0--->2 */                 {0, 0, 0},
                                           {0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph1));
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 1},
         /* 1<-->0 */                      {1, 0}}));
  EXPECT_TRUE(hasLoop(graph2));
  Graph graph3(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},     // 0-->1
                                           {0, 0, 1, 0},     //   
                                           {0, 0, 0, 1},     // 2<->3
                                           {1, 0, 0, 0}}));   
  EXPECT_TRUE(hasLoop(graph3));
  Graph graph4(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},     // 0-->1
                                           {0, 0, 1, 0},     // ^   |
                                           {0, 0, 0, 1},     // |   v  
                                           {1, 0, 0, 0}}));  // 3<--2
  EXPECT_TRUE(hasLoop(graph4));
  Graph graph5(buildSimpleAdjacencyMatrix({{1}}));
  EXPECT_TRUE(hasLoop(graph5));
  Graph graph6(buildSimpleAdjacencyMatrix({{0}}));
  EXPECT_FALSE(hasLoop(graph6));
  Graph graph7akaS3t40(buildSimpleAdjacencyMatrix({{0, 0, 0},      //0
                                                   {0, 0, 1},      //
                                                   {0, 1, 0}}));   //1<-->2
  EXPECT_TRUE(hasLoop(graph7akaS3t40));
  Graph graph8akaManual(buildSimpleAdjacencyMatrix({{0, 0, 0, 0, 0, 0},
      /*  0  1--->3           */                    {0, 0, 0, 1, 0, 0},
      /*          ^           */                    {0, 0, 0, 1, 1, 0},
      /*          |           */                    {0, 0, 0, 0, 0, 0},
      /*          2--->4--->5 */                    {0, 0, 0, 0, 0, 1},
                                                    {0, 0, 0, 0, 0, 0}}));
  EXPECT_FALSE(hasLoop(graph8akaManual));
}
TEST(hasLoop, Stress) {
  for (int mask = 0; mask < (1 << 20); ++mask) {
    vector<vector<bool>> matrix = genMatrixByBoolMask(tuple<int,int>(5, mask));
    Graph g = buildSimpleAdjacencyMatrix(matrix);
    ASSERT_EQ(hasLoopDummy(g),hasLoop(g)) << mask;
  }
}
//  getStronglyConnectedComponents  //
//  ==============================  //
TEST(getStronglyConnectedComponents, Manual) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 1, 0, 0, 0},
     /* 0 <-- 2 <-    */                  {0, 0, 0, 0, 0},
     /* |     |   \   */                  {1, 0, 0, 1, 0},
     /* v     v    \  */                  {0, 1, 0, 0, 1},
     /* 1 <-- 3 --> 4 */                  {0, 0, 1, 0, 0}}));  
  Coloring components = getStronglyConnectedComponents(graph);
  //expected division into components: 0; 1; 2 + 3 + 4 
  EXPECT_EQ(3, components.getNumberOfColors());
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(3));
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(4));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(1));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(0));
  EXPECT_FALSE(components.getColorOf(1) == components.getColorOf(0));
}
//  Stress for strong components  //
//  ----------------------------  //
typedef ::testing::TestWithParam<tuple<int,int>> StressByMask;
void checkColoring(const Coloring& expected, const Coloring& value,
            const vector<vector<bool>>& matrix)
{
  ASSERT_EQ(expected.getNumberOfVertexes(),
               value.getNumberOfVertexes()) << matrix;
  ASSERT_EQ(expected.getNumberOfColors(),
               value.getNumberOfColors()) << matrix;
  for (size_t i = 0; i < value.delegate.size(); ++i)
    ASSERT_EQ(i, value.color[value.delegate[i]]);
  for (size_t i = 0; i < value.color.size(); ++i)
    for (size_t j = 0; j < value.color.size(); ++j)
      EXPECT_EQ(expected.color[i] == expected.color[j],
                value.color[i] == value.color[j]);
}
TEST_P(StressByMask, getStronglyConnectedComponentsStress) {
  int size = std::get<0>(GetParam());
  int maxmask = std::get<1>(GetParam());
  for (int mask = 0; mask < maxmask; ++mask) {
    vector<vector<bool>> matrix(genMatrixByBoolMask(tuple<int,int>(size,
                                                                   mask)));
    Graph graph(buildSimpleAdjacencyMatrix(matrix));
    Coloring components = getStronglyConnectedComponents(graph);
    Coloring dummy_components = getStronglyConnectedComponentsDummy(graph);
    checkColoring(dummy_components, components, matrix);
  }
}
INSTANTIATE_TEST_CASE_P(func, StressByMask,
                        Values(tuple<int,int>(4, 1 << 16),
                               tuple<int,int>(5, 1 << 20)));

//     Testing of getCompletionToStrongСonnectivityInСondensed     //
//     =======================================================     //
//  Some manual tests  //
//  -----------------  //
TEST(getCompletionToStrongСonnectivityInСondensed, manual) {
  vector<vector<bool>> matrix = {{0, 0, 0, 0, 0, 0},
  /*  0  1--->3           */     {0, 0, 0, 1, 0, 0},
  /*          ^           */     {0, 0, 0, 1, 1, 0},
  /*          |           */     {0, 0, 0, 0, 0, 0},
  /*          2--->4--->5 */     {0, 0, 0, 0, 0, 1},
                                 {0, 0, 0, 0, 0, 0}};
  Graph graph(buildSimpleAdjacencyMatrix(matrix));
  vector<pair<int,int>> completion;
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(3, completion.size());
  for (auto it = completion.begin(); it != completion.end(); ++it)
    matrix[it->first][it->second] = 1;
  Graph completed_graph(buildSimpleAdjacencyMatrix(matrix));
  Coloring components = getStronglyConnectedComponentsDummy(completed_graph);
  EXPECT_EQ(1, components.getNumberOfColors());
}
TEST(getCompletionToStrongСonnectivityInСondensed, death) {
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
        /* has loops */                    {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_DEATH({getCompletionToStrongСonnectivityInСondensed(graph1);}, "");
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 0, 0, 0},
                                           {0, 0, 0, 1},
      /*  0      1--->3<---2     */        {0, 0, 0, 1},
      /* more sources than sinks */        {0, 0, 0, 0}}));
  EXPECT_DEATH({getCompletionToStrongСonnectivityInСondensed(graph2);}, "");
}
TEST(getCompletionToStrongСonnectivityInСondensed, s1) {
  Graph graph(buildSimpleAdjacencyMatrix({{0}}));
  vector<pair<int,int>> completion;
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(0, completion.size());
}
TEST(getCompletionToStrongСonnectivityInСondensed, s2) {
  vector<pair<int,int>> completion;
  pair<int,int> e01(0, 1);
  pair<int,int> e10(1, 0);
  Graph graph(buildSimpleAdjacencyMatrix({{0, 0},
                                          {0, 0}})); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  sort(completion.begin(), completion.end());
  EXPECT_EQ(2, completion.size()); 
  EXPECT_EQ(e01, completion[0]);
  EXPECT_EQ(e10, completion[1]);

  graph = buildSimpleAdjacencyMatrix({{0, 1},
                                      {0, 0}}); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(1, completion.size()); 
  EXPECT_EQ(e10, completion[0]);

  graph = buildSimpleAdjacencyMatrix({{0, 0},
                                      {1, 0}}); 
  completion = getCompletionToStrongСonnectivityInСondensed(graph);
  EXPECT_EQ(1, completion.size()); 
  EXPECT_EQ(e01, completion[0]);
}
//  Testing on all valid graphs sized 3, 4, 5  //
//    and on all topsorted graphs sized 6, 7   //
//    --------------------------------------   //
void runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int> mask) {
  vector<vector<bool>> matrix = genMatrixByBoolMask(mask);
  Graph graph = buildSimpleAdjacencyMatrix(matrix);
  vector<int> source = getSource(graph);
  vector<int> sink = getSink(graph);
  vector<int> isolated = getIsolated(graph);
  if ((!hasLoop(graph)) && (source.size() <= sink.size())) {
    vector<pair<int,int>> completion;
    completion = getCompletionToStrongСonnectivityInСondensed(graph);
    for (auto it = completion.begin(); it != completion.end(); ++it) {
      matrix[(*it).first][(*it).second] = 1;
    }
    Graph completed = buildSimpleAdjacencyMatrix(matrix);
    Coloring components = getStronglyConnectedComponentsDummy(completed);
    ASSERT_EQ(isolated.size() + sink.size(), completion.size()) << mask;
    ASSERT_EQ(1, components.getNumberOfColors()) << mask;
  }
}
typedef ::testing::TestWithParam<int> StressNoSelfLoops;
TEST_P(StressNoSelfLoops, getCompletionToStrongСonnectivityInСondensed) {
  int size = GetParam();
  int maxmask = 1 << (size * (size - 1));
  for (int i = 0; i < maxmask; ++i)
    runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int>(size, i));
}
INSTANTIATE_TEST_CASE_P(func, StressNoSelfLoops, Range(3, 6));
typedef ::testing::TestWithParam<int> StressTopsorted;
TEST_P(StressTopsorted, getCompletionToStrongСonnectivityInСondensed) {
  int size = GetParam();
  int maxmask = 1 << (size * (size - 1) / 2);
  for (int i = 0; i < maxmask; ++i)
    runGetCompletionToStrongСonnectivityInСondensed(tuple<int,int>(size, i));
}
INSTANTIATE_TEST_CASE_P(func, StressTopsorted, Range(6, 8));