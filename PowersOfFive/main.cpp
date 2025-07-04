//
// Created by sluggernot on 3/17/25.
//
#include <iostream>
#include <boost/multiprecision/gmp.hpp>
using namespace boost::multiprecision;

mpz_int toPowerOfFive(const mpz_int& incoming);
mpz_int toPowerOfN(const mpz_int& incoming, const unsigned int N);

bool findAnAlternativeWay(const mpz_int& target, const mpz_int& a, const mpz_int& b);

int main (int argc, const char * argv[]) {

  constexpr int initialLimit = 100;

  mpz_int start = 61400; //starting point. (to the power of 5)
  mpz_int limit = start + initialLimit;

  std::cout << "Starting with : " << start << std::endl;

  mpz_int total = 0;
  mpz_int mpz_i;//, mpz_j;
  while (true) {
    for (mpz_int i = start; i <= limit; ++i) {
      mpz_i = toPowerOfFive(i);
      for (mpz_int j = i+1; j <= limit; ++j) {
        total = mpz_i+toPowerOfFive(j);
        if (findAnAlternativeWay(total, i, j)) {
          std::cout << "Holy shit we've seen: " << total << " We're on: " << i << ' ' << j << std::endl;
          return 0;
        }
      }
    }
    std::cout <<"Hit limit: " << limit << "  Total so far: " << total << "\n";
    start = limit;
    limit += initialLimit;
  }

  return 0;
}

mpz_int toPowerOfFive(const mpz_int& incoming) {
  return incoming * incoming * incoming * incoming * incoming;
}

bool searchMeeting(const mpz_int& target, const mpz_int& a, const mpz_int& b) {
  mpz_int aVal = 0;
  for (mpz_int i = a-1; i >= b; --i) {
    if (i == b) continue;
    aVal = toPowerOfFive(i);
    for (mpz_int j = b+1; j <= i; ++j) {
      if (aVal + toPowerOfFive(j) == target) { std::cout << "FOUND A HIT!: " << i << " - " << j << std::endl; return true; }
    }
  }
  return false;
}

bool findAnAlternativeWay(const mpz_int& target, const mpz_int& a, const mpz_int& b) {

  mpz_int aVal = 0;
  mpz_int totalCheck = 0;
  for (mpz_int i = a+1; ; ++i) {
    if (i == b) continue;
    aVal = toPowerOfFive(i);
    if (aVal > 2 * target) break;
    for (mpz_int j = b-(i-a); j >= 0; --j) {
      totalCheck = aVal + toPowerOfFive(j);
      if (totalCheck > target) break;
      if (totalCheck == target) { std::cout << "FOUND A HIT!: " << i << " - " << j << std::endl; return true; }
    }
  }

  return searchMeeting(target, a, b);
}

mpz_int toPowerOfN(const mpz_int& incoming, const unsigned int N) {
  mpz_int total = incoming;
  for (unsigned i = 1; i <= N; ++i) {
    total *= incoming;
  }
  return total;
}
