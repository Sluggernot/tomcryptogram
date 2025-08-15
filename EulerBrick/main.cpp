#include <iostream>
#include <string>
#include "euler_brick_search.h"

void printUsage() {
    std::cout << "Efficient Perfect Euler Brick Search" << std::endl;
    std::cout << "Usage: ./EulerBrick [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --start=N       Starting value for search (default: 1)" << std::endl;
    std::cout << "  --end=N         Ending value for search (default: 100000)" << std::endl;
    std::cout << "  --threads=N     Number of threads (default: 1)" << std::endl;
    std::cout << "  --output=FILE   Save solutions to file" << std::endl;
    std::cout << "  --help          Show this help" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  ./EulerBrick --start=1 --end=50000 --threads=8" << std::endl;
    std::cout << "  ./EulerBrick --start=1 --end=1000000 --threads=16 --output=solutions.csv" << std::endl;
    std::cout << std::endl;
    std::cout << "Mathematical Background:" << std::endl;
    std::cout << "  Searching for perfect Euler bricks where all face diagonals" << std::endl;
    std::cout << "  AND the space diagonal are integers." << std::endl;
    std::cout << "  Uses proven constraint: one edge must be divisible by 17, 29, or 37" << std::endl;
}

int main(int argc, char* argv[]) {
    mpz_int start_val = 1;
    mpz_int end_val = 100000;
    int num_threads = 1;
    std::string output_file = "";
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            printUsage();
            return 0;
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
    
    // Validate inputs
    if (start_val >= end_val) {
        std::cerr << "Error: start value must be less than end value" << std::endl;
        return 1;
    }
    
    if (num_threads < 1 || num_threads > 64) {
        std::cerr << "Error: thread count must be between 1 and 64" << std::endl;
        return 1;
    }
    
    std::cout << "=== PERFECT EULER BRICK SEARCH ===" << std::endl;
    std::cout << "Search range: " << start_val << " to " << end_val << std::endl;
    std::cout << "Threads: " << num_threads << std::endl;
    std::cout << "Using constraint: one edge divisible by 17, 29, or 37" << std::endl;
    if (!output_file.empty()) {
        std::cout << "Output file: " << output_file << std::endl;
    }
    std::cout << std::endl;
    
    // Create and run search
    EulerBrickSearch search(start_val, end_val);
    
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