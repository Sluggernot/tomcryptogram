#include <iostream>
#include <fstream>
#include <vector>
#include <boost/multiprecision/gmp.hpp>
#include "MagicSquares/mpz_only.h"

using namespace boost::multiprecision;

class LargeScaleAnalyzer {
private:
    std::ofstream csvFile;
    std::ofstream summaryFile;
    int processedCount = 0;
    int candidatesFound = 0;
    mpz_int bestRelativeError = 999999999;
    mpz_int bestCandidate = 0;
    
    bool isStrategicCandidate(const mpz_int& index) {
        // Based on pattern analysis: prioritize numbers divisible by 5, 13, 17, 41
        bool has_5 = (index % 5 == 0);
        bool has_13 = (index % 13 == 0);
        bool has_17 = (index % 17 == 0);
        bool has_41 = (index % 41 == 0);
        
        // Must have factor 5 (100% of best candidates have it)
        if (!has_5) return false;
        
        // Must have at least 2 of {13, 17, 41} (85% pattern)
        int critical_factors = (has_13 ? 1 : 0) + (has_17 ? 1 : 0) + (has_41 ? 1 : 0);
        return critical_factors >= 2;
    }
    
    void quickFactorAnalysis(const mpz_int& index, std::string& factors, bool& promising) {
        factors = "";
        promising = false;
        int factor_score = 0;
        
        if (index % 5 == 0) { factors += "5 "; factor_score++; }
        if (index % 13 == 0) { factors += "13 "; factor_score++; }
        if (index % 17 == 0) { factors += "17 "; factor_score++; }
        if (index % 41 == 0) { factors += "41 "; factor_score++; }
        if (index % 349 == 0) { factors += "349 "; factor_score++; }
        
        // Promising if has factor 5 and at least 2 others from {13, 17, 41}
        promising = (index % 5 == 0) && (factor_score >= 3);
    }
    
public:
    LargeScaleAnalyzer(const std::string& csvFilename, const std::string& summaryFilename) {
        csvFile.open(csvFilename);
        summaryFile.open(summaryFilename);
        
        if (!csvFile.is_open() || !summaryFile.is_open()) {
            std::cerr << "Error opening output files!" << std::endl;
            return;
        }
        
        // Write CSV header
        csvFile << "index,index_squared,equidistant_count,strategic_factors,factor_score,promising,near_miss_error,relative_error,ratio_range\n";
        
        // Write summary header
        summaryFile << "LARGE-SCALE MAGIC SQUARE ANALYSIS SUMMARY\n";
        summaryFile << "========================================\n\n";
    }
    
    ~LargeScaleAnalyzer() {
        if (csvFile.is_open()) csvFile.close();
        if (summaryFile.is_open()) {
            summaryFile << "\nFINAL STATISTICS:\n";
            summaryFile << "Total processed: " << processedCount << "\n";
            summaryFile << "Strategic candidates found: " << candidatesFound << "\n";
            summaryFile << "Best candidate: " << bestCandidate << " (rel_error: " << bestRelativeError << ")\n";
            summaryFile.close();
        }
    }
    
