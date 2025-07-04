//
// Created by sluggernot on 2/27/25.
//

#include "mpz_only.h"

#include <fstream>

#include "MagicSquare_data.h"

bool isASquare(const mpz_int& testMe) {
    if (testMe < 0) return false;
#if VERBOSE
    std::cout << "Test Me: " << testMe << "\n";
#endif
    const mpz_int root = sqrt(testMe);
    return testMe == root*root;
}

mpz_only::mpz_only() {
    //Just in case.
    currentVal = 1;
    boundingVal = 1;
    maxVal = -1;
}

void mpz_only::setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding, const mpz_int& max, bool adjustStart) {
    boundingVal = bounding;
    currentVal = starting;
    maxVal = max;

    if (boundingVal < 1) {boundingVal = 1;}
    if (currentVal < 1) {currentVal = 1;}
    //if (currentVal < boundingVal) {currentVal = boundingVal;}
    std::cout << "Starting value: " << currentVal << " Bounding: " << boundingVal << std::endl;
    if (adjustStart && currentVal % boundingVal != 0) {
        currentVal = currentVal - (currentVal % boundingVal);
        if (currentVal % boundingVal != 0) { std::cout << "SANITY CHECK: I forgot how modulus worked?" << std::endl; }

        std::cout << "Adjusted starting value: " << currentVal << " Bounding: " << boundingVal << std::endl;
    }
}

void mpz_only::findAllEquidistantValues(const mpz_int& index, std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs) {
    equidistPairs.clear();

    const mpz_int twoX = index*index * 2;
    mpz_int iterR = index+1;
    mpz_int iterL = index;

    mpz_int iterRv = iterR*iterR;
    while (iterRv < twoX) {
        while (iterL >= 0 && twoX < iterL*iterL + iterRv) {--iterL;}
        if (iterL < 0) { break; }
        if (iterL*iterL + iterRv == twoX) {
            equidistPairs.emplace_back(iterL*iterL, iterRv);
        }
        ++iterR;
        iterRv = iterR*iterR;
    }
}

bool mpz_only::advanceTheCurrentVal() {
    currentVal+=boundingVal;
    if (counter % 10000 == 0) {
        std::ofstream file("lastLeftoff.txt");
        file << currentVal << "\n";
        file.close();
        std::cout << currentVal << std::endl;
    } //So I can have some idea of where to pick up.
    return maxVal == -1 || currentVal <= maxVal;
}

void mpz_only::start() {
    while (true) {
        //GivenAnIndexTestValue(currentVal); //Would only use if I wanted single threaded to have special output?
        findAllEquidistantValues(currentVal, equidistant_vals);
        if (testEquidistantValsForSquares(currentVal, equidistant_vals)) {break;};
        ++counter;
        if (!advanceTheCurrentVal()) return;
    }
}

bool mpz_only::returnWorkerValAndReadyNext(mpz_int& index) {
    ++counter;

    std::unique_lock<std::mutex> lock(mpzOnlyMutex);
    if (!advanceTheCurrentVal()) return false;
    const auto ret = currentVal;
    index = ret;
    return true;
}

void mpz_only::GivenAnIndexTestValue(const mpz_int &index) {
    findAllEquidistantValues(index, equidistant_vals);
    if (equidistant_vals.size() >= mostEquidistants) {
        std::cout << "Met or exceeded highest equidistant count: " << index << "," << index*index << "," << equidistant_vals.size() << \
            " Largest value: " << equidistant_vals.at(equidistant_vals.size()-1).second <<  std::endl;
        mostEquidistants = equidistant_vals.size();
    }
}

