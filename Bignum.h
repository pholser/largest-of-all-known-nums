#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <tr1/cstdint>
#include <vector>
#include <iostream>

class Bignum {
    public:
        Bignum(int);
        Bignum(const std::vector<uint32_t>& digits, int sign);
        Bignum(const Bignum& other);
        Bignum& operator=(const Bignum& other);
        bool operator==(const Bignum& other) const;
        bool operator!=(const Bignum& other) const;
        const Bignum operator+(const Bignum& other) const;
        Bignum& operator+=(const Bignum& other);
        friend std::ostream& operator<<(std::ostream& out, const Bignum& n);

    private:
        static const uint64_t BASE;
        std::vector<uint32_t> store;
        int signum;
};

#endif  // __BIGNUM_H__
