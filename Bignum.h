#ifndef __BIGNUM_H__
#define __BIGNUM_H__

class Bignum {
    public:
        Bignum(int);
        bool operator==(const Bignum& other) const;
        bool operator!=(const Bignum& other) const;

    private:
        int store;
};
#endif  // __BIGNUM_H__
