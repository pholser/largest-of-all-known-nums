#include "Bignum.h"
#include "gtest/gtest.h"
#include <vector>
#include <tr1/cstdint>
#include <cstdarg>

std::vector<uint32_t> d(int num_digits, ...) {
    std::vector<uint32_t> digits;

    va_list args;
    va_start(args, num_digits);
    for (int i = 0; i < num_digits; ++i)
        digits.push_back(va_arg(args, uint32_t));
    va_end(args);

    return digits;
}

TEST(BignumTest, ABignumIsEqualToItself) {
    Bignum n(d(1, 2U), 1);

    ASSERT_EQ(n, n);
}

TEST(BignumTest, DistinctButEquivalentBignumsAreEqual) {
    Bignum m(d(1, 0x1234789DU), 1);
    Bignum n(d(1, 0x1234789DU), 1);

    ASSERT_EQ(m, n);
}

TEST(BignumTest, DistinctAndNotEquivalentBignumsAreNotEqual) {
    Bignum m(d(1, 5U), 1);
    Bignum n(d(1, 3U), -1);

    ASSERT_NE(m, n);
}

TEST(BignumTest, BignumsAreNotEqualIfTheirSignsAreDifferent) {
    Bignum m(d(1, 7U), 1);
    Bignum n(d(1, 7U), -1);

    ASSERT_NE(m, n);
}

TEST(BignumTest, SimpleAddition) {
    Bignum m(d(1, 1U), 1);
    Bignum n(d(1, 0U), 0);

    ASSERT_EQ(Bignum(d(1, 1U), 1), m + n);
}

TEST(BignumTest, SimpleAdditionWithAssignment) {
    Bignum m(d(1, 2U), 1);
    m += Bignum(d(1, 1U), 1);

    ASSERT_EQ(Bignum(d(1, 3U), 1), m);
}

TEST(BignumTest, LargerSimpleAddition) {
    Bignum m(d(1, 0x0FFFFFFFU), 1);
    Bignum n(d(1, 1U), 1);

    ASSERT_EQ(Bignum(d(1, 0x10000000U), 1), m + n);
}

TEST(BignumTest, AddingTwoBignumsCanCreateMultipleBigDigits) {
    Bignum m(d(1, 0xFFFFFFFFU), 1);
    Bignum n(d(1, 0x00000001U), 1);

    ASSERT_EQ(Bignum(d(2, 0x00000000U, 0x00000001U), 1), m + n);
}

TEST(BignumTest, StillLargerSimpleAddition) {
    Bignum m(d(2, 0xFFFFFFFFU, 0x00FFFFFFU), 1);
    Bignum n(d(1, 0x00000001U), 1);

    ASSERT_EQ(Bignum(d(2, 0x00000000U, 0x01000000U), 1), m + n);
}

TEST(BignumTest, LargePowerOfTwoSimpleAddition) {
    Bignum m(d(6, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1);
    Bignum n(d(1, 0x00000001U), 1);
    Bignum expected =
        Bignum(d(6, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1);

    ASSERT_EQ(expected, m + n);
}

TEST(BignumTest, NegativeNumberLessThanPositiveNumber) {
    Bignum m(d(1, 1U), -1);
    Bignum n(d(1, 2U), 1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, NegativeNumberOfGreaterMagnitudeLessThanNegativeNumberOfLesserMagnitude) {
    Bignum m(d(1, 3U), -1);
    Bignum n(d(1, 2U), -1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, NegativeNumberOfGreaterMagnitudeLessThanNegativeNumberOfLesserMagnitudeLarge) {
    Bignum m(d(3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x12345679U), -1);
    Bignum n(d(3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x12345678U), -1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, NegativeNumberOfGreaterMagnitudeLessThanPositiveNumberOfLesserMagnitudeLarge) {
    Bignum m(d(3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x12345679U), -1);
    Bignum n(d(3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0x12345678U), 1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, PositiveNumberOfLesserMagnitudeLessThanPositiveNumberOfGreaterMagnitude) {
    Bignum m(d(1, 3U), 1);
    Bignum n(d(1, 4U), 1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, PositiveNumberOfLesserMagnitudeLessThanPositiveNumberOfGreaterMagnitudeLarge) {
    Bignum m(d(4, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000001U), 1);
    Bignum n(d(4, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000002U), 1);

    ASSERT_LT(m, n);
}

TEST(BignumTest, NegativeNumberOfLesserMagnitudeLessThanPositiveNumberOfGreaterMagnitudeLarge) {
    Bignum m(d(4, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000001U), -1);
    Bignum n(d(4, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000002U), 1);

    ASSERT_LT(m, n);
}
