# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This project uses CMake with multiple subprojects. The main CMakeLists.txt defines several executables:

- **MagicSquares**: The primary mathematical computation project for finding magic squares
- **PowersOfFive**: Explores mathematical relationships with powers of five
- **EulerBrick**: Mathematical exploration (currently minimal implementation)
- **NoobyHabits**: C++ best practices examples and learning exercises
- **EveryMapout**: Utility project
- **FiveMinuteTimer**: Timer utility
- **MultiThreading**: Threading examples
- **TestFuncs**: Test functions

Build directories: `cmake-build-debug/` and `cmake-build-release/`

## Key Dependencies

- **GMP/GMPXX**: GNU Multiple Precision Arithmetic Library for arbitrary precision calculations
- **Boost Multiprecision**: Used extensively for large number computations
- **Eigen3**: Linear algebra library (commented out in some projects)
- **C++20**: Language standard

## Project Architecture

### MagicSquares (Primary Project)
The main mathematical research project focused on finding perfect magic squares of squares. Key components:

- `main.cpp`: Command-line interface with parameters for threading, file processing, and search ranges
- `mpz_only.h/cpp`: Core computation engine using arbitrary precision arithmetic with threading support
- `MagicSquare_data.h/cpp`: Data structures and validation for magic square properties
- `squares_container.h/cpp`: Container classes for managing square collections
- `OutputFiles/`: Directory containing CSV results from computational searches

The project searches for magic squares where each cell contains a perfect square and all rows, columns, and diagonals sum to the same value.

### Key Computational Concepts
- **Equidistant values**: Numbers that maintain consistent mathematical relationships when squared
- **Parametric search**: Algorithm using variables x, a, b to systematically explore solution space
- **Multi-threading**: Parallel computation with worker threads for large-scale searches
- **Near misses**: Solutions that almost satisfy magic square conditions (documented in Notes file)

### NoobyHabits
Educational project demonstrating C++ best practices, based on a YouTube video with 31 examples of common mistakes and their solutions.

### PowersOfFive
Mathematical exploration of relationships between powers of five, searching for specific sum patterns.

## File Organization

- Root level contains main CMakeLists.txt and individual project directories
- Each subproject has its own CMakeLists.txt with specific dependencies
- `MagicSquares/Notes`: Contains extensive research notes, findings, and mathematical observations
- `cmake-build-*/`: Generated build artifacts (both debug and release configurations)

## Development Notes

- The MagicSquares project has found several "near miss" solutions documented in the Notes file
- Computational searches have processed over 1 trillion magic square combinations
- The project uses CSV output for long-running computational results
- Threading is implemented to utilize multiple CPU cores for mathematical searches

## Recent Progress (August 2025)

### Algorithm Optimization
- **Performance breakthrough**: Optimized `findAllEquidistantValues` algorithm achieves 2x speedup
- **Key insight**: Eliminated nested loops by calculating iterL directly using `targetL = √(2X² - iterR²)`
- **Mathematical optimization**: Reduced O(R × L) nested loops to O(R) single loop with direct calculation
- **Testing on 1216265**: Original 50,325 microseconds → Optimized 25,550 microseconds

### Prime Factorization Discovery
- **Critical finding**: 1216265 (best near-miss candidate with 7680 error) has special properties
- **Mathematical insight**: All prime factors are ≡ 1 (mod 4): {5, 17, 41, 349}
- **Number theory connection**: Numbers with prime factors ≡ 1 (mod 4) have many sum-of-two-squares representations
- **Explains equidistant pairs**: 1216265 should have ~32 representations as sum of two squares (found 40 pairs)

### Strategic Candidate Analysis
- **Generated strategic_candidates.txt**: 45 mathematically targeted values based on prime factorization patterns
- **All candidates have exactly 40 equidistant pairs**: Confirms pattern-based selection worked
- **Testing infrastructure**: Created `analyze_candidates.cpp` for batch analysis
- **Performance validation**: 2x speedup applies to all candidates

