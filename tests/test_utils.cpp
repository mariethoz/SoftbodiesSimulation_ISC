#include <gtest/gtest.h>
#include "utils.h"

TEST(UtilsTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(UtilsTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-2, -3), -5);
}