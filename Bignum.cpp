#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "Bignum.h"

const uint64_t Bignum::BASE = UINT32_MAX + 1ULL;

uint64_t digit(const std::vector<uint32_t>& digits, std::vector<uint32_t>::size_type index) {
    return index >= digits.size() ? 0U : digits[index];
}

std::vector<uint32_t> add(const std::vector<uint32_t>& first, const std::vector<uint32_t>& second) {
    std::vector<uint32_t>::size_type max_length = std::max(first.size(), second.size());
    uint64_t carry(0);
    std::vector<uint32_t> sum_digits;

    for (std::vector<uint32_t>::size_type i = 0; i < max_length; ++i) {
        uint64_t sum = digit(first, i) + digit(second, i) + carry;
        sum_digits.push_back(sum % Bignum::BASE);
        carry = sum / Bignum::BASE;
    }

    if (carry != 0)
        sum_digits.push_back(carry);

    return sum_digits;
}

std::vector<uint32_t> subtract(const std::vector<uint32_t>& first, const std::vector<uint32_t>& second) {
    std::vector<uint32_t>::size_type max_length = std::max(first.size(), second.size());
    int64_t borrow(0);
    std::vector<uint32_t> difference_digits;

    for (std::vector<uint32_t>::size_type i = 0; i < max_length; ++i) {
        int64_t difference = digit(first, i) - digit(second, i) - borrow;
        if (difference < 0) {
            difference += Bignum::BASE;
            borrow = 1;
        } else
            borrow = 0;

        difference_digits.push_back(difference % Bignum::BASE);
    }

    return difference_digits;
}

Bignum::Bignum(const std::vector<uint32_t>& digits, int sign)
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
    if (other.sign == 0) {
        return *this;
    }

    Bignum this_abs = abs();
    Bignum other_abs = other.abs();

    if (sign == other.sign) {
        store = add(this_abs.store, other_abs.store);
    } else {
        if (this_abs > other_abs)
            store = subtract(this_abs.store, other_abs.store);
        else {
            store = subtract(other_abs.store, this_abs.store);
            sign = other.sign;
        }
    }

    return *this;
}

const Bignum& Bignum::operator-=(const Bignum& other) {
    if (other.sign == 0) {
        return *this;
    }

    Bignum this_abs = abs();
    Bignum other_abs = other.abs();

    if (sign != other.sign) {
        store = add(this_abs.store, other_abs.store);
    } else {
        if (this_abs > other_abs)
            store = subtract(this_abs.store, other_abs.store);
        else {
            store = subtract(other_abs.store, this_abs.store);
            sign = -sign;
        }
    }

    return *this;
}

Bignum operator-(const Bignum& left, const Bignum& right) {
    return Bignum(left) -= right;
}

Bignum Bignum::operator-() const {
    return Bignum(store, -sign);
}

std::ostream& operator<<(std::ostream& out, const Bignum& n) {
    out << "sign: " << n.sign << std::endl;
    out << "digits: " << std::endl;

    for (std::vector<uint32_t>::size_type i = 0; i != n.store.size(); ++i)
        out << i << ": " << std::hex << n.store[i] << std::endl;

    return out;
}
