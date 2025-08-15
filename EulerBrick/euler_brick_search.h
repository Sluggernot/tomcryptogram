#ifndef EULER_BRICK_SEARCH_H
#define EULER_BRICK_SEARCH_H

#include <boost/multiprecision/gmp.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <fstream>

using namespace boost::multiprecision;

struct EulerBrickSolution {
    mpz_int a, b, c;
    mpz_int face_diag1, face_diag2, face_diag3;
    mpz_int space_diag;
    
    bool isValid() const;
    void print() const;
};

struct EulerBrickWorker {
    std::thread worker_thread;
    mpz_int current_range_start;
    mpz_int current_range_end;
    int thread_id;
    std::vector<EulerBrickSolution> solutions_found;
};

class EulerBrickSearch {
private:
    mpz_int search_start;
    mpz_int search_end;
    mpz_int current_position;
    std::atomic<long long> candidates_tested{0};
    std::atomic<long long> solutions_found{0};
    
    std::mutex progress_mutex;
    std::mutex solution_mutex;
    std::vector<EulerBrickSolution> all_solutions;
    
    // Search space reduction using proven constraints
    bool satisfiesDivisibilityConstraint(const mpz_int& a, const mpz_int& b, const mpz_int& c) const;
    bool satisfiesAllConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const;
    bool satisfiesParityConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const;
    bool satisfiesPrimeConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const;
    bool isValidCandidate(const mpz_int& a, const mpz_int& b, const mpz_int& c) const;
    
    // Helper functions for constraint checking
    bool hasFactorInAnyEdge(const mpz_int& a, const mpz_int& b, const mpz_int& c, int factor) const;
    bool hasFactorInAnyDiagonalOrEdge(const mpz_int& a, const mpz_int& b, const mpz_int& c, int factor) const;
    
    // Fast integer square root check
    bool isPerfectSquare(const mpz_int& n) const;
    
    // Core search logic
    void searchRange(mpz_int start, mpz_int end, int thread_id);
    bool testEulerBrickCandidate(const mpz_int& a, const mpz_int& b, const mpz_int& c, EulerBrickSolution& solution);
    
public:
    explicit EulerBrickSearch(mpz_int start = 1, mpz_int end = 1000000);
    
    // Single-threaded search
    void search();
    
    // Multi-threaded search  
    void searchMultiThreaded(int num_threads);
    
    // Progress and results
    void printProgress() const;
    void printSolutions() const;
    void saveSolutionsToFile(const std::string& filename) const;
    
    // Search space optimization
    mpz_int getNextValidCandidate(mpz_int current) const;
    
    // Range management for threading
    bool getNextWorkRange(mpz_int& start, mpz_int& end, mpz_int range_size = 10000);
};

#endif // EULER_BRICK_SEARCH_H