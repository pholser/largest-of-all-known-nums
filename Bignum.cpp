#include <stdlib.h>
#include "Bignum.h"

Bignum::Bignum(int n) {
    store = std::vector<uint32_t>();
    store.push_back(abs(n));
    signum = n == 0 ? 0 : (n < 0 ? -1 : 1);
}

Bignum::Bignum(const std::vector<uint32_t>& digits, int sign)
    : store(digits), signum(sign) {
}

bool Bignum::operator==(const Bignum& other) const {
    return store == other.store && signum == other.signum;
}

bool Bignum::operator!=(const Bignum& other) const {
    return !(*this == other);
}

const Bignum Bignum::operator+(const Bignum& other) const {
    
}

Bignum& Bignum::operator+=(const Bignum& other) {
    return *this;
}