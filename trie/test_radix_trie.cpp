#include <radix_trie.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace s = std;

struct TestRadixTrie : ::testing::Test {
  TestRadixTrie(){}
  ~TestRadixTrie(){}

  RadixTrie trie;
};

TEST_F(TestRadixTrie, TestInsert){
  s::string e1 = "hello world";
  s::string e2 = "hello";
  s::string e3 = "bonjour";
  trie.insert(e1);

  EXPECT_FALSE(trie.exist(e2));
  EXPECT_FALSE(trie.exist(e3));
  EXPECT_TRUE(trie.exist(e1));

  trie.insert(e2);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_FALSE(trie.exist(e3));

  trie.insert(e3);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));

  trie.insert(e2);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));
}

TEST_F(TestRadixTrie, TestInsert2){
  s::string e1 = "hello world";
  s::string e2 = "hello";
  s::string e3 = "bonjour";
  trie.insert(e2);

  EXPECT_FALSE(trie.exist(e1));
  EXPECT_FALSE(trie.exist(e3));
  EXPECT_TRUE(trie.exist(e2));

  trie.insert(e1);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_FALSE(trie.exist(e3));

  trie.insert(e3);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));

  trie.insert(e2);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));
}

TEST_F(TestRadixTrie, TestInsert3){
  s::string e1 = "hello world";
  s::string e2 = "hello";
  s::string e3 = "hell yeah";
  trie.insert(e1);

  EXPECT_FALSE(trie.exist(e2));
  EXPECT_FALSE(trie.exist(e3));
  EXPECT_TRUE(trie.exist(e1));

  trie.insert(e2);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_FALSE(trie.exist(e3));

  trie.insert(e3);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));

  trie.insert(e2);

  EXPECT_TRUE(trie.exist(e1));
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));
}


TEST_F(TestRadixTrie, TestDelete){
  s::string e1 = "hello world";
  s::string e2 = "hello ";
  s::string e3 = "olla";

  trie.insert(e1);
  trie.insert(e2);
  trie.insert(e3);

  trie.remove(e1);
  EXPECT_FALSE(trie.exist(e1));
  trie.insert(e1);
  trie.remove(e2);
  EXPECT_FALSE(trie.exist(e2));
  trie.insert(e2);
  EXPECT_TRUE(trie.exist(e2));
  EXPECT_TRUE(trie.exist(e3));
}

TEST_F(TestRadixTrie, TestSize){
  s::string e1 = "hello world";
  s::string e2 = "hello ";
  s::string e3 = "olla";
  s::string e4 = "bonjour";
  s::string e5 = "gonbawa";

  trie.insert(e1);
  trie.insert(e2);
  trie.insert(e3);
  trie.insert(e4);
  trie.insert(e5);
  trie.insert(e3);
  trie.insert(e1);
  trie.insert(e2);

  EXPECT_EQ(5, trie.size());
}
