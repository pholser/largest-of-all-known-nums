#include "Bignum.h"
#include "gtest/gtest.h"
#include <vector>
#include <tr1/cstdint>

TEST(BignumTest, ABignumConstructedFromAnIntIsEqualToItself) {
    Bignum n(2);
    ASSERT_EQ(n, n);
}

TEST(BignumTest, BignumsConstructedFromTwoDifferentIntsAreNotEqual) {
    Bignum m(5);
    Bignum n(-3);
    ASSERT_NE(m, n);
}

TEST(BignumTest, AddingTwoBignumsCanCreateMultipleBigDigits) {
    Bignum m(0xFFFFFFFF);
    Bignum n(1);

    std::vector<uint32_t> expected_digits;
    expected_digits.push_back(0x00000000);
    expected_digits.push_back(0x00000001);
    Bignum expected(expected_digits, 1);
    ASSERT_EQ(expected, m + n);
}