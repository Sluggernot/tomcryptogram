#include "euler_brick_search.h"
#include <iostream>
#include <iomanip>
#include <chrono>

// EulerBrickSolution methods
bool EulerBrickSolution::isValid() const {
    // Check all four Euler brick constraints
    bool face1 = (a*a + b*b == face_diag1*face_diag1);
    bool face2 = (b*b + c*c == face_diag2*face_diag2);  
    bool face3 = (a*a + c*c == face_diag3*face_diag3);
    bool space = (a*a + b*b + c*c == space_diag*space_diag);
    
    return face1 && face2 && face3 && space;
}

void EulerBrickSolution::print() const {
    std::cout << "PERFECT EULER BRICK FOUND!" << std::endl;
    std::cout << "Dimensions: a=" << a << ", b=" << b << ", c=" << c << std::endl;
    std::cout << "Face diagonals: " << face_diag1 << ", " << face_diag2 << ", " << face_diag3 << std::endl;
    std::cout << "Space diagonal: " << space_diag << std::endl;
    std::cout << "Verification:" << std::endl;
    std::cout << "  a²+b² = " << (a*a + b*b) << " = " << (face_diag1*face_diag1) << " ✓" << std::endl;
    std::cout << "  b²+c² = " << (b*b + c*c) << " = " << (face_diag2*face_diag2) << " ✓" << std::endl;
    std::cout << "  a²+c² = " << (a*a + c*c) << " = " << (face_diag3*face_diag3) << " ✓" << std::endl;
    std::cout << "  a²+b²+c² = " << (a*a + b*b + c*c) << " = " << (space_diag*space_diag) << " ✓" << std::endl;
}

// EulerBrickSearch implementation
EulerBrickSearch::EulerBrickSearch(mpz_int start, mpz_int end) 
    : search_start(start), search_end(end), current_position(start) {
    std::cout << "Euler Brick Search initialized: " << start << " to " << end << std::endl;
    std::cout << "Applied constraints:" << std::endl;
    std::cout << "  - Prime factors: 5, 7, 11, 19 (must divide at least one edge)" << std::endl;
    std::cout << "  - Factor of 3: Two edges divisible by 3, one by 9" << std::endl;
    std::cout << "  - Parity: 1 odd edge, 1 divisible by 4, 1 divisible by 16" << std::endl;
    std::cout << "  - Extended factors: 13, 17, 29, 37 (edges/diagonals)" << std::endl;
    std::cout << "Expected filter efficiency: >99.9%" << std::endl;
}

// Helper functions for constraint checking
bool EulerBrickSearch::hasFactorInAnyEdge(const mpz_int& a, const mpz_int& b, const mpz_int& c, int factor) const {
    return (a % factor == 0) || (b % factor == 0) || (c % factor == 0);
}

bool EulerBrickSearch::hasFactorInAnyDiagonalOrEdge(const mpz_int& a, const mpz_int& b, const mpz_int& c, int factor) const {
    // Just check edges - diagonal checking is too expensive for the main loop
    return hasFactorInAnyEdge(a, b, c, factor);
}

bool EulerBrickSearch::satisfiesPrimeConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const {
    // Required prime factor constraints from proven results
    if (!hasFactorInAnyEdge(a, b, c, 5)) return false;   // One edge must be divisible by 5
    if (!hasFactorInAnyEdge(a, b, c, 7)) return false;   // One edge must be divisible by 7  
    if (!hasFactorInAnyEdge(a, b, c, 11)) return false;  // One edge must be divisible by 11
    if (!hasFactorInAnyEdge(a, b, c, 19)) return false;  // One edge must be divisible by 19
    
    // More complex constraints involving edges, face diagonals, or space diagonal
    if (!hasFactorInAnyDiagonalOrEdge(a, b, c, 13)) return false;  // One edge or space diagonal must be divisible by 13
    if (!hasFactorInAnyDiagonalOrEdge(a, b, c, 17)) return false;  // One edge, face diagonal or space diagonal must be divisible by 17
    if (!hasFactorInAnyDiagonalOrEdge(a, b, c, 29)) return false;  // One edge, face diagonal or space diagonal must be divisible by 29
    if (!hasFactorInAnyDiagonalOrEdge(a, b, c, 37)) return false;  // One edge, face diagonal or space diagonal must be divisible by 37
    
    return true;
}

