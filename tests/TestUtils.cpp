#include <gtest/gtest.h>
#include "Utils.h"

TEST(UtilsTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(UtilsTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-2, -3), -5);
}