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

TEST(BignumTest, ConstructingABignumFromAPositiveInt) {
    Bignum n(123456);

    ASSERT_EQ(Bignum(d(1, 0x0001E240U), 1), n);
}

TEST(BignumTest, ConstructingABignumFromANegativeInt) {
    Bignum n(-345);

    ASSERT_EQ(Bignum(d(1, 0x00000159), -1), n);
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

TEST(BignumTest, SignumOfNegative) {
    Bignum n(d(1, 3U), -1);

    ASSERT_EQ(-1, n.signum());
}

TEST(BignumTest, SignumOfZero) {
    Bignum n(d(1, 0U), 0);

    ASSERT_EQ(0, n.signum());
}

TEST(BignumTest, SignumOfPositive) {
    Bignum n(d(1, 7U), 1);

    ASSERT_EQ(1, n.signum());
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

TEST(BignumTest, LargerPositivePlusSmallerNegative) {
    Bignum m(d(1, 3U), 1);
    Bignum n(d(1, 2U), -1);

    ASSERT_EQ(Bignum(d(1, 1U), 1), m + n);
}

TEST(BignumTest, LargerPositivePlusSmallerNegativeWithAssignment) {
    Bignum m(d(1, 4U), 1);
    m += Bignum(d(1, 3U), -1);

    ASSERT_EQ(Bignum(d(1, 1U), 1), m);
}

TEST(BignumTest, LargerNegativePlusSmallerPositive) {
    Bignum m(d(1, 5U), -1);
    Bignum n(d(1, 2U), 1);

    ASSERT_EQ(Bignum(d(1, 3U), -1), m + n);
}

TEST(BignumTest, LargerNegativePlusSmallerPositiveWithAssignment) {
    Bignum m(d(1, 6U), -1);
    m += Bignum(d(1, 2U), 1);

    ASSERT_EQ(Bignum(d(1, 4U), -1), m);
}

TEST(BignumTest, NegativePlusNegative) {
    Bignum m(d(1, 3U), -1);
    Bignum n(d(1, 2U), -1);

    ASSERT_EQ(Bignum(d(1, 5U), -1), m + n);
}

TEST(BignumTest, NegativePlusNegativeWithAssignment) {
    Bignum m(d(1, 4U), -1);
    m += Bignum(d(1, 3U), -1);

    ASSERT_EQ(Bignum(d(1, 7U), -1), m);
}

TEST(BignumTest, PositivePlusNegativeOfSameMagnitude) {
    Bignum m(d(2, 0x12341234U, 0x56785678U), 1);
    m += Bignum(d(2, 0x12341234U, 0x56785678U), -1);

    ASSERT_EQ(Bignum(d(1, 0U), 0), m);
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

TEST(BignumTest, NegationOfPositiveNumber) {
    Bignum n(d(1, 8U), 1);

    ASSERT_EQ(Bignum(d(1, 8U), -1), -n);
}

TEST(BignumTest, NegationOfNegativeNumber) {
    Bignum n(d(1, 36U), -1);

    ASSERT_EQ(Bignum(d(1, 36U), 1), -n);
}

TEST(BignumTest, NegationOfZero) {
    Bignum n(d(1, 0U), 0);

    ASSERT_EQ(Bignum(d(1, 0U), 0), -n);
}

TEST(BignumTest, SimpleSubtraction) {
    Bignum m(d(1, 1U), 1);
    Bignum n(d(1, 0U), 0);

    ASSERT_EQ(Bignum(d(1, 1U), 1), m - n);
}

TEST(BignumTest, SimpleSubtractionWithAssignment) {
    Bignum m(d(1, 5U), 1);
    m -= Bignum(d(1, 3U), 1);

    ASSERT_EQ(Bignum(d(1, 2U), 1), m);
}

TEST(BignumTest, PositiveMinusLargerPositive) {
    Bignum m(d(1, 5U), 1);
    Bignum n(d(1, 7U), 1);

    ASSERT_EQ(Bignum(d(1, 2U), -1), m - n);
}

TEST(BignumTest, PositiveMinusLargerPositiveWithAssignment) {
    Bignum m(d(1, 8U), 1);
    m -= Bignum(d(1, 13U), 1);

    ASSERT_EQ(Bignum(d(1, 5U), -1), m);
}

TEST(BignumTest, NegativeMinusPositive) {
    Bignum m(d(1, 5U), -1);
    Bignum n(d(1, 7U), 1);

    ASSERT_EQ(Bignum(d(1, 12U), -1), m - n);
}

TEST(BignumTest, NegativeMinusPositiveWithAssignment) {
    Bignum m(d(1, 5U), -1);
    m -= Bignum(d(1, 35U), 1);

    ASSERT_EQ(Bignum(d(1, 40U), -1), m);
}

TEST(BignumTest, NegativeMinusSmallerNegative) {
    Bignum m(d(1, 5U), -1);
    Bignum n(d(1, 3U), -1);

    ASSERT_EQ(Bignum(d(1, 2U), -1), m - n);
}

TEST(BignumTest, NegativeMinusSmallerNegativeWithAssignment) {
    Bignum m(d(1, 5U), -1);
    m -= Bignum(d(1, 4U), -1);

    ASSERT_EQ(Bignum(d(1, 1U), -1), m);
}

TEST(BignumTest, NegativeMinusLargerNegative) {
    Bignum m(d(1, 5U), -1);
    Bignum n(d(1, 42U), -1);

    ASSERT_EQ(Bignum(d(1, 37U), 1), m - n);
}

TEST(BignumTest, NegativeMinusLargerNegativeWithAssignment) {
    Bignum m(d(1, 17U), -1);
    m -= Bignum(d(1, 73U), -1);

    ASSERT_EQ(Bignum(d(1, 56U), 1), m);
}

TEST(BignumTest, LargerSimpleSubtraction) {
    Bignum m(d(1, 0x10000000U), 1);
    Bignum n(d(1, 1U), 1);

    ASSERT_EQ(Bignum(d(1, 0x0FFFFFFFU), 1), m - n);
}

TEST(BignumTest, StillLargerSimpleSubtraction) {
    Bignum m(d(2, 0x00000000U, 0x01000000), 1);
    Bignum n(d(1, 1U), 1);

    ASSERT_EQ(Bignum(d(2, 0xFFFFFFFFU, 0x00FFFFFFU), 1), m - n);
}

TEST(BignumTest, EnormousSimpleSubtraction) {
    Bignum m(d(6, 0x00000001U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1);
    Bignum n(d(1, 1U), 1);

    ASSERT_EQ(
        Bignum(d(6, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1),
        m - n);
}

TEST(BignumTest, LargeSubtractionSmallResult) {
    Bignum m(d(2, 0x00000001U, 0x00010000U), 1);
    Bignum n(d(2, 0x00000000U, 0x00010000U), 1);

    ASSERT_EQ(Bignum(d(1, 0x00000001U), 1), m - n);
}

TEST(BignumTest, LargeSubtractionMediumResult) {
    Bignum m(d(2, 0x0FFFFFFFU, 0x00010000U), 1);
    Bignum n(d(2, 0x00000000U, 0x00010000U), 1);

    ASSERT_EQ(Bignum(d(1, 0x0FFFFFFFU), 1), m - n);
}

TEST(BignumTest, EnormousSubtractionMediumResult) {
    Bignum m(d(6, 0x00000000U, 0x00010000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1);
    Bignum n(d(2, 0x00000000U, 0x00010000U), 1);

    ASSERT_EQ(
        Bignum(d(6, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00001000U), 1),
        m - n);
}

TEST(BignumTest, AnotherEnormousSubtractionMediumResult) {
    Bignum m(d(6, 0xFFFFFFFFU, 0x0000FFFFU, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000100U), 1);
    Bignum n(d(2, 0x00000000U, 0x00010000U), 1);

    ASSERT_EQ(
        Bignum(d(6, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFF, 0xFFFFFFFFU, 0x000000FFU), 1),
        m - n);
}

TEST(BignumTest, PositiveMinusPositiveOfSameMagnitude) {
    Bignum m(d(3, 0x1U, 0x2U, 0x3U), 1);
    m -= Bignum(d(3, 0x1U, 0x2U, 0x3U), 1);

    ASSERT_EQ(Bignum(d(1, 0U), 0), m);
}

TEST(BignumTest, NegativeMinusNegativeOfSameMagnitude) {
    Bignum m(d(3, 0x4U, 0x5U, 0x6U), -1);
    m -= Bignum(d(3, 0x4U, 0x5U, 0x6U), -1);

    ASSERT_EQ(Bignum(d(1, 0U), 0), m);
}

TEST(BignumTest, AbsoluteValueOfNegativeNumber) {
    Bignum m(d(4, 0x01234567U, 0x89ABCDEFU, 0x12345678U, 0x9ABCDEF0U), -1);

    ASSERT_EQ(Bignum(d(4, 0x01234567U, 0x89ABCDEFU, 0x12345678U, 0x9ABCDEF0U), 1), m.abs());
}

TEST(BignumTest, AbsoluteValueOfPositiveNumber) {
    Bignum n(d(1, 1U), 1);

    ASSERT_EQ(Bignum(d(1, 1U), 1), n.abs());
}

TEST(BignumTest, AbsoluteValueOfZero) {
    Bignum n(d(1, 0U), 0);

    ASSERT_EQ(Bignum(d(1, 0U), 0), n.abs());
}

TEST(BignumTest, PrefixIncrement) {
    Bignum m(-34);
    Bignum n = ++m;

    ASSERT_EQ(Bignum(-33), m);
    ASSERT_EQ(Bignum(-33), n);
}

TEST(BignumTest, PostfixIncrement) {
    Bignum m(-67);
    Bignum n = m++;

    ASSERT_EQ(Bignum(-66), m);
    ASSERT_EQ(Bignum(-67), n);
}

TEST(BignumTest, PrefixDecrement) {
    Bignum m(-4);
    Bignum n = --m;

    ASSERT_EQ(Bignum(-5), m);
    ASSERT_EQ(Bignum(-5), n);
}

TEST(BignumTest, PostfixDecrement) {
    Bignum m(88);
    Bignum n = m--;

    ASSERT_EQ(Bignum(87), m);
    ASSERT_EQ(Bignum(88), n);
}

TEST(BignumTest, RightShiftSingleDigitByOne) {
    Bignum m(d(1, 0x12345678U), 1);
    Bignum n(m >> 1);

    ASSERT_EQ(Bignum(d(1, 0x091A2B3CU), 1), n);
}

TEST(BignumTest, RightShiftSingleDigitByOneWithAssignment) {
    Bignum m(d(1, 0x1234567AU), 1);
    m >>= 1;

    ASSERT_EQ(Bignum(d(1, 0x091A2B3DU), 1), m);
}

TEST(BignumTest, RightShiftTheNumberOneByOne) {
    Bignum m(d(1, 0x1U), 1);
    Bignum n(m >> 1);

    ASSERT_EQ(Bignum(d(1, 0x0U), 0), n);
}

TEST(BignumTest, RightShiftNegativeSingleDigitByOne) {
    Bignum m(d(1, 0xFEDCBA98U), -1);
    Bignum n(m >> 1);

    ASSERT_EQ(Bignum(d(1, 0x7F6E5D4CU), -1), n);
}

TEST(BignumTest, RightShiftNegativeSingleDigitByOneWithAssignment) {
    Bignum m(d(1, 0xBEEFFACEU), -1);
    m >>= 1;

    ASSERT_EQ(Bignum(d(1, 0x5F77FD67U), -1), m);
}

TEST(BignumTest, RightShiftNegativeOneByOne) {
    Bignum m(d(1, 0x1U), -1);
    Bignum n(m >> 1);

    ASSERT_EQ(Bignum(d(1, 0x0U), 0), n);
}

TEST(BignumTest, RightShiftMultipleDigits) {
    Bignum m(d(3, 0x01234567U, 0x89ABCDEFU, 0x78784949U), 1);
    Bignum n(m >> 7);

    ASSERT_EQ(Bignum(d(3, 0xDE02468AU, 0x9313579BU, 0x00F0F092U), 1), n);
}

TEST(BignumTest, RightShiftMultipleDigitsWithAssignment) {
    Bignum m(d(3, 0x00000034U, 0xC08905EAU, 0x4892BEADU), 1);
    m >>= 23;

    ASSERT_EQ(Bignum(d(3, 0x120BD400U, 0x257D5B81U, 0x00000091U), 1), m);
}