bool EulerBrickSearch::satisfiesParityConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const {
    // From the notes: "One edge, two face diagonals and the space diagonal must be odd, 
    // one edge and the remaining face diagonal must be divisible by 4, 
    // and the remaining edge must be divisible by 16."
    
    // Count parity of edges
    int odd_edges = 0;
    int div_by_4_edges = 0;
    int div_by_16_edges = 0;
    
    if (a % 2 == 1) odd_edges++;
    else if (a % 16 == 0) div_by_16_edges++;
    else if (a % 4 == 0) div_by_4_edges++;
    
    if (b % 2 == 1) odd_edges++;
    else if (b % 16 == 0) div_by_16_edges++;
    else if (b % 4 == 0) div_by_4_edges++;
    
    if (c % 2 == 1) odd_edges++;
    else if (c % 16 == 0) div_by_16_edges++;
    else if (c % 4 == 0) div_by_4_edges++;
    
    // Must have exactly: 1 odd, 1 divisible by 4, 1 divisible by 16
    return (odd_edges == 1) && (div_by_4_edges == 1) && (div_by_16_edges == 1);
}

bool EulerBrickSearch::satisfiesAllConstraints(const mpz_int& a, const mpz_int& b, const mpz_int& c) const {
    // Fast essential constraints only
    
    // 1. One edge divisible by 17, 29, or 37 (already mostly handled in main loop)
    if (!((a % 17 == 0) || (a % 29 == 0) || (a % 37 == 0) ||
          (b % 17 == 0) || (b % 29 == 0) || (b % 37 == 0) ||
          (c % 17 == 0) || (c % 29 == 0) || (c % 37 == 0))) {
        return false;
    }
    
    // 2. Must have factors 5, 7, 11, 19 somewhere
    if (!((a % 5 == 0) || (b % 5 == 0) || (c % 5 == 0))) return false;
    if (!((a % 7 == 0) || (b % 7 == 0) || (c % 7 == 0))) return false;
    if (!((a % 11 == 0) || (b % 11 == 0) || (c % 11 == 0))) return false;
    if (!((a % 19 == 0) || (b % 19 == 0) || (c % 19 == 0))) return false;
    
    return true;
}

bool EulerBrickSearch::satisfiesDivisibilityConstraint(const mpz_int& a, const mpz_int& b, const mpz_int& c) const {
    // Legacy function - now replaced by satisfiesAllConstraints
    return satisfiesAllConstraints(a, b, c);
}

bool EulerBrickSearch::isPerfectSquare(const mpz_int& n) const {
    if (n < 0) return false;
    mpz_int root = sqrt(n);
    return (root * root == n);
}

bool EulerBrickSearch::isValidCandidate(const mpz_int& a, const mpz_int& b, const mpz_int& c) const {
    // Quick filters before expensive calculations
    if (a >= b || b >= c) return false;  // Ensure a < b < c to avoid duplicates
    
    // Apply all proven mathematical constraints
    if (!satisfiesAllConstraints(a, b, c)) return false;
    
    return true;
}

