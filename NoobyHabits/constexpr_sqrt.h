#pragma once
//https://youtu.be/8-VZoXn8f9U?si=blen9BXBDmgEEw64
// https://github.com/PlummersSoftwareLLC/Primes/blob/6980b4d355deaaa16dedd82b2069a03920a58f6a/PrimeCPP/solution_3/Sieve.h#L11
// 
// This link below is the original source.
// https://baptiste-wicht.com/posts/2014/07/compile-integer-square-roots-at-compile-time-in-cpp.html


    static constexpr size_t ct_sqrt(size_t res, size_t l, size_t r) {
        if (l == r) {
            return r;
        }
        else {
            const auto mid = (r + l) / 2;

            if (mid * mid >= res) {
                return ct_sqrt(res, l, mid);
            }
            else {
                return ct_sqrt(res, mid + 1, r);
            }
        }
    }

    static constexpr size_t ct_sqrt(size_t res) {
        return ct_sqrt(res, 1, res);
    }

