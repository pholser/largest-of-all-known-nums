#include "Bignum.h"
#include "gtest/gtest.h"
#include <vector>
#include <tr1/cstdint>
#include <cstdarg>

Bignum make_bignum(int signum, int num_digits, ...) {
    std::vector<uint32_t> digits;

    va_list args;
    va_start(args, num_digits);
    for (int i = 0; i < num_digits; ++i)
        digits.push_back(va_arg(args, uint32_t));
    va_end(args);
    return Bignum(digits, signum);
}

TEST(BignumTest, ABignumConstructedFromAnIntIsEqualToItself) {
    Bignum n = make_bignum(1, 1, 2U);

    ASSERT_EQ(n, n);
}

TEST(BignumTest, BignumsConstructedFromTwoDifferentIntsAreNotEqual) {
    Bignum m = make_bignum(1, 1, 5U);
    Bignum n = make_bignum(-1, 1, 3U);

    ASSERT_NE(m, n);
}

TEST(BignumTest, AddingTwoBignumsCanCreateMultipleBigDigits) {
    std::vector<uint32_t> digits;
    digits.push_back(0xFFFFFFFFU);
    std::vector<uint32_t> other_digits;
    other_digits.push_back(0x0000001U);

    Bignum m = make_bignum(1, 1, 0xFFFFFFFFU);
    Bignum n = make_bignum(1, 1, 0x00000001U);
    Bignum expected = make_bignum(1, 2, 0x00000000U, 0x00000001U);
    Bignum actual(m + n);

    ASSERT_EQ(expected, actual);
}