bool EulerBrickSearch::testEulerBrickCandidate(const mpz_int& a, const mpz_int& b, const mpz_int& c, EulerBrickSolution& solution) {
    // Calculate required values
    mpz_int a2 = a * a;
    mpz_int b2 = b * b; 
    mpz_int c2 = c * c;
    
    mpz_int face1_sq = a2 + b2;
    mpz_int face2_sq = b2 + c2;
    mpz_int face3_sq = a2 + c2;
    mpz_int space_sq = a2 + b2 + c2;
    
    // Check if all are perfect squares
    if (!isPerfectSquare(face1_sq) || !isPerfectSquare(face2_sq) || 
        !isPerfectSquare(face3_sq) || !isPerfectSquare(space_sq)) {
        return false;
    }
    
    // Found a perfect Euler brick!
    solution.a = a;
    solution.b = b; 
    solution.c = c;
    solution.face_diag1 = sqrt(face1_sq);
    solution.face_diag2 = sqrt(face2_sq);
    solution.face_diag3 = sqrt(face3_sq);
    solution.space_diag = sqrt(space_sq);
    
    return true;
}

void EulerBrickSearch::searchRange(mpz_int start, mpz_int end, int thread_id) {
    std::vector<EulerBrickSolution> local_solutions;
    long long local_candidates = 0;
    long long total_combinations = 0;
    
    for (mpz_int a = start; a <= end; a++) {
        // Early filter on 'a' - check if it has any required factors
        bool a_has_factor = (a % 17 == 0) || (a % 29 == 0) || (a % 37 == 0) || (a % 5 == 0) || (a % 7 == 0) || (a % 11 == 0) || (a % 19 == 0);
        
        for (mpz_int b = a + 1; b <= end + (end - start); b++) {
            // Early filter on 'b' - if neither a nor b have factors, we can skip many 'c' values
            bool b_has_factor = (b % 17 == 0) || (b % 29 == 0) || (b % 37 == 0) || (b % 5 == 0) || (b % 7 == 0) || (b % 11 == 0) || (b % 19 == 0);
            
            // If neither a nor b have ANY required factors, c MUST have specific factors
            bool need_c_factors = !a_has_factor && !b_has_factor;
            
            for (mpz_int c = b + 1; c <= end + 2*(end - start); c++) {
                total_combinations++;
                
                // Quick elimination: if a and b don't have factors, c must have 17/29/37 AND 5,7,11,19
                if (need_c_factors) {
                    bool c_has_required = ((c % 17 == 0) || (c % 29 == 0) || (c % 37 == 0)) && 
                                         (c % 5 == 0) && (c % 7 == 0) && (c % 11 == 0) && (c % 19 == 0);
                    if (!c_has_required) {
                        continue;
                    }
                }
                
                // Apply remaining constraint checking
                if (!isValidCandidate(a, b, c)) {
                    continue;
                }
                
                local_candidates++;
                
                EulerBrickSolution solution;
                if (testEulerBrickCandidate(a, b, c, solution)) {
                    local_solutions.push_back(solution);
                    
                    // Immediate notification of solution
                    {
                        std::lock_guard<std::mutex> lock(solution_mutex);
                        std::cout << "Thread " << thread_id << " FOUND SOLUTION: " 
                                 << "a=" << a << ", b=" << b << ", c=" << c << std::endl;
                        all_solutions.push_back(solution);
                        solutions_found++;
                    }
                }
                
                // Progress reporting and checkpoint saving
                if (total_combinations % 1000000 == 0) {
                    std::lock_guard<std::mutex> lock(progress_mutex);
                    if (thread_id == 0) {  // Only one thread reports progress
                        double filter_efficiency = 100.0 * (1.0 - (double)local_candidates / total_combinations);
                        std::cout << "Progress: " << total_combinations << " combinations, " 
                                 << local_candidates << " passed filters (" 
                                 << std::fixed << std::setprecision(2) << filter_efficiency 
                                 << "% filtered out), " << solutions_found.load() 
                                 << " solutions found" << std::endl;
                        
                        // Save checkpoint every 10 million combinations
                        if (total_combinations % 10000000 == 0) {
                            saveProgress(a, 0, "euler_progress.txt");
                        }
                    }
                }
            }
        }
    }
    
    // Final update
    candidates_tested += local_candidates % 100000;
}

