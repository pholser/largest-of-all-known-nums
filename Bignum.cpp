#include "Bignum.h"

Bignum::Bignum(int n) : store(n) {
}

bool Bignum::operator==(const Bignum& other) const {
    return store == other.store;
}

bool Bignum::operator !=(const Bignum& other) const {
    return !(*this == other);
}
