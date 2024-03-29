#include "Bignum.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

const uint64_t Bignum::BASE = UINT32_MAX + 1ULL;
const std::size_t Bignum::BITS_IN_DIGIT = sizeof(uint32_t) * CHAR_BIT;

uint64_t digit(const std::deque<uint32_t>& digits, std::deque<uint32_t>::size_type index) {
    return index >= digits.size() ? 0U : digits[index];
}

void strip_leading_zeros(std::deque<uint32_t>& digits) {
    for (std::deque<uint32_t>::reverse_iterator i = digits.rbegin(); *i == 0U && digits.size() > 1;)
        digits.erase((++i).base());
}

std::deque<uint32_t> add(const std::deque<uint32_t>& first, const std::deque<uint32_t>& second) {
    std::deque<uint32_t>::size_type max_length = std::max(first.size(), second.size());
    uint64_t carry(0);
    std::deque<uint32_t> sum_digits;

    for (std::deque<uint32_t>::size_type i = 0; i < max_length; ++i) {
        uint64_t sum = digit(first, i) + digit(second, i) + carry;
        sum_digits.push_back(sum % Bignum::BASE);
        carry = sum / Bignum::BASE;
    }

    if (carry != 0)
        sum_digits.push_back(carry);

    strip_leading_zeros(sum_digits);
    return sum_digits;
}

std::deque<uint32_t> subtract(const std::deque<uint32_t>& first, const std::deque<uint32_t>& second) {
    std::deque<uint32_t>::size_type max_length = std::max(first.size(), second.size());
    int64_t borrow(0);
    std::deque<uint32_t> difference_digits;

    for (std::deque<uint32_t>::size_type i = 0; i < max_length; ++i) {
        int64_t difference = digit(first, i) - digit(second, i) - borrow;
        if (difference < 0) {
            difference += Bignum::BASE;
            borrow = 1;
        } else
            borrow = 0;

        difference_digits.push_back(difference % Bignum::BASE);
    }

    strip_leading_zeros(difference_digits);
    return difference_digits;
}

Bignum::Bignum(const int64_t value) : store() {
    sign = value < 0 ? -1 : (value == 0 ? 0 : 1);
    uint64_t no_sign = ::abs(value);
    store.push_back((uint32_t) (no_sign & 0x00000000FFFFFFFFULL));
    store.push_back((uint32_t) (no_sign >> 32));
    strip_leading_zeros(store);
}

Bignum::Bignum(const std::deque<uint32_t>& digits, const int sign)
    : store(digits), sign(sign) {
}

Bignum::Bignum(const Bignum& other)
    : store(other.store), sign(other.sign) {
}

Bignum& Bignum::operator=(const Bignum& other) {
    if (this != &other) {
        store = other.store;
        sign = other.sign;
    }

    return *this;
}

bool Bignum::equal(const Bignum& other) const {
    return store == other.store && sign == other.sign;
}

bool Bignum::less(const Bignum& other) const {
    if (sign < other.sign)
        return true;

    if (store.size() < other.store.size())
        return true;

    for (int i = store.size() - 1; i >= 0; --i) {
        if (sign < 0 ? store[i] > other.store[i] : store[i] < other.store[i])
            return true;
    }

    return false;
}

int Bignum::signum() const {
    return sign;
}

Bignum Bignum::abs() const {
    return sign >= 0 ? Bignum(*this) : Bignum(store, 1);
}

bool operator==(const Bignum& left, const Bignum& right) {
    return left.equal(right);
}

bool operator!=(const Bignum& left, const Bignum& right) {
    return !(left == right);
}

bool operator<(const Bignum& left, const Bignum& right) {
    return left.less(right);
}

bool operator<=(const Bignum& left, const Bignum& right) {
    return !(left > right);
}

bool operator>(const Bignum& left, const Bignum& right) {
    return right < left;
}

bool operator>=(const Bignum& left, const Bignum& right) {
    return right <= left;
}

Bignum operator+(const Bignum& left, const Bignum& right) {
    return Bignum(left) += right;
}

