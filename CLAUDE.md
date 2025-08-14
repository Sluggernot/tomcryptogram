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