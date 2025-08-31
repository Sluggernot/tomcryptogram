//
// Created by sluggernot on 3/17/25.
//
#include <iostream>
#include <unordered_map>
#include <boost/multiprecision/gmp.hpp>
using namespace boost::multiprecision;

std::unordered_map<mpz_int, mpz_int> fifthPowerCache;

mpz_int toPowerOfFive(const mpz_int& incoming);
mpz_int toPowerOfN(const mpz_int& incoming, const unsigned int N);

bool isPossibleTaxicabCandidate(const mpz_int& target);
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
        if (isPossibleTaxicabCandidate(total) && findAnAlternativeWay(total, i, j)) {
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
  auto it = fifthPowerCache.find(incoming);
  if (it != fifthPowerCache.end()) {
    return it->second;
  }
  mpz_int result = incoming * incoming * incoming * incoming * incoming;
  fifthPowerCache[incoming] = result;
  return result;
}

bool isPossibleTaxicabCandidate(const mpz_int& target) {
  // Fifth powers modulo 7 can only be: 0, 1, 2, 4, 5, 6
  // So sums of two fifth powers modulo 7 can only be certain values
  // This eliminates many impossible candidates quickly
  int targetMod7 = static_cast<int>(target % 7);
  static const bool validMod7[] = {true, true, true, true, true, true, false};
  return validMod7[targetMod7];
}

bool findAnAlternativeWay(const mpz_int& target, const mpz_int& a, const mpz_int& b) {

  mpz_int aVal = 0;
  mpz_int totalCheck = 0;
  
  // Forward search (i > a)
  for (mpz_int i = a+1; ; ++i) {
    if (i == b) continue;
    aVal = toPowerOfFive(i);
    if (aVal > target) break;
    for (mpz_int j = b-(i-a); j >= 0; --j) {
      totalCheck = aVal + toPowerOfFive(j);
      if (totalCheck > target) break;
      if (totalCheck == target) { std::cout << "FOUND A HIT!: " << i << " - " << j << std::endl; return true; }
    }
  }

  // Backward search (i < a) - unified from searchMeeting
  for (mpz_int i = a-1; i >= b; --i) {
    if (i == b) continue;
    aVal = toPowerOfFive(i);
    for (mpz_int j = b+1; j <= i; ++j) {
      if (aVal + toPowerOfFive(j) == target) { std::cout << "FOUND A HIT!: " << i << " - " << j << std::endl; return true; }
    }
  }
  
  return false;
}

mpz_int toPowerOfN(const mpz_int& incoming, const unsigned int N) {
  mpz_int total = incoming;
  for (unsigned i = 1; i <= N; ++i) {
    total *= incoming;
  }
  return total;
}
