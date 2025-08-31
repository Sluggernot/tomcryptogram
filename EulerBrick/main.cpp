#include <iostream>
#include <string>
#include "euler_brick_search.h"

void printUsage() {
    std::cout << "Efficient Perfect Euler Brick Search" << std::endl;
    std::cout << "Usage: ./EulerBrick [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --start=N       Starting value for search (default: 500000000000)" << std::endl;
    std::cout << "  --end=N         Ending value for search (default: 500000010000)" << std::endl;
    std::cout << "  --threads=N     Number of threads (default: 8)" << std::endl;
    std::cout << "  --output=FILE   Save solutions to file" << std::endl;
    std::cout << "  --pickup        Resume from previous checkpoint" << std::endl;
    std::cout << "  --help          Show this help" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  ./EulerBrick --start=500000000000 --end=500001000000 --threads=8" << std::endl;
    std::cout << "  ./EulerBrick --pickup --threads=16" << std::endl;
    std::cout << "  ./EulerBrick --start=1000000000000 --end=1000001000000 --threads=16 --output=solutions.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Mathematical Background:" << std::endl;
    std::cout << "  Searching for perfect Euler bricks where all face diagonals" << std::endl;
    std::cout << "  AND the space diagonal are integers." << std::endl;
    std::cout << "  Proven minimum bounds: smallest edge > 5×10¹¹" << std::endl;
    std::cout << "  Uses constraint filtering: one edge must be divisible by 17, 29, or 37" << std::endl;
}

int main(int argc, char* argv[]) {
    // Default to viable search range based on proven mathematical bounds
    mpz_int start_val = mpz_int("500000000000");  // 5×10¹¹
    mpz_int end_val = mpz_int("500000010000");    // Small range for initial testing
    int num_threads = 8;  // Default to multi-threading for large number performance
    std::string output_file = "";
    bool pickup_from_checkpoint = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            printUsage();
            return 0;
        }
        else if (arg == "--pickup") {
            pickup_from_checkpoint = true;
        }
        else if (arg.substr(0, 8) == "--start=") {
            start_val = mpz_int(arg.substr(8));
        }
        else if (arg.substr(0, 6) == "--end=") {
            end_val = mpz_int(arg.substr(6));
        }
        else if (arg.substr(0, 10) == "--threads=") {
            num_threads = std::stoi(arg.substr(10));
        }
        else if (arg.substr(0, 9) == "--output=") {
            output_file = arg.substr(9);
        }
        else {
            std::cout << "Unknown argument: " << arg << std::endl;
            printUsage();
            return 1;
        }
    }
    
    // Create search object first (needed for checkpoint loading)
    std::cout << "Start: " << start_val << std::endl;
    EulerBrickSearch search(start_val, end_val);
    
    // Handle checkpoint pickup
    if (pickup_from_checkpoint) {
        std::cout << "Attempting to resume from checkpoint..." << std::endl;
        if (search.loadCheckpoint("euler_progress.txt")) {
            std::cout << "Successfully loaded checkpoint!" << std::endl;
        } else {
            std::cout << "No checkpoint found, starting fresh search..." << std::endl;
        }
    }
    
    // Validate inputs (after potential checkpoint loading)
    if (start_val >= end_val) {
        std::cerr << "Error: start value must be less than end value" << std::endl;
        return 1;
    }
    
    if (num_threads < 1 || num_threads > 64) {
        std::cerr << "Error: thread count must be between 1 and 64" << std::endl;
        return 1;
    }
    
    // Warn if searching below proven bounds
    mpz_int minimum_bound = mpz_int("500000000000");  // 5×10¹¹
    if (start_val < minimum_bound) {
        std::cout << "WARNING: Starting below proven minimum bound of 5×10¹¹" << std::endl;
        std::cout << "Mathematical proof shows no perfect Euler brick exists below this threshold" << std::endl;
        std::cout << "Consider using --start=" << minimum_bound << " or higher" << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "=== PERFECT EULER BRICK SEARCH ===" << std::endl;
    std::cout << "Search range: " << start_val << " to " << end_val << std::endl;
    std::cout << "Threads: " << num_threads << std::endl;
    std::cout << "Mathematical constraints: 17/29/37, 5/7/11/19, parity rules" << std::endl;
    std::cout << "Expected filter efficiency: >99.9%" << std::endl;
    if (!output_file.empty()) {
        std::cout << "Output file: " << output_file << std::endl;
    }
    if (pickup_from_checkpoint) {
        std::cout << "Resume mode: Will save checkpoints every 10M combinations" << std::endl;
    }
    std::cout << std::endl;
    
    if (num_threads > 1) {
        search.searchMultiThreaded(num_threads);
    } else {
        search.search();
    }
    
    // Print results
    search.printSolutions();
    
    // Save to file if specified
    if (!output_file.empty()) {
        search.saveSolutionsToFile(output_file);
    }
    
    std::cout << "\n=== SEARCH COMPLETE ===" << std::endl;
    
    return 0;
}