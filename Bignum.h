#ifndef PHOLSER_BIGNUM_H
#define PHOLSER_BIGNUM_H

#include <tr1/cstdint>
#include <vector>
#include <iostream>

class Bignum {
    public:
        static const uint64_t BASE;
        static const std::size_t BITS_IN_DIGIT;

        Bignum(const int64_t);
        Bignum(const std::vector<uint32_t>&, const int);
        Bignum(const Bignum&);
        Bignum& operator=(const Bignum&);

        const Bignum& operator+=(const Bignum&);
        const Bignum& operator-=(const Bignum&);
        const Bignum& operator>>=(const unsigned int);
        Bignum operator-() const;
        Bignum abs() const;

        bool equal(const Bignum&) const;
        bool less(const Bignum&) const;
        int signum() const;

        friend std::ostream& operator<<(std::ostream&, const Bignum&);

    private:
        std::vector<uint32_t> store;
        int sign;

        void reconcile_sign_of_zero();
};

bool operator==(const Bignum&, const Bignum&);
bool operator!=(const Bignum&, const Bignum&);
bool operator<(const Bignum&, const Bignum&);
bool operator<=(const Bignum&, const Bignum&);
bool operator>(const Bignum&, const Bignum&);
bool operator>=(const Bignum&, const Bignum&);
Bignum operator+(const Bignum&, const Bignum&);
Bignum operator-(const Bignum&, const Bignum&);
Bignum& operator++(Bignum&);
Bignum operator++(Bignum&, int);
Bignum& operator--(Bignum&);
Bignum operator--(Bignum&, int);
Bignum operator>>(const Bignum& n, const unsigned int);

#endif  // PHOLSER_BIGNUM_H
