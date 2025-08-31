# PowersOfFive

## Purpose
This project searches for **taxicab numbers for fifth powers** - numbers that can be expressed as the sum of two fifth powers in more than one way.

## Mathematical Problem
The program searches for cases where:
```
i^5 + j^5 = x^5 + y^5
```
where (i,j) ` (x,y)

This is the fifth-power variant of the famous Ramanujan taxicab number problem (which deals with cubes).

## Algorithm
1. Starting from value 61400, generates pairs (i,j) where j > i
2. Computes sum = i^5 + j^5 using arbitrary precision arithmetic (GMP)
3. For each sum, searches for alternative pairs (x,y) that produce the same result
4. Uses two search strategies:
   - `findAnAlternativeWay()`: Forward search from current position
   - `searchMeeting()`: Backward search within bounds

## Implementation Details
- Uses **Boost Multiprecision** with GMP backend for handling very large numbers
- Incremental search with configurable batch size (100 values per iteration)
- Outputs "Holy shit we've seen:" when a taxicab number is found
- Continues searching indefinitely until a match is discovered

## Current Status
- Searching from value 61400 onwards
- No taxicab numbers for fifth powers have been found yet
- Known mathematical conjecture suggests they may not exist, making this an open research problem

## Related Work
This connects to the broader study of generalized taxicab numbers Ta(n,k) where k numbers are summed to the nth power in multiple ways.