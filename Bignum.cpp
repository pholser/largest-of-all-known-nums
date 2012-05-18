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

const Bignum Bignum::operator+(const Bignum& other) const {
    Bignum result = *this;
    result += other;
    return result;
}

Bignum& Bignum::operator+=(const Bignum& other) {
    uint64_t k = 0;
    uint32_t n = std::max(store.size(), other.store.size());

    std::vector<uint32_t> new_digits;

    for (uint32_t j = 0; j < n; ++j) {
        uint64_t sum = digit(store, j) + digit(other.store, j) + k;
        new_digits.push_back(sum % Bignum::BASE);
        k = sum / Bignum::BASE;
    }

    if (k != 0)
        new_digits.push_back(k);

    store = new_digits;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Bignum& n) {
    out << "sign: " << n.signum << std::endl;
    out << "digits: " << std::endl;

    for (std::vector<uint32_t>::size_type i = 0; i != n.store.size(); ++i)
        out << i << ": " << std::hex << n.store[i] << std::endl;

    return out;
}
