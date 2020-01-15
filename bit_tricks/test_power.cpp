#include <gtest/gtest.h>

#include <power.h>

TEST(power, TestPower1){
  unsigned h = nearest_power_2(10);
  EXPECT_EQ(16, h);
}

TEST(power, TestPower2){
  unsigned h = nearest_power_2(16);
  EXPECT_EQ(16, h);
}

TEST(power, TestPower3){
  unsigned h = nearest_power_2(1);
  EXPECT_EQ(1, h);
}

TEST(power, TestPower4){
  unsigned h = nearest_power_2(0);
  EXPECT_EQ(0, h);
}

TEST(power, TestPower5){
  unsigned h = nearest_power_2(255);
  EXPECT_EQ(256, h);
}

TEST(power, TestPower6){
  unsigned h = nearest_power_2(2);
  EXPECT_EQ(2, h);
}

TEST(power, TestPower7){
  unsigned h = nearest_power_2(5);
  EXPECT_EQ(8, h);
}

TEST(power, TestPower8){
  unsigned h = nearest_power_2(2000);
  EXPECT_EQ(2048, h);
}
