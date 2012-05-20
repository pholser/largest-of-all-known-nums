#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <tr1/cstdint>
#include <vector>
#include <iostream>

class Bignum {
    public:
        Bignum(const std::vector<uint32_t>& digits, int sign);
        Bignum(const Bignum& other);
        Bignum& operator=(const Bignum& other);

        const Bignum& operator+=(const Bignum& other);
        const Bignum& operator-=(const Bignum& other);

        bool equal(const Bignum& other) const;
        bool less(const Bignum& other) const;

        friend std::ostream& operator<<(std::ostream& out, const Bignum& n);

    private:
        static const uint64_t BASE;
        std::vector<uint32_t> store;
        int signum;
};

bool operator==(const Bignum& left, const Bignum& right);
bool operator!=(const Bignum& left, const Bignum& right);
bool operator<(const Bignum& left, const Bignum& right);
bool operator<=(const Bignum& left, const Bignum& right);
bool operator>(const Bignum& left, const Bignum& right);
bool operator>=(const Bignum& left, const Bignum& right);
Bignum operator+(const Bignum& left, const Bignum& right);
Bignum operator-(const Bignum& left, const Bignum& right);

#endif  // __BIGNUM_H__
