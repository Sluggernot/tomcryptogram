/*
// Created by sluggernot on 8/31/25.
Numbers whose divisors can not add up to the number itself
https://en.wikipedia.org/wiki/Weird_number
https://oeis.org/A006037

*/

#include <iostream>
#include <boost/multiprecision/gmp.hpp>
using namespace boost::multiprecision;

void findAllDivisors(const mpz_int& number, std::vector<mpz_int>& divisors);
bool findDivisorComboThatAddsUpTo(const mpz_int& number, const std::vector<mpz_int>& divisors);
bool isAbundant(const mpz_int& number, const std::vector<mpz_int>& divisors);

int main(int argc, const char * argv[]) {

    mpz_int i = 3;
    std::vector<mpz_int> divisors;
    int total = 0;

    for (; ; ++i) {
        divisors.clear();

        findAllDivisors(i, divisors);
        if (divisors.size() <= 2) continue;//Prime
        if (!isAbundant(i, divisors)) continue;//Not Abundant

        if (findDivisorComboThatAddsUpTo(i, divisors)) {
            std::cout << i << " is a weird number! ";
            for (auto& j : divisors) { std::cout << j << " ";}
            std::cout << "\n";
            ++total;
            if (total > 100) { break; }
        }
    }

    return 0;
}

void findAllDivisors(const mpz_int& number, std::vector<mpz_int>& divisors) {
    divisors.emplace_back(1);
    for (mpz_int i = 2; i <= number/2; ++i) {
        if (number % i == 0) {
            divisors.emplace_back(i);
        }
    }
}

bool findDivisorComboThatAddsUpTo(const mpz_int& number, const std::vector<mpz_int>& divisors) {
    mpz_int sum = 0;
    for (const auto& i : divisors) {
        sum += i;
        if (sum == number) {
            return false;
        }
        if (sum > number) {
            //TODO:Strategy for reducing the sum by different amounts to see if other number combinations exist.
        }
    }
    return true;//No combo found.
}

bool isAbundant(const mpz_int& number, const std::vector<mpz_int>& divisors) {
    mpz_int sum = 0;
    for (auto& i : divisors) {
        sum += i;
    }
    return sum > number;
}