    void analyzeCandidate(const mpz_int& index) {
        std::vector<std::pair<mpz_int, mpz_int>> equidistPairs;
        mpz_only::findAllEquidistantValues(index, equidistPairs);
        
        // Skip if not enough pairs
        if (equidistPairs.size() < 4) return;
        
        // Quick factor analysis
        std::string factors;
        bool promising;
        quickFactorAnalysis(index, factors, promising);
        
        // Calculate metrics
        mpz_int nearMissError = 0;
        if (equidistPairs.size() >= 3) {
            mpz_int x = index * index;
            mpz_int ideal_sum = x * 3;
            mpz_int test_sum = equidistPairs[0].first + equidistPairs[1].second + equidistPairs[2].second;
            nearMissError = abs(ideal_sum - test_sum);
        }
        
        mpz_int relativeError = nearMissError / index;
        
        // Calculate ratio range
        double ratioRange = 1.0;
        if (equidistPairs.size() > 0) {
            mpf_float largest = equidistPairs.back().second;
            mpf_float smallest = equidistPairs.front().first;
            ratioRange = (double)(largest / smallest);
        }
        
        // Count strategic factors
        int factorScore = 0;
        if (index % 5 == 0) factorScore++;
        if (index % 13 == 0) factorScore++;
        if (index % 17 == 0) factorScore++;
        if (index % 41 == 0) factorScore++;
        if (index % 349 == 0) factorScore++;
        
        // Output to CSV (only promising candidates to save space)
        if (promising || equidistPairs.size() >= 40) {
            csvFile << index << "," << (index * index) << "," << equidistPairs.size() << ","
                   << "\"" << factors << "\"," << factorScore << "," << (promising ? "1" : "0") << ","
                   << nearMissError << "," << relativeError << "," << std::fixed << std::setprecision(6) << ratioRange << "\n";
            
            candidatesFound++;
            
            // Track best candidate
            if (relativeError < bestRelativeError) {
                bestRelativeError = relativeError;
                bestCandidate = index;
            }
        }
        
        processedCount++;
        
        // Progress reporting
        if (processedCount % 10000 == 0) {
            std::cout << "Processed: " << processedCount << ", Strategic candidates: " << candidatesFound 
                     << ", Best: " << bestCandidate << " (rel_error: " << bestRelativeError << ")" << std::endl;
            
            summaryFile << "Progress at " << processedCount << ": Strategic=" << candidatesFound 
                       << ", Best=" << bestCandidate << " (rel_error=" << bestRelativeError << ")\n";
            summaryFile.flush();
        }
        
        // Report exceptional findings immediately
        if (equidistPairs.size() > 100) {
            std::cout << "EXCEPTIONAL: " << index << " has " << equidistPairs.size() << " pairs!" << std::endl;
            summaryFile << "EXCEPTIONAL: " << index << " has " << equidistPairs.size() << " pairs!\n";
        }
        
        if (relativeError < 1000) {
            std::cout << "BREAKTHROUGH: " << index << " has relative error " << relativeError << "!" << std::endl;
            summaryFile << "BREAKTHROUGH: " << index << " has relative error " << relativeError << "!\n";
        }
    }
    
    void batchAnalyze(mpz_int start, mpz_int end, mpz_int step = 1) {
        std::cout << "Starting large-scale analysis from " << start << " to " << end << std::endl;
        summaryFile << "Analysis range: " << start << " to " << end << " (step=" << step << ")\n\n";
        
        for (mpz_int index = start; index <= end; index += step) {
            // Prioritize strategic candidates
            if (isStrategicCandidate(index)) {
                analyzeCandidate(index);
            }
            // Also check some non-strategic candidates for comparison (every 100th)
            else if (index % 100 == 0) {
                analyzeCandidate(index);
            }
        }
        
        std::cout << "Analysis complete!" << std::endl;
    }
    
    void smartScan(mpz_int start, mpz_int end) {
        std::cout << "Starting smart scan focusing on strategic patterns..." << std::endl;
        summaryFile << "SMART SCAN MODE: Focusing on numbers with factors {5, 13, 17, 41}\n\n";
        
        // Pre-calculate multiples for efficient scanning
        std::vector<mpz_int> strategic_bases;
        
        // Generate numbers that are multiples of our key factors
        for (mpz_int base5 = start - (start % 5); base5 <= end; base5 += 5) {
            if (base5 < start) continue;
            
            // Check if this base has our strategic factors
            if (isStrategicCandidate(base5)) {
                strategic_bases.push_back(base5);
            }
        }
        
        std::cout << "Found " << strategic_bases.size() << " strategic candidates in range" << std::endl;
        
        for (const auto& candidate : strategic_bases) {
            analyzeCandidate(candidate);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <start> <end> [smart]" << std::endl;
        std::cout << "  start: Starting index for analysis" << std::endl;
        std::cout << "  end:   Ending index for analysis" << std::endl;
        std::cout << "  smart: Use smart scanning (optional)" << std::endl;
        return 1;
    }
    
    mpz_int start(argv[1]);
    mpz_int end(argv[2]);
    bool smart_mode = (argc > 3 && std::string(argv[3]) == "smart");
    
    std::string timestamp = std::to_string(time(nullptr));
    std::string csv_filename = "large_scale_results_" + timestamp + ".csv";
    std::string summary_filename = "analysis_summary_" + timestamp + ".txt";
    
    LargeScaleAnalyzer analyzer(csv_filename, summary_filename);
    
    if (smart_mode) {
        analyzer.smartScan(start, end);
    } else {
        analyzer.batchAnalyze(start, end);
    }
    
    std::cout << "Results saved to:" << std::endl;
    std::cout << "  " << csv_filename << std::endl;
    std::cout << "  " << summary_filename << std::endl;
    
    return 0;
}