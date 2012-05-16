#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <tr1/cstdint>
#include <vector>

class Bignum {
    public:
        Bignum(int);
        Bignum(const std::vector<uint32_t>& digits, int sign);
        bool operator==(const Bignum& other) const;
        bool operator!=(const Bignum& other) const;
        const Bignum operator+(const MyClass& other) const;
        Bignum& operator+=(const MyClass& other);

    private:
        std::vector<uint32_t> store;
        int signum;
};
#endif  // __BIGNUM_H__