const Bignum& Bignum::operator+=(const Bignum& other) {
    if (other.sign == 0)
        return *this;

    Bignum this_abs = abs();
    Bignum other_abs = other.abs();

    if (sign == other.sign)
        store = add(this_abs.store, other_abs.store);
    else {
        if (this_abs > other_abs)
            store = subtract(this_abs.store, other_abs.store);
        else {
            store = subtract(other_abs.store, this_abs.store);
            sign = other.sign;
        }
    }

    reconcile_sign_of_zero();

    return *this;
}

Bignum operator-(const Bignum& left, const Bignum& right) {
    return Bignum(left) -= right;
}

const Bignum& Bignum::operator-=(const Bignum& other) {
    if (other.sign == 0)
        return *this;

    Bignum this_abs = abs();
    Bignum other_abs = other.abs();

    if (sign != other.sign)
        store = add(this_abs.store, other_abs.store);
    else {
        if (this_abs > other_abs)
            store = subtract(this_abs.store, other_abs.store);
        else {
            store = subtract(other_abs.store, this_abs.store);
            sign = -sign;
        }
    }

    reconcile_sign_of_zero();

    return *this;
}

Bignum operator>>(const Bignum& n, const unsigned int increment) {
    return Bignum(n) >>= increment;
}

const Bignum& Bignum::operator>>=(const unsigned int increment) {
    uint32_t leading(0);
    uint32_t trailing(0);

    store.erase(store.begin(), store.begin() + (increment / Bignum::BITS_IN_DIGIT));

    unsigned int shift_amount = increment % Bignum::BITS_IN_DIGIT;
    for (std::deque<uint32_t>::reverse_iterator i = store.rbegin(); i != store.rend(); ++i) {
        trailing = *i & ((1 << shift_amount) - 1);
        *i >>= shift_amount;
        *i |= leading;
        leading = trailing << (Bignum::BITS_IN_DIGIT - shift_amount);
    }

    reconcile_sign_of_zero();

    return *this;
}

Bignum operator<<(const Bignum& n, const unsigned int increment) {
    return Bignum(n) <<= increment;
}

const Bignum& Bignum::operator<<=(const unsigned int increment) {
    uint32_t leading(0);
    uint32_t trailing(0);

    unsigned int number_of_trailing_zeros = increment / Bignum::BITS_IN_DIGIT;

    unsigned int shift_amount = increment % Bignum::BITS_IN_DIGIT;
    if (shift_amount > 0) {
        for (std::deque<uint32_t>::iterator i = store.begin(); i != store.end(); ++i) {
            leading = *i & ~((1 << (Bignum::BITS_IN_DIGIT - shift_amount)) - 1);
            *i <<= shift_amount;
            *i |= trailing;
            trailing = leading >> (Bignum::BITS_IN_DIGIT - shift_amount);
        }
    }

    if (trailing != 0)
        store.push_back(trailing);

    for (unsigned int i = 0; i < number_of_trailing_zeros; ++i)
        store.push_front(0x0U);

    reconcile_sign_of_zero();

    return *this;
}

Bignum Bignum::operator-() const {
    return Bignum(store, -sign);
}

Bignum& operator++(Bignum& n) {
    n += 1;
    return n;
}

Bignum operator++(Bignum& n, int) {
    Bignum old(n);
    ++n;
    return old;
}

Bignum& operator--(Bignum& n) {
    n -= 1;
    return n;
}

Bignum operator--(Bignum& n, int) {
    Bignum old(n);
    --n;
    return old;
}

void Bignum::reconcile_sign_of_zero() {
    if (store.size() == 1 && store[0] == 0U)
        sign = 0;
}

std::ostream& operator<<(std::ostream& out, const Bignum& n) {
    out << "sign: " << n.sign << std::endl;
    out << "digits: " << std::endl;

    for (std::deque<uint32_t>::size_type i = 0; i != n.store.size(); ++i)
        out << i << ": " << std::hex << n.store[i] << std::endl;

    return out;
}
