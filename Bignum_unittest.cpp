#include "Bignum.h"
#include "gtest/gtest.h"

TEST(BignumTest, ABignumConstructedFromAnIntIsEqualToItself) {
    Bignum n(2);
    ASSERT_EQ(n, n);
}

TEST(BignumTest, BignumsConstructedFromTwoDifferentIntsAreNotEqual) {
    Bignum m(5);
    Bignum n(-3);
    ASSERT_NE(m, n);
}
