#ifndef PHOLSER_BIGNUM_H
#define PHOLSER_BIGNUM_H

#include <tr1/cstdint>
#include <vector>
#include <iostream>

class Bignum {
    public:
        static const uint64_t BASE;
        static const std::size_t BITS_IN_DIGIT;

        Bignum(int64_t value);
        Bignum(const std::vector<uint32_t>& digits, int sign);
        Bignum(const Bignum& other);
        Bignum& operator=(const Bignum& other);

        const Bignum& operator+=(const Bignum& other);
        const Bignum& operator-=(const Bignum& other);
        const Bignum& operator>>=(unsigned int n);
        Bignum operator-() const;
        Bignum abs() const;

        bool equal(const Bignum& other) const;
        bool less(const Bignum& other) const;
        int signum() const;

        friend std::ostream& operator<<(std::ostream& out, const Bignum& n);

    private:
        std::vector<uint32_t> store;
        int sign;

        void reconcile_sign_of_zero();
};

bool operator==(const Bignum& left, const Bignum& right);
bool operator!=(const Bignum& left, const Bignum& right);
bool operator<(const Bignum& left, const Bignum& right);
bool operator<=(const Bignum& left, const Bignum& right);
bool operator>(const Bignum& left, const Bignum& right);
bool operator>=(const Bignum& left, const Bignum& right);
Bignum operator+(const Bignum& left, const Bignum& right);
Bignum operator-(const Bignum& left, const Bignum& right);
Bignum& operator++(Bignum& n);
Bignum operator++(Bignum& n, int);
Bignum& operator--(Bignum& n);
Bignum operator--(Bignum& n, int);
Bignum operator>>(const Bignum& b, unsigned int n);

#endif  // PHOLSER_BIGNUM_H
