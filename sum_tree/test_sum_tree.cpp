#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sum_tree.h>

namespace s = std;

class MockSumTree : public SumTree {
public:
  MockSumTree(uint size, float init_val): SumTree(size, init_val) {}

  using SumTree::nearest_power;
  using SumTree::leaf_idx_to_array_idx;
  using SumTree::array_idx_to_leaf_idx;
  using SumTree::mod_padding;
};

struct TestSumTree1 : ::testing::Test {
  TestSumTree1(): tree(10, 0.1F) {}
  ~TestSumTree1(){}

  MockSumTree tree;
};

TEST_F(TestSumTree1, TestMax1){
  EXPECT_DOUBLE_EQ(10 * 0.1F, tree.max());
}

TEST_F(TestSumTree1, TestSize1){
  EXPECT_EQ(10, tree.size());
}

TEST_F(TestSumTree1, TestSample1){
  uint sample_idx = tree.sample(0.F);
  EXPECT_EQ(0U, sample_idx);
}

TEST_F(TestSumTree1, TestSample2){
  uint sample_idx = tree.sample(0.1);
  EXPECT_EQ(1U, sample_idx);
}

TEST_F(TestSumTree1, TestSample3){
  uint sample_idx = tree.sample(0.1F * 7 + 0.0001);
  EXPECT_EQ(7U, sample_idx);
}

TEST_F(TestSumTree1, TestSample4){
  uint sample_idx = tree.sample(0.1F * 8 + 0.0001);
  EXPECT_EQ(8U, sample_idx);
}

TEST_F(TestSumTree1, TestSample5){
  uint sample_idx = tree.sample(0.1F * 2);
  EXPECT_EQ(2U, sample_idx);
}

TEST_F(TestSumTree1, TestSample6){
  uint sample_idx = tree.sample(0.1F * 3);
  EXPECT_EQ(3U, sample_idx);
}

TEST_F(TestSumTree1, TestSample7){
  uint sample_idx = tree.sample(0.1F * 5 + 0.0001); //numerical instability
  EXPECT_EQ(5U, sample_idx);
}

TEST_F(TestSumTree1, TestSample8){
  uint sample_idx = tree.sample(0.1F * 6);
  EXPECT_EQ(6U, sample_idx);
}

TEST_F(TestSumTree1, TestSample9){
  uint sample_idx = tree.sample(0.1F * 9);
  EXPECT_EQ(9U, sample_idx);
}

TEST_F(TestSumTree1, TestNearestPower1){
  uint power = tree.nearest_power(100);
  EXPECT_EQ(128U, power);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx1){
  uint array_idx = tree.leaf_idx_to_array_idx(15);
  EXPECT_EQ(0, array_idx);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx2){
  uint array_idx = tree.leaf_idx_to_array_idx(16);
  EXPECT_EQ(1, array_idx);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx3){
  uint array_idx = tree.leaf_idx_to_array_idx(18);
  EXPECT_EQ(3, array_idx);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx4){
  uint array_idx = tree.leaf_idx_to_array_idx(9);
  EXPECT_EQ(4, array_idx);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx5){
  uint array_idx = tree.leaf_idx_to_array_idx(10);
  EXPECT_EQ(5, array_idx);
}

TEST_F(TestSumTree1, TestLeafIdxToArrayIdx6){
  uint array_idx = tree.leaf_idx_to_array_idx(14);
  EXPECT_EQ(9, array_idx);
}

TEST_F(TestSumTree1, TestArrayIdxToLeafIdx1){
  uint leaf_idx = tree.array_idx_to_leaf_idx(0);
  EXPECT_EQ(15, leaf_idx);
}

TEST_F(TestSumTree1, TestArrayIdxToLeafIdx2){
  uint leaf_idx = tree.array_idx_to_leaf_idx(3);
  EXPECT_EQ(18, leaf_idx);
}

TEST_F(TestSumTree1, TestArrayIdxToLeafIdx3){
  uint leaf_idx = tree.array_idx_to_leaf_idx(4);
  EXPECT_EQ(9, leaf_idx);
}

TEST_F(TestSumTree1, TestUpdate1){
  tree.update(1, 0.2);
  EXPECT_EQ(10 * 0.1F + 0.1F, tree.max());
  EXPECT_EQ(10, tree.size());
  EXPECT_EQ(1, tree.sample(0.2));
  EXPECT_EQ(2, tree.sample(0.3));
}

TEST_F(TestSumTree1, TestUpdate2){
  tree.update(4, 0.3);
  EXPECT_EQ(10 * 0.1F + 0.2F, tree.max());
  EXPECT_EQ(10, tree.size());
  EXPECT_EQ(2, tree.sample(0.2F + 0.0001F));
  EXPECT_EQ(4, tree.sample(0.4F + 0.0001F));
  EXPECT_EQ(4, tree.sample(0.5F + 0.0001F));
  EXPECT_EQ(5, tree.sample(0.7F + 0.0001F));
}

TEST_F(TestSumTree1, TestModPadding){
  EXPECT_EQ(5U, tree.mod_padding());
}

struct TestSumTree2 : ::testing::Test {
  TestSumTree2(): tree(2000, 1.F) {}
  ~TestSumTree2(){}

  MockSumTree tree;
};

TEST_F(TestSumTree2, TestSize1){
  EXPECT_EQ(2000, tree.size());
}

struct TestSumTree3 : ::testing::Test {
  TestSumTree3(): tree(12, 1.F) {}
  ~TestSumTree3(){}

  MockSumTree tree;
};

TEST_F(TestSumTree3, TestModPadding){
  EXPECT_EQ(9U, tree.mod_padding());
}

struct TestSumTree4 : ::testing::Test {
  TestSumTree4(): tree(13, 1.F) {}
  ~TestSumTree4(){}

  MockSumTree tree;
};

TEST_F(TestSumTree4, TestModPadding){
  EXPECT_EQ(11U, tree.mod_padding());
}

struct TestSumTree5 : ::testing::Test {
  TestSumTree5(): tree(15, 1.F) {}
  ~TestSumTree5(){}

  MockSumTree tree;
};

TEST_F(TestSumTree5, TestModPadding){
  EXPECT_EQ(15U, tree.mod_padding());
}

struct TestSumTree6 : ::testing::Test {
  TestSumTree6(): tree(16, 1.F) {}
  ~TestSumTree6(){}

  MockSumTree tree;
};

TEST_F(TestSumTree6, TestModPadding){
  EXPECT_EQ(1U, tree.mod_padding());
}

struct TestSumTree7 : ::testing::Test {
  TestSumTree7(): tree(9, 1.F) {}
  ~TestSumTree7(){}

  MockSumTree tree;
};

TEST_F(TestSumTree7, TestModPadding){
  EXPECT_EQ(3U, tree.mod_padding());
}

