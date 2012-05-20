#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "Bignum.h"

const uint64_t Bignum::BASE = UINT32_MAX + 1ULL;

uint64_t digit(const std::vector<uint32_t>& digits, std::vector<uint32_t>::size_type index) {
    return index >= digits.size() ? 0U : digits[index];
}

Bignum::Bignum(const std::vector<uint32_t>& digits, int sign)
    : store(digits), signum(sign) {
}

Bignum::Bignum(const Bignum& other)
    : store(other.store), signum(other.signum) {
}

Bignum& Bignum::operator=(const Bignum& other) {
    if (this != &other) {
        store = other.store;
        signum = other.signum;
    }

    return *this;
}

bool Bignum::equal(const Bignum& other) const {
    return store == other.store && signum == other.signum;
}

bool Bignum::less(const Bignum& other) const {
    if (signum < other.signum)
        return true;

    if (store.size() < other.store.size())
        return true;

    for (int i = store.size() - 1; i >= 0; --i) {
        if (signum < 0 ? store[i] > other.store[i] : store[i] < other.store[i])
            return true;
    }

    return false;
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
    uint64_t carry(0);
    std::vector<uint32_t>::size_type max_length = std::max(store.size(), other.store.size());

    std::vector<uint32_t> new_digits;

    for (std::vector<uint32_t>::size_type i = 0; i < max_length; ++i) {
        uint64_t sum = digit(store, i) + digit(other.store, i) + carry;
        new_digits.push_back(sum % Bignum::BASE);
        carry = sum / Bignum::BASE;
    }

    if (carry != 0)
        new_digits.push_back(carry);

    store = new_digits;
    return *this;
}

const Bignum& Bignum::operator-=(const Bignum& other) {
    uint64_t k = 0;
    std::vector<uint32_t>::size_type max_length = std::max(store.size(), other.store.size());

    std::vector<uint32_t> new_digits;

    for (std::vector<uint32_t>::size_type i = 0; i < max_length; ++i) {
        uint64_t difference = digit(store, i) - digit(other.store, i) + k;
        new_digits.push_back(difference % Bignum::BASE);
        k = difference / Bignum::BASE;
    }

    store = new_digits;
    return *this;
}

Bignum operator-(const Bignum& left, const Bignum& right) {
    return Bignum(left) -= right;
}

std::ostream& operator<<(std::ostream& out, const Bignum& n) {
    out << "sign: " << n.signum << std::endl;
    out << "digits: " << std::endl;

    for (std::vector<uint32_t>::size_type i = 0; i != n.store.size(); ++i)
        out << i << ": " << std::hex << n.store[i] << std::endl;

    return out;
}
