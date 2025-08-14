#include <iostream>
#include <fstream>
#include <vector>
#include <boost/multiprecision/gmp.hpp>
#include "MagicSquares/mpz_only.h"

using namespace boost::multiprecision;

int main() {
    std::ifstream inputFile("strategic_candidates.txt");
    std::ofstream csvFile("pattern_analysis.csv");
    
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open strategic_candidates.txt" << std::endl;
        return 1;
    }
    
    if (!csvFile.is_open()) {
        std::cerr << "Error: Could not create pattern_analysis.csv" << std::endl;
        return 1;
    }
    
    // Write CSV header
    mpz_only::outputCsvHeader(csvFile);
    
    std::string line;
    int processedCount = 0;
    
    std::cout << "Processing strategic candidates for pattern analysis..." << std::endl;
    
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;
        
        mpz_int candidate(line);
        std::vector<std::pair<mpz_int, mpz_int>> equidistPairs;
        
        // Find equidistant values
        mpz_only::findAllEquidistantValues(candidate, equidistPairs);
        
        // Calculate near miss error (simplified version)
        mpz_int nearMissError = 0;
        if (equidistPairs.size() >= 4) {
            // Basic near miss calculation - distance from ideal magic square sum
            mpz_int x = candidate * candidate;
            mpz_int ideal_sum = x * 3;
            
            // Try a simple combination for near miss estimation
            if (equidistPairs.size() >= 3) {
                mpz_int test_sum = equidistPairs[0].first + equidistPairs[1].second + equidistPairs[2].second;
                nearMissError = abs(ideal_sum - test_sum);
            }
        }
        
        // Output to CSV
        mpz_only::outputCsvRow(csvFile, candidate, equidistPairs, nearMissError);
        
        processedCount++;
        std::cout << "Processed: " << candidate << " (" << equidistPairs.size() << " pairs)" << std::endl;
    }
    
    inputFile.close();
    csvFile.close();
    
    std::cout << "Analysis complete! Processed " << processedCount << " candidates." << std::endl;
    std::cout << "Results saved to pattern_analysis.csv" << std::endl;
    std::cout << "\nYou can now visualize patterns using tools like:" << std::endl;
    std::cout << "- Excel/Google Sheets for basic plotting" << std::endl;
    std::cout << "- Python pandas/matplotlib for advanced analysis" << std::endl;
    std::cout << "- R for statistical pattern detection" << std::endl;
    
    return 0;
}