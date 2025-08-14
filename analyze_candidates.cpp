#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;

void findAllEquidistantValues(const mpz_int& index, std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs) {
    equidistPairs.clear();

    const mpz_int twoX = index*index * 2;
    mpz_int iterR = index+1;
    
    // Mathematical optimization: limit search range
    // For iterR, we know iterR² < 2X², so iterR < √2 * X ≈ 1.414 * X
    const mpz_int maxR = sqrt(twoX);

    mpz_int iterRv = iterR*iterR;
    while (iterR <= maxR) {
        // Mathematical optimization: calculate iterL directly using integer square root
        // We want iterL² = 2X² - iterR², so iterL = √(2X² - iterR²)
        mpz_int targetLSquared = twoX - iterRv;
        if (targetLSquared < 0) break;
        
        mpz_int targetL = sqrt(targetLSquared);
        
        // Check if targetL² exactly equals targetLSquared (perfect square)
        if (targetL * targetL == targetLSquared) {
            equidistPairs.emplace_back(targetLSquared, iterRv);
        }
        
        ++iterR;
        iterRv = iterR*iterR;
    }
}

int main() {
    std::ifstream file("strategic_candidates.txt");
    std::vector<std::pair<mpz_int, size_t>> results; // pair of (candidate, equidistant_count)
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open strategic_candidates.txt" << std::endl;
        return 1;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        mpz_int candidate(line);
        std::vector<std::pair<mpz_int, mpz_int>> equidistPairs;
        
        findAllEquidistantValues(candidate, equidistPairs);
        
        results.emplace_back(candidate, equidistPairs.size());
        std::cout << "Candidate: " << candidate << " has " << equidistPairs.size() << " equidistant pairs" << std::endl;
    }
    
    file.close();
    
    // Sort by equidistant count (descending)
    std::sort(results.begin(), results.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    std::cout << "\n=== RESULTS SORTED BY EQUIDISTANT COUNT ===" << std::endl;
    std::cout << "Rank\tCandidate\tEquidistant Count" << std::endl;
    std::cout << "----\t---------\t----------------" << std::endl;
    
    for (size_t i = 0; i < results.size(); i++) {
        std::cout << (i+1) << "\t" << results[i].first << "\t\t" << results[i].second << std::endl;
    }
    
    return 0;
}