bool mpz_only::testEquidistantValsForSquares(const mpz_int& index, const std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs) {
    if (equidistPairs.size() < 2) return false;
    if (equidistPairs.size() > 200) std::cout << index << " has " << equidistPairs.size() << " pairs. Largest val: " << equidistPairs.at(equidistPairs.size()-1).second << std::endl;

    const mpz_int x = index * index;
    
    // Try all combinations of equidistant pairs to find a, b values for magic square
    for (size_t i = 0; i < equidistPairs.size(); i++) {
        for (size_t j = i + 1; j < equidistPairs.size(); j++) {
            // Extract potential a and b values from the pairs
            // Since pairs are (a², b²) where a² + b² = 2x², 
            // we need to find actual a, b values that work
            
            const mpz_int a_squared = equidistPairs.at(i).first;
            const mpz_int b_squared = equidistPairs.at(j).first;
            
            // Check if a_squared and b_squared are perfect squares
            const mpz_int a = sqrt(a_squared);
            const mpz_int b = sqrt(b_squared);
            
            if (a * a != a_squared || b * b != b_squared) continue;
            
            // Now test if this produces a magic square of squares
            // Magic square formula:
            // x-a  | x+a+b |  x-b
            // x+a-b |   x   | x-a+b
            // x+b  | x-a-b |  x+a
            
            mpz_int square_values[9];
            square_values[0] = x - a;      // top-left
            square_values[1] = x + a + b;  // top-center  
            square_values[2] = x - b;      // top-right
            square_values[3] = x + a - b;  // middle-left
            square_values[4] = x;          // middle-center
            square_values[5] = x - a + b;  // middle-right
            square_values[6] = x + b;      // bottom-left
            square_values[7] = x - a - b;  // bottom-center
            square_values[8] = x + a;      // bottom-right
            
            // Check if all values are positive and perfect squares
            bool allSquares = true;
            for (int k = 0; k < 9; k++) {
                if (square_values[k] <= 0 || !isASquare(square_values[k])) {
                    allSquares = false;
                    break;
                }
            }
            
            if (allSquares) {
                std::cout << "\n*** FOUND MAGIC SQUARE OF SQUARES! ***" << std::endl;
                std::cout << "Index: " << index << ", a: " << a << ", b: " << b << std::endl;
                
                // Print the magic square
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        std::cout << square_values[row*3 + col] << "\t";
                    }
                    std::cout << std::endl;
                }
                return true;
            }
            
            // Also try with swapped roles of a and b
            square_values[0] = x - b;      // top-left
            square_values[1] = x + b + a;  // top-center  
            square_values[2] = x - a;      // top-right
            square_values[3] = x + b - a;  // middle-left
            square_values[4] = x;          // middle-center
            square_values[5] = x - b + a;  // middle-right
            square_values[6] = x + a;      // bottom-left
            square_values[7] = x - b - a;  // bottom-center
            square_values[8] = x + b;      // bottom-right
            
            allSquares = true;
            for (int k = 0; k < 9; k++) {
                if (square_values[k] <= 0 || !isASquare(square_values[k])) {
                    allSquares = false;
                    break;
                }
            }
            
            if (allSquares) {
                std::cout << "\n*** FOUND MAGIC SQUARE OF SQUARES! ***" << std::endl;
                std::cout << "Index: " << index << ", a: " << b << ", b: " << a << std::endl;
                
                // Print the magic square
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        std::cout << square_values[row*3 + col] << "\t";
                    }
                    std::cout << std::endl;
                }
                return true;
            }
        }
    }
    
    // Check for near misses (optional, for debugging)
    static int near_miss_count = 0;
    const mpz_int threshold = index / 1000;  // Adjust threshold as needed
    
    for (size_t i = 0; i < equidistPairs.size() && near_miss_count < 10; i++) {
        for (size_t j = i + 1; j < equidistPairs.size(); j++) {
            const mpz_int a_squared = equidistPairs.at(i).first;
            const mpz_int b_squared = equidistPairs.at(j).first;
            
            const mpz_int a = sqrt(a_squared);
            const mpz_int b = sqrt(b_squared);
            
            if (a * a != a_squared || b * b != b_squared) continue;
            
            mpz_int square_values[9];
            square_values[0] = x - a;
            square_values[1] = x + a + b;
            square_values[2] = x - b;
            square_values[3] = x + a - b;
            square_values[4] = x;
            square_values[5] = x - a + b;
            square_values[6] = x + b;
            square_values[7] = x - a - b;
            square_values[8] = x + a;
            
            int non_square_count = 0;
            mpz_int total_error = 0;
            
            for (int k = 0; k < 9; k++) {
                if (square_values[k] > 0) {
                    mpz_int root = sqrt(square_values[k]);
                    mpz_int error = abs(square_values[k] - root * root);
                    if (error > 0) {
                        non_square_count++;
                        total_error += error;
                    }
                }
            }
            
            if (non_square_count <= 3 && total_error < threshold) {
                std::cout << "\nNear miss found - Index: " << index << ", a: " << a << ", b: " << b;
                std::cout << ", Non-squares: " << non_square_count << ", Total error: " << total_error << std::endl;
                near_miss_count++;
            }
        }
    }

    return false;
}

