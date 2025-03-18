//
// Created by sluggernot on 3/17/25.
//
#include <iostream>
#include <boost/multiprecision/gmp.hpp>
#include <set>
using namespace boost::multiprecision;

mpz_int toPowerOfFive(const mpz_int& incoming);

int main (int argc, const char * argv[]) {

  constexpr int initialLimit = 10000;

  mpz_int start = 1;
  mpz_int limit = initialLimit;

  std::set<mpz_int> totalsWeveSeen;
  mpz_int total = 0;
  while (true) {
    for (mpz_int i = start; i <= limit; ++i) {
      for (mpz_int j = i+1; j <= limit; ++j) {
        total = toPowerOfFive(i) + toPowerOfFive(j);
        if (!totalsWeveSeen.contains(total)) {
          totalsWeveSeen.insert(total);
        }
        else {
          std::cout << "Holy shit we've seen: " << total << " We're on: " << i << ' ' << j << std::endl;
          std::cout << "We checked: " << totalsWeveSeen.size() << " values" << std::endl;
          return 0;
        }
      }
    }
    std::cout <<"Hit limit: " << limit << std::endl;
    start = limit;
    limit += initialLimit;
  }

  std::cout << "We checked: " << totalsWeveSeen.size() << " values" << std::endl;

  return 0;
}

mpz_int toPowerOfFive(const mpz_int& incoming) {
  return incoming * incoming * incoming * incoming * incoming;
}