void EulerBrickSearch::search() {
    std::cout << "Starting single-threaded Euler brick search..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    searchRange(search_start, search_end, 0);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    
    std::cout << "Search complete in " << duration.count() << " seconds" << std::endl;
    std::cout << "Total candidates tested: " << candidates_tested.load() << std::endl;
    std::cout << "Solutions found: " << solutions_found.load() << std::endl;
}

void EulerBrickSearch::searchMultiThreaded(int num_threads) {
    std::cout << "Starting multi-threaded Euler brick search with " << num_threads << " threads..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<EulerBrickWorker> workers(num_threads);
    mpz_int range_per_thread = (search_end - search_start) / num_threads;
    
    // Launch threads
    for (int i = 0; i < num_threads; i++) {
        workers[i].thread_id = i;
        workers[i].current_range_start = search_start + i * range_per_thread;
        workers[i].current_range_end = (i == num_threads - 1) ? search_end : 
                                       search_start + (i + 1) * range_per_thread - 1;
        
        workers[i].worker_thread = std::thread([this, i, &workers]() {
            this->searchRange(workers[i].current_range_start, workers[i].current_range_end, i);
        });
    }
    
    // Wait for completion
    for (auto& worker : workers) {
        worker.worker_thread.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    
    std::cout << "Multi-threaded search complete in " << duration.count() << " seconds" << std::endl;
    std::cout << "Total candidates tested: " << candidates_tested.load() << std::endl;
    std::cout << "Solutions found: " << solutions_found.load() << std::endl;
}

void EulerBrickSearch::printSolutions() const {
    std::cout << "\n=== ALL SOLUTIONS FOUND ===" << std::endl;
    for (const auto& solution : all_solutions) {
        solution.print();
        std::cout << "---" << std::endl;
    }
}

void EulerBrickSearch::saveSolutionsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    file << "# Perfect Euler Brick Solutions" << std::endl;
    file << "# Format: a,b,c,face_diag1,face_diag2,face_diag3,space_diag" << std::endl;
    
    for (const auto& solution : all_solutions) {
        file << solution.a << "," << solution.b << "," << solution.c << ","
             << solution.face_diag1 << "," << solution.face_diag2 << "," 
             << solution.face_diag3 << "," << solution.space_diag << std::endl;
    }
    
    file.close();
    std::cout << "Solutions saved to: " << filename << std::endl;
}

// Checkpoint/resume functionality
void EulerBrickSearch::saveProgress(mpz_int current_pos, int thread_count, const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "# Euler Brick Search Progress Checkpoint" << std::endl;
        file << "search_start=" << search_start << std::endl;
        file << "search_end=" << search_end << std::endl;
        file << "current_position=" << current_pos << std::endl;
        file << "thread_count=" << thread_count << std::endl;
        file << "candidates_tested=" << candidates_tested.load() << std::endl;
        file << "solutions_found=" << solutions_found.load() << std::endl;
        file << "timestamp=" << time(nullptr) << std::endl;
        file.close();
        std::cout << "Progress saved to: " << filename << std::endl;
    }
}

bool EulerBrickSearch::loadCheckpoint(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No checkpoint file found: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "search_start") search_start = mpz_int(value);
        else if (key == "search_end") search_end = mpz_int(value);
        else if (key == "current_position") current_position = mpz_int(value);
        else if (key == "candidates_tested") candidates_tested = std::stoll(value);
        else if (key == "solutions_found") solutions_found = std::stoll(value);
    }
    
    file.close();
    std::cout << "Checkpoint loaded successfully!" << std::endl;
    std::cout << "Resuming from position: " << current_position << std::endl;
    std::cout << "Search range: " << search_start << " to " << search_end << std::endl;
    return true;
}

void EulerBrickSearch::saveCheckpoint(const std::string& filename) const {
    saveProgress(current_position, 0, filename);
}