void mpz_only::makeThreadsAndCalculate(const int howManyThreads) {
    std::cout << "About to make the workers." << std::endl;
    mpz_threadWorker worker_thread[howManyThreads];
    std::cout << "Starting threads..." << std::endl;

    //Make them all point to the precalculated data.
    for (int i = 0; i < howManyThreads; i++) {
        worker_thread[i].t_threadNum = i;
    }
    std::cout << "Data is set." << std::endl;

    auto lambda = [this](mpz_threadWorker& worker) {
        while (returnWorkerValAndReadyNext(worker.t_currentVal))
        {
            findAllEquidistantValues(worker.t_currentVal, worker.t_equidistant_vals);
            if (testEquidistantValsForSquares(worker.t_currentVal, worker.t_equidistant_vals)) {
                std::cout << "found one" <<std::endl;
                break;
            }
        }
    };

    for (int i = 0; i < howManyThreads; i++) {
        worker_thread[i].t_worker_thread = std::thread(lambda, std::ref(worker_thread[i]));
    }
    std::cout << "Threads are processing." << std::endl;
    for (int i = 0; i < howManyThreads; i++) {
        worker_thread[i].t_worker_thread.join();
    }
    std::cout << "Threads are done." << std::endl;
}

mpz_int mpz_only::PrintAllDataGivenAValue(const mpz_int &index, bool bPrint/*=true*/) {
    findAllEquidistantValues(index, equidistant_vals);
    if (equidistant_vals.size() < 4) {std::cout << "Not enough equidistant vals to make anything. "; return 0; }
    if (bPrint) {
        std::cout <<"\nIndex: " << index << " Value: " << index*index << "  Equidistant count: " <<  equidistant_vals.size() << "\n\n";
        mpf_float valueF = index*index;
        std::vector<mpf_float> ratios; //why did I put these in a vector?

        for (int i = 0; i < equidistant_vals.size(); i++) {
            mpz_int& lVal = equidistant_vals[i].first;
            mpz_int& rVal = equidistant_vals[i].second;

            //Ratio of the difference from the value to index
            mpf_float ratio = (mpf_float(rVal)-valueF) / mpf_float(valueF);
            ratios.push_back(ratio);
            std::cout << sqrt(lVal) << ", " << lVal << "  -  " << sqrt(rVal) << ", " << rVal << " Ratio: " << ratio << "\n";
        }
        //Go through all ratios and try to find a pair that ~= another?
        //    return;
        std::cout << "Now searching for closest to magic square\n";
    }
    const mpz_int& x = index*index;
    const mpz_int threex = x*3;

    mpz_int closestToI = equidistant_vals.at(equidistant_vals.size()-1).second;
    mpz_int closestToAll = equidistant_vals.at(equidistant_vals.size()-1).second;
    mpz_int total = x*3;//equidistant_vals.at(equidistant_vals.size()-1).second;
    int Xa, Xb, Xc, Xd;

    for(int i = equidistant_vals.size()-1; i >= 2; i--) {
        const mpz_int botCenter = equidistant_vals.at(i).first;
        const mpz_int topCenter = equidistant_vals.at(i).second;
        for (int j = i-1; j >= 1; j--) {
            for (int k = j-1; k >= 0; k--) {
                if (k == i || k == j) { continue; }

                total = abs(x*3-(
                    // botCenter+equidistant_vals.at(j).second+equidistant_vals.at(k).second+
                    topCenter+equidistant_vals.at(j).first+equidistant_vals.at(k).first ));
                if (total == closestToI) {
                    std::cout << "Two sets with the same results? What does this mean? Value: " << total << " indices:\n" << i << " " << j << " " << k << std::endl;
                    std::cout << Xa << " " << Xb << " " << Xc << " " << "\n\n";
                }
                if (total < closestToI) {
                    if (total < 1000) std::cout <<"If this number is 0, it's the I shape: " << total << std::endl;

                    closestToI = total;
                    closestToAll = x*3+1;
                    for (int l = 0; l < equidistant_vals.size(); l++) {
                        if (l == i || l == j || l == k) { continue;}
                        mpz_int howClose = abs(x*6-(
                            (equidistant_vals.at(j).second+equidistant_vals.at(k).first +equidistant_vals.at(l).second)+
                            (equidistant_vals.at(j).first +equidistant_vals.at(k).second+equidistant_vals.at(l).first)));

                        if (howClose < closestToAll) {
                            Xa = i; Xb = j; Xc = k; Xd = l;
                            closestToAll=howClose;
                        }
                    }
                }
            }
        }
    }

    mpz_int howClose = abs(threex - equidistant_vals.at(Xb).first - equidistant_vals.at(Xa).second - equidistant_vals.at(Xc).first);
    if (bPrint) {
        std::cout <<"\nIndices: " << "A: " << Xa << " B: " << Xb << " C: " << Xc << " Xd: " << Xd << std::endl;
        const MagicSquare_data checkMe(
                        equidistant_vals.at(Xb).first, equidistant_vals.at(Xa).second, equidistant_vals.at(Xc).first,
                        equidistant_vals.at(Xd).second,x,                              equidistant_vals.at(Xd).first,
                        equidistant_vals.at(Xc).second,equidistant_vals.at(Xa).first,  equidistant_vals.at(Xb).second);
        checkMe.printMagicSquare_withSums(true);
        std::cout << "\nHow close are we to a magic square by top/bot row to 3xCenter: " << howClose << std::endl;

        mpz_int leftCol  = abs(threex - equidistant_vals.at(Xb).first - equidistant_vals.at(Xd).second - equidistant_vals.at(Xc).second);
        std::cout << "\nL/R column difference to 3x: " << leftCol << std::endl;
    }

    return howClose;
}