### Pattern Visualization System
- **CSV output implementation**: Added comprehensive data export for pattern analysis
- **Data captured**: Index, equidistant count, prime factors, mod4 compatibility, near miss errors, pair ratios
- **Created pattern_analysis.csv**: Rich dataset for 45 strategic candidates
- **Visualization ready**: Data formatted for import into Python/matplotlib, R, Excel, etc.

### Key Insights for Future Work
1. **Focus on numbers with prime factors ≡ 1 (mod 4)** for high equidistant counts
2. **Near miss analysis** shows patterns but no clear path to perfect squares yet
3. **Mathematical optimization** more effective than micro-optimizations for large number arithmetic
4. **Pattern visualization** now possible with comprehensive CSV data export

### Files Added/Modified
- `analyze_candidates.cpp`: Batch analysis tool for strategic candidates
- `csv_analysis.cpp`: Pattern visualization data generator  
- `strategic_candidates.txt`: 45 mathematically targeted test values
- `pattern_analysis.csv`: Comprehensive analysis results for visualization
- `MagicSquares/mpz_only.h/cpp`: Optimized algorithm + CSV output functions
- `MagicSquares/claude_chat.txt`: Detailed session history and discoveries

### Pattern Analysis Breakthrough (August 2025)
- **MAJOR DISCOVERY**: Found candidate 1105 with relative error 449 (10x better than previous best!)
- **Strategic pattern validated**: {5, 13, 17} factors produce optimal results
- **Mathematical insight confirmed**: Factor 5 essential (100% correlation), factors 13+17 critical (85% correlation)

### Large-Scale Analysis Tools
- **Created large_scale_analyzer.cpp**: Optimized for 1000-20million range processing  
- **Smart scanning mode**: Focuses only on strategic factor patterns (90% search space reduction)
- **Real-time breakthrough detection**: Automatically flags exceptional candidates
- **Python visualization suite**: Both full-featured and dependency-free versions for pattern analysis

### Strategic Search Parameters (Data-Driven)
- **ESSENTIAL: Factor 5** (appears in 100% of best candidates)
- **CRITICAL: Factors 13 AND 17** (appears in 85% of best candidates) 
- **BENEFICIAL: Factor 41** (pushes candidates into top tier)
- **Target ratio range: 2.03-2.18** (optimal performance window)
- **Priority pattern: {5, 13, 17, 41}** for maximum potential

### Next Steps
- Run large-scale smart scan on 1000-50,000 range using validated patterns
- Test candidate 1105 thoroughly for actual magic square formation potential
- Scale to 1000-20million range with strategic factor filtering
- Continue mathematical impossibility research guided by discovered patterns

### Command Usage for Large-Scale Search
```bash
# Single-threaded CSV output
./MagicSquares --start=1000 --maxValue=50000 --output=results.csv

# MULTI-THREADED CSV output (MUCH FASTER!)
./MagicSquares --start=1000 --maxValue=2000000 --j=8 --output=viewMe.csv

# Multi-threaded without CSV (original functionality)
./MagicSquares --start=1000 --maxValue=2000000 --j=8

# Results analysis with Python visualizer
python3 simple_visualizer.py --csv viewMe.csv

# Alternative: Use standalone analyzer for smart scanning
./large_scale_analyzer 1000 50000 smart
```

### CSV Output Implementation (Clean Design)
- **No class pollution**: CSV output uses separate functions (`searchWithCsvOutput()`, `makeThreadsAndCalculateWithCsv()`)
- **Preserves core logic**: Main search functionality (`start()`, `makeThreadsAndCalculate()`) remains untouched
- **Multi-threaded support**: Thread-safe CSV writing with mutex protection for massive performance gains
- **Parameter-based**: Clean function interface without unnecessary instance state
- **Pattern analysis ready**: Outputs all strategic metrics for visualization

### Multi-Threading Performance
- **Thread-safe CSV writing**: Uses mutex to protect shared file access
- **Parallel processing**: Each thread processes different index ranges simultaneously
- **Expected speedup**: 4-8x faster on multi-core systems (vs single-threaded CSV)
- **Immediate flushing**: CSV data written in real-time for progress monitoring