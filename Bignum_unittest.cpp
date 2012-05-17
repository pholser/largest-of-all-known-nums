#include "Bignum.h"
#include "gtest/gtest.h"
#include <vector>
#include <tr1/cstdint>

TEST(BignumTest, ABignumConstructedFromAnIntIsEqualToItself) {
    std::vector<uint32_t> digits;
    digits.push_back(2U);

    Bignum n(digits, 1);

    ASSERT_EQ(n, n);
}

TEST(BignumTest, BignumsConstructedFromTwoDifferentIntsAreNotEqual) {
    std::vector<uint32_t> digits;
    digits.push_back(5U);
    std::vector<uint32_t> other_digits;
    other_digits.push_back(3U);

    Bignum m(digits, 1);
    Bignum n(other_digits, -1);

    ASSERT_NE(m, n);
}

TEST(BignumTest, AddingTwoBignumsCanCreateMultipleBigDigits) {
    std::vector<uint32_t> digits;
    digits.push_back(0xFFFFFFFFU);
    std::vector<uint32_t> other_digits;
    other_digits.push_back(1U);

    Bignum m(digits, 1);
    Bignum n(other_digits, 1);

    std::vector<uint32_t> expected_digits;
    expected_digits.push_back(0U);
    expected_digits.push_back(1U);
    Bignum expected(expected_digits, 1);
    Bignum actual(m + n);

    ASSERT_EQ(expected, actual);
}

