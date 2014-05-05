#include "gtest/gtest.h"
#include "graph.h"
#include "helpers.h"

//  Algorithm  //
//  =========  //
TEST(helpersFunc, isPath) {
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 1},      //0->1
                                           {0, 0}})); 
  EXPECT_TRUE(isPath(graph2, 0, 1));
  EXPECT_FALSE(isPath(graph2, 1, 0));

  Graph graph4(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},      // 0-->1
                                           {0, 0, 1, 0},      // ^   |
                                           {0, 0, 0, 1},      // |   v  
                                           {1, 0, 0, 0}}));   // 3<--2
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_TRUE(isPath(graph4, i, j));
}
TEST(helpersFunc, getStronglyConnectedComponentsDummy) {
  Graph graph(buildSimpleAdjacencyMatrix({{0, 1, 0, 0, 0},
     /* 0 <-- 2 <-    */                  {0, 0, 0, 0, 0},
     /* |     |   \   */                  {1, 0, 0, 1, 0},
     /* v     v    \  */                  {0, 1, 0, 0, 1},
     /* 1 <-- 3 --> 4 */                  {0, 0, 1, 0, 0}}));  
  Coloring components = getStronglyConnectedComponentsDummy(graph);
  //expected division into components: 0; 1; 2 + 3 + 4 
  EXPECT_EQ(3, components.getNumberOfColors());
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(3));
  EXPECT_EQ(components.getColorOf(2), components.getColorOf(4));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(1));
  EXPECT_FALSE(components.getColorOf(2) == components.getColorOf(0));
  EXPECT_FALSE(components.getColorOf(1) == components.getColorOf(0));
}
TEST(helpersFunc, hasSelfLoop) {
  Graph graph0(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
                                           {1, 1, 0, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_FALSE(hasSelfLoop(graph0));
  Graph graph1(buildSimpleAdjacencyMatrix({{0, 1, 0, 0},
                                           {1, 0, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_TRUE(hasSelfLoop(graph1));
  Graph graph2(buildSimpleAdjacencyMatrix({{0, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 0}}));
  EXPECT_TRUE(hasSelfLoop(graph2));
}

//  Generators  //
//  ==========  //
TEST(helpersGen, genMatrixByBoolMask) {
  tuple<int,int> mask_from_comments_to_source(4, 749);
  vector<vector<bool>> expected_matix = {{0, 1, 0, 1},
                                         {1, 0, 1, 0},
                                         {1, 0, 0, 1},
                                         {1, 0, 0, 0}};
  EXPECT_EQ(expected_matix, genMatrixByBoolMask(mask_from_comments_to_source));
}