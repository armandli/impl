#include <skip_list.h>
#include <iostream>

#include <gtest/gtest.h>

class skip_list1_tester: public skip_list1<double> {
public:
  size_t test_random_level(){
    return random_level();
  }
  Node* test_search(const double& v){
    return search(v);
  }

  using Node = Node;
};

struct TestRandomLevelGeneration1 : testing::Test {
  TestRandomLevelGeneration1() = default;
  ~TestRandomLevelGeneration1() = default;

  skip_list1_tester tester;
};

TEST_F(TestRandomLevelGeneration1, test1){
  std::vector<size_t> levels;
  for (size_t i = 0; i < 100000; ++i)
    levels.push_back(tester.test_random_level());

  size_t count_0 = 0, count_1 = 0, count_2 = 0, count_o = 0;
  for (size_t v : levels)
    switch (v){
    case 0: count_0++; break;
    case 1: count_1++; break;
    case 2: count_2++; break;
    default: count_o++; break;
    }

  std::cout << "count 0: " << count_0 << " 1: " << count_1 << " 2: " << count_2 << std::endl;

  EXPECT_TRUE(count_0 > count_1);
  EXPECT_TRUE(count_1 > count_2);
  EXPECT_TRUE(count_0 > count_o);
  EXPECT_TRUE(count_1 + count_2 < count_0);
}

struct TestEmptyTree1: testing::Test {
  TestEmptyTree1() = default;
  ~TestEmptyTree1(){
    list.clear();
  }

  skip_list1_tester list;
};

TEST_F(TestEmptyTree1, testSearch1){
  EXPECT_EQ(0, list.size());

  list.insert(1917.);
  skip_list1_tester::Node* node = list.test_search(1917.);
  ASSERT_TRUE(node != nullptr);
  EXPECT_EQ(1917., node->v);
}

TEST_F(TestEmptyTree1, testSearch2){
  EXPECT_EQ(0, list.size());

  list.insert(22232.991);
  list.insert(1999.);
  list.insert(2000.);
  list.insert(2001.);
  skip_list1_tester::Node* node1 = list.test_search(1999.);
  EXPECT_TRUE(node1 != nullptr);
  EXPECT_EQ(1999., node1->v);
}

TEST_F(TestEmptyTree1, testSearch3){
  EXPECT_EQ(0, list.size());

  list.insert(22232.991);
  list.insert(1999.);
  list.insert(1999.);
  list.insert(1999.);
  skip_list1_tester::Node* node1 = list.test_search(1999.);
  EXPECT_TRUE(node1 != nullptr);
  EXPECT_EQ(1999., node1->v);
}

TEST_F(TestEmptyTree1, testEmpty1){
  EXPECT_EQ(0, list.size());

  list.insert(12.);
  EXPECT_TRUE(not list.empty());

  list.insert(18.);
  EXPECT_TRUE(not list.empty());

  list.erase(18.);
  list.erase(12.);
  EXPECT_TRUE(list.empty());
}

TEST_F(TestEmptyTree1, testSize1){
  list.insert(111111.);
  EXPECT_EQ(1, list.size());
  list.erase(111111.);
  EXPECT_EQ(0, list.size());
  list.insert(1020304050.);
  list.insert(121.);
  list.insert(2e-17);
  list.insert(19937.0);
  EXPECT_EQ(4, list.size());
}

TEST_F(TestEmptyTree1, testInsert){
  list.insert(16.779);
  list.insert(21221.99);
  list.insert(1.234567);
  list.insert(2e21);

  EXPECT_TRUE(list.find(16.779) != list.end());
  EXPECT_TRUE(list.find(1.234567) != list.end());
  EXPECT_TRUE(list.find(17.779) == list.end());
  EXPECT_TRUE(list.find(2e21) != list.end());
}
