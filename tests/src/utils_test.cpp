#include <gtest/gtest.h>

extern "C" {
  #include <utils.h>
}

TEST(quickselect_t1, t1) {
  float arr[] = {1.2, 2.3, 4, 1.2};
  
  float median = quickselect(arr, 0, 3, 2);
  EXPECT_FLOAT_EQ(1.75, median);
};


TEST(sum_t1, t1) {
  int arr[] = {1, 2, 3, 4};
  
  int sum_n = sum(arr, 4);
  EXPECT_EQ(10, sum_n);
};

TEST(getMax_t1, t1) {
  float arr[] = {1.3, 2.1, -7.3, 1.2, 9.2};
  float max_n = getMax(arr, 5);
  EXPECT_FLOAT_EQ(9.2, max_n);
};

TEST(getMin_t1, t1) {
  float arr[] = {1.3, 2.1, -7.3, 1.2, 9.2};
  float min_n = getMin(arr, 5);
  EXPECT_FLOAT_EQ(-7.3, min_n);
};



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