void mpz_only::isOneDouble(const mpz_int& startingPlace = 0) const {

    // mpz_int closest_diff = 1;//squares_set.at(maxValInContainer-1); //Stupidly large number to start with
    // //mpz_int temp_diff = squares_set.at(maxValInContainer-1); //Stupidly large number to start with
    // mpz_int closestIdxHalf = 0;
    // mpz_int closestIdxDoub = 0;
    //
    // mpz_int startingPoint = 10;
    // if (startingPlace > 10) { startingPoint = startingPlace; }
    // for (mpz_int i = startingPoint; i < squares_set.size(); ++i ) {
    //     const mpz_int twoTimes = squares_set.at(i)*2;
    //     for (mpz_int j = i+1; j < squares_set.size(); ++j) {
    //         if (squares_set.at(j) > twoTimes+twoTimes/4) { break; } //Some arbitrary allowance to go over but not go to end of all squares.
    //         if (squares_set.at(j) == twoTimes) { //Impossible. This is more of a sanity check.
    //             std::cout << "Wtf? Exactly double? Index: " << i << " is half of index: " << j << std::endl;
    //             return;
    //         }
    //         if (abs(squares_set.at(j) - twoTimes) <= closest_diff) {
    //             closest_diff = abs(squares_set.at(j) - twoTimes);
    //             closestIdxHalf = i;
    //             closestIdxDoub = j;
    //             std::cout << "Half: idx: " << i << " val: " << squares_set.at(i) << " It's doub: idx:" << j << " val: " << squares_set.at(j) << " ValDiff: " << squares_set.at(j) - twoTimes << std::endl;
    //             i = i*2 + (i/10*4);//sqrt(squares_set.at(i)*5);
    //             break;
    //         }
    //     }
    // }
    // if (closestIdxHalf == startingPlace) { std::cout << "Start and closest to half were the same \n"; }
    // std::cout << "Closest indices to being double: " << closestIdxHalf << " roughly half of idx: " << closestIdxDoub << "  with a diff of ";
    // std::cout << abs(squares_set.at(closestIdxHalf) *2 - squares_set.at(closestIdxDoub)) << " " << std::endl;
}

// Parametric approach functions - much more efficient than brute force
bool mpz_only::testParametricMagicSquare(const mpz_int& x, const mpz_int& a, const mpz_int& b) {
    // Magic square formula:
    // x-a  | x+a+b |  x-b
    // x+a-b |   x   | x-a+b
    // x+b  | x-a-b |  x+a
    
    mpz_int values[9] = {
        x - a,      x + a + b,  x - b,
        x + a - b,  x,          x - a + b,
        x + b,      x - a - b,  x + a
    };
    
    // Check if all values are positive and perfect squares
    for (int i = 0; i < 9; i++) {
        if (values[i] <= 0 || !isASquare(values[i])) {
            return false;
        }
    }
    
    std::cout << "\n*** PARAMETRIC MAGIC SQUARE OF SQUARES FOUND! ***" << std::endl;
    std::cout << "x = " << x << ", a = " << a << ", b = " << b << std::endl;
    
    // Print the magic square
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            std::cout << values[row * 3 + col] << "\t";
        }
        std::cout << std::endl;
    }
    
    return true;
}

bool mpz_only::isValidParametricTriple(const mpz_int& x, const mpz_int& a, const mpz_int& b) {
    // Basic constraints for a magic square to have all positive values
    if (a <= 0 || b <= 0 || x <= 0) return false;
    if (a >= x || b >= x) return false;          // Ensure all values are positive
    if (a + b >= x) return false;                // Ensure x - a - b > 0
    
    // Additional mathematical constraints based on known results
    // From the Notes, we know that certain modular constraints apply
    
    // Constraint 1: For efficiency, focus on x values that are perfect squares
    if (!isASquare(x)) return false;
    
    // Constraint 2: From number theory, certain modular relationships improve chances
    // Based on the Notes mentioning factors like 5, 13, 17, 29, 37
    mpz_int x_mod = x % 840;  // LCM of some promising small primes
    if (x_mod % 5 != 0 && x_mod % 13 != 0 && x_mod % 17 != 0 && 
        x_mod % 29 != 0 && x_mod % 37 != 0) {
        return false;  // Focus on values divisible by these primes
    }
    
    return true;
}

