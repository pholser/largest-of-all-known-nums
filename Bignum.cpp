#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "Bignum.h"

const uint64_t Bignum::BASE = UINT32_MAX + 1ULL;

uint64_t digit(const std::vector<uint32_t>& digits, uint32_t index) {
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

bool Bignum::operator==(const Bignum& other) const {
    return store == other.store && signum == other.signum;
}

bool Bignum::operator!=(const Bignum& other) const {
    return !(*this == other);
}

Bignum operator+(const Bignum& left, const Bignum& right) {
    return Bignum(left) += right;
}

const Bignum& Bignum::operator+=(const Bignum& other) {
    uint64_t carry(0);
    uint32_t max_length = std::max(store.size(), other.store.size());

    std::vector<uint32_t> new_digits;

    for (uint32_t i = 0; i < max_length; ++i) {
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
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Bignum& n) {
    out << "sign: " << n.signum << std::endl;
    out << "digits: " << std::endl;

    for (std::vector<uint32_t>::size_type i = 0; i != n.store.size(); ++i)
        out << i << ": " << std::hex << n.store[i] << std::endl;

    return out;
}