void mpz_only::parametricSearch(const mpz_int& start_x, const mpz_int& end_x) {
    std::cout << "Starting parametric search from " << start_x << " to " << end_x << std::endl;
    
    mpz_int solutions_found = 0;
    mpz_int tests_performed = 0;
    
    for (mpz_int x = start_x; x <= end_x; ++x) {
        // Skip values that don't meet basic constraints
        if (x % 1000 == 0) {
            std::cout << "Parametric search progress: x = " << x << ", tests = " << tests_performed << std::endl;
        }
        
        // Optimized ranges for a and b based on mathematical constraints
        mpz_int max_a = min(x / 3, (mpz_int)1000);  // Reasonable upper bound
        mpz_int max_b = min(x / 3, (mpz_int)1000);
        
        for (mpz_int a = 1; a <= max_a; ++a) {
            // Skip if a is not promising based on constraints
            if (!isASquare(a * a)) continue;  // Focus on cases where a is sqrt of a perfect square
            
            for (mpz_int b = 1; b <= max_b; ++b) {
                if (a == b) continue;  // Avoid degenerate cases
                
                ++tests_performed;
                
                if (!isValidParametricTriple(x, a, b)) continue;
                
                if (testParametricMagicSquare(x, a, b)) {
                    ++solutions_found;
                    std::cout << "Solution " << solutions_found << " found!" << std::endl;
                    
                    // For now, stop at first solution for verification
                    if (solutions_found >= 1) {
                        std::cout << "Stopping parametric search after finding solution." << std::endl;
                        return;
                    }
                }
            }
        }
    }
    
    std::cout << "Parametric search complete. Tests performed: " << tests_performed;
    std::cout << ", Solutions found: " << solutions_found << std::endl;
}

void mpz_only::parametricSearchThreaded(const mpz_int& start_x, const mpz_int& end_x, int numThreads) {
    std::cout << "Starting threaded parametric search from " << start_x << " to " << end_x;
    std::cout << " using " << numThreads << " threads" << std::endl;
    
    std::atomic<mpz_int> current_x(start_x);
    std::atomic<mpz_int> solutions_found(0);
    std::atomic<mpz_int> tests_performed(0);
    std::atomic<bool> solution_found(false);
    
    auto workerFunc = [&](int threadId) {
        mpz_int local_tests = 0;
        
        while (!solution_found.load()) {
            mpz_int x = current_x.fetch_add(1);
            if (x > end_x) break;
            
            if (x % 1000 == 0 && threadId == 0) {  // Only thread 0 reports progress
                std::cout << "Thread " << threadId << " progress: x = " << x;
                std::cout << ", total tests = " << tests_performed.load() << std::endl;
            }
            
            // Optimized ranges for a and b
            mpz_int max_a = min(x / 3, (mpz_int)1000);
            mpz_int max_b = min(x / 3, (mpz_int)1000);
            
            for (mpz_int a = 1; a <= max_a && !solution_found.load(); ++a) {
                if (!isASquare(a * a)) continue;
                
                for (mpz_int b = 1; b <= max_b && !solution_found.load(); ++b) {
                    if (a == b) continue;
                    
                    local_tests++;
                    
                    if (!isValidParametricTriple(x, a, b)) continue;
                    
                    if (testParametricMagicSquare(x, a, b)) {
                        mpz_int sol_count = solutions_found.fetch_add(1) + 1;
                        std::cout << "Thread " << threadId << " found solution " << sol_count << "!" << std::endl;
                        solution_found.store(true);  // Signal all threads to stop
                        break;
                    }
                }
            }
            
            // Update global test counter periodically to avoid contention
            if (local_tests % 100 == 0) {
                tests_performed.fetch_add(local_tests);
                local_tests = 0;
            }
        }
        
        // Add remaining local tests
        tests_performed.fetch_add(local_tests);
        std::cout << "Thread " << threadId << " finished" << std::endl;
    };
    
    // Launch threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerFunc, i);
    }
    
    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Threaded parametric search complete. Tests performed: " << tests_performed.load();
    std::cout << ", Solutions found: " << solutions_found.load() << std::endl;
}
