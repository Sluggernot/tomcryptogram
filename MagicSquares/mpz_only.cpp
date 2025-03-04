//
// Created by sluggernot on 2/27/25.
//

#include "mpz_only.h"
#include "MagicSquare_data.h"

bool isASquare(const mpz_int& testMe) {
    if (testMe < 0) return false;
#if VERBOSE
    std::cout << "Test Me: " << testMe << "\n";
#endif
    const mpz_int root = sqrt(testMe);
    return testMe == root*root;
}

mpz_only::mpz_only(const unsigned long long howMany) {
    std::cout << "Map max size: " << squares_set.max_size() << std::endl;
    std::cout << "Inserting   : " << howMany << std::endl;
    mpz_int squareRoot = 0;
    mpz_int squareVal = 0;
    maxValInContainer = howMany;
    for (unsigned long long i = 0; i < howMany; i++) {
        squareRoot = i;
        squareVal = squareRoot * squareRoot;
        squares_set[squareRoot] = squareVal;
    }
    std::cout << "Squares_set size: " << squares_set.size() << std::endl;
    std::cout << "SANITY CHECK: " << isASquare(squares_set.at(squares_set.size()-1)) << std::endl;
    //Just in case.
    currentVal = 1;
    boundingVal = 1;
}

void mpz_only::setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding) {
    boundingVal = bounding;
    currentVal = starting;

    if (boundingVal < 0) {boundingVal = 1;}
    if (currentVal < 0) {currentVal = 1;}
    if (currentVal < boundingVal) {currentVal = boundingVal;}
    std::cout << "Starting value: " << currentVal << " Bounding: " << boundingVal << std::endl;
    if (currentVal % boundingVal != 0) {
        currentVal = currentVal - (currentVal % boundingVal);
        if (currentVal % boundingVal != 0) { std::cout << "SANITY CHECK: I forgot how modulus worked?" << std::endl; }

        std::cout << "Adjusted starting value: " << currentVal << " Bounding: " << boundingVal << std::endl;
    }
}

void mpz_only::findAllEquidistantValues(const mpz_int& index, std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs, const std::map<mpz_int, mpz_int>* squares_set) {
    equidistPairs.clear();
    if (!squares_set->contains(index)) {std::cout << "Index not found: " << index << std::endl; return;}
    //Probably make the set a map.
    const mpz_int& idxVal = squares_set->at(index);
    mpz_int iterR = index+2;
    mpz_int iterL = index-2;
    mpz_int subtraction = squares_set->at(iterR)-idxVal;
    while (subtraction < idxVal) {
        while (squares_set->contains(iterL) && idxVal - squares_set->at(iterL) < subtraction) {
            iterL-=2;
        }
        if (iterL < 0) {std::cout << iterL << " out of range?" << std::endl; break;}//!squares_set->contains(iterL)
        if (idxVal - squares_set->at(iterL) == subtraction) {
            equidistPairs.emplace_back(squares_set->at(iterL), squares_set->at(iterR));
        }
        iterR+=2;
        iterL-=2;
        if (!squares_set->contains(iterR)) { std::cout << "WRITE CODE FOR EXPANDING MAP SIZE!" << iterR << std::endl; return;}
        subtraction = squares_set->at(iterR)-idxVal;
    }
}

void mpz_only::start() {
    while (!quit) {
        GivenAnIndexTestValue(currentVal);
        if (testEquidistantValsForSquares(currentVal, equidistant_vals, &squares_set)) {break;};
        currentVal+=boundingVal;

        ++counter;
        if (counter % 1000 == 0) { std::cout << currentVal << std::endl; } //So I can have some idea of where to pick up.
    }
}

mpz_int mpz_only::returnWorkerValAndReadyNext() {
    std::unique_lock<std::mutex> lock(gLocker);
    auto ret = currentVal;
    currentVal += boundingVal;
    if (counter++ % 1000 == 0) { std::cout << currentVal << std::endl; }
    return ret;
}

void mpz_only::GivenAnIndexTestValue(const mpz_int &index) {
    findAllEquidistantValues(index, equidistant_vals, &squares_set);
    if (equidistant_vals.size() >= mostEquidistants) {
        std::cout << "Met or exceeded highest equidistant count: " << index << "," << index*index << "," << equidistant_vals.size() << \
            " Largest value: " << equidistant_vals.at(equidistant_vals.size()-1).second <<  std::endl;
        mostEquidistants = equidistant_vals.size();
    }
    //fileOutput << index << "," << index*index << "," << equidistant_vals.size() << ",";
}

bool mpz_only::testEquidistantValsForSquares(const mpz_int& index, const std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs, const std::map<mpz_int, mpz_int>* squares_set) {
    if (equidistPairs.size() < 4) return false; //Need to have pairs for 2 diags and two tips of the cross
    if (equidistPairs.size() > 67) std::cout << index << " has " << equidistPairs.size() << " pairs. Largest val: " << equidistPairs.at(equidistPairs.size()-1).second << std::endl;

    const mpz_int& x = squares_set->at(index);
    for (int i = 0; i < equidistPairs.size()-3; i++) {
        const mpz_int a = x - equidistPairs[i].first;

        for (int j = i+2; j < equidistPairs.size()-2; j++) {
            const mpz_int b = x - equidistPairs[j].first;
            // New early out idea would check equidistPairs for the subtractions. No square roots.
            // Need a perf checker for some implementations and to know if multithreaded is faster or do I just wait on locks

            //BottomCenter
            const mpz_int xMinusAMinusB = x - a - b;
            //LeftCenter
            const mpz_int xPlusAMinusB = x + a - b;

            bool foundEquidistantCrossTips = false;
            for (int k = i+1; k < j; k++) {
                if (equidistPairs[k].first == xPlusAMinusB) {
                    foundEquidistantCrossTips = true; break;
                }
            }
            if (!foundEquidistantCrossTips)//I just want this info. Not pertinent to finding the real answer.
                for (int k = j+1; k < equidistPairs.size(); k++) {
                    if (equidistPairs[k].first == xMinusAMinusB) {
                        foundEquidistantCrossTips = true; break;
                    }
                }
            //shit cant just check squares_set.contains(). We are calculating the squareness of the value with x-a-b. Not the square root.
            if (foundEquidistantCrossTips)
            {
                std::cout << "Value has at least 3 sets working: " << index << " WOAH!" << std::endl;
                //TopCenter
                const mpz_int xPlusAPlusB = x + a + b;

                //RightCenter
                const mpz_int xMinusAPlusB = x - a + b;

                //x-a could be squares_set.at(equidistant_vals.at(i).first), x+a could be .second, x+-b can be at(j) but this is roughly the same since we basically never expect to get here.
                const MagicSquare_data checkMe(
                    x-a, xPlusAPlusB, x-b,
                    xPlusAMinusB, x, xMinusAPlusB,
                    x+b, xMinusAMinusB, x+a);
                checkMe.printMagicSquare_withSums(true);
                checkMe.printMagicSquareDetails();
                if (checkMe.isMagicSquare()) return true;

                std::cout << "IS NOT really a magic square, according to the check!\n";
                //Kinda should be impossible to hit. Returning true to ensure I investigate whatever happened here.
                return true;
            }
        }
    }
    return false;
}

void mpz_only::makeThreadsAndCalculate() {
    std::cout << "About to make the workers." << std::endl;
    constexpr int threadCount = 6;
    mpz_threadWorker worker_thread[threadCount];
    std::cout << "Starting threads..." << std::endl;

    //Make them all point to the precalculated data.
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i].t_squares_set_ptr = &squares_set;
    }
    std::cout << "Data is set." << std::endl;

    auto lambda = [this](mpz_threadWorker& worker) {
        while (true)
        {
            worker.t_currentVal = returnWorkerValAndReadyNext();
            findAllEquidistantValues(worker.t_currentVal, worker.t_equidistant_vals, worker.t_squares_set_ptr);
            if (testEquidistantValsForSquares(worker.t_currentVal, worker.t_equidistant_vals, worker.t_squares_set_ptr)) {
                std::cout << "found one" <<std::endl;
                break;
            }
        }
    };
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i].t_worker_thread = std::thread(lambda, std::ref(worker_thread[i]));
    }
    std::cout << "Threads are ready." << std::endl;
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i].t_worker_thread.join();
    }
    std::cout << "Threads are done." << std::endl;
}

void mpz_only::PrintAllDataGivenAValue(const mpz_int &index) {

    findAllEquidistantValues(index, equidistant_vals, &squares_set);
    std::cout <<"Index: " << index << " Value: " << squares_set.at(index) << "  Equidistant count: " <<  equidistant_vals.size() << std::endl;
    for (int i = 0; i < equidistant_vals.size(); i++) {
        mpz_int& lVal = equidistant_vals[i].first;
        mpz_int& rVal = equidistant_vals[i].second;
        std::cout << sqrt(lVal) << ", " << lVal << "  -  " << sqrt(rVal) << ", " << rVal << std::endl;
        if (isASquare(squares_set.at(index) - lVal)) std::cout <<"The difference to X: " << squares_set.at(index) << " - " << lVal << " is SQUARE ROOT.\n";
        if (isASquare(rVal - lVal)) std::cout << "The difference between equidistants, themselves: " << sqrt(rVal - lVal) << " is SQUARE ROOT.\n";

        std::cout << "\tDifference from X to values: " << squares_set.at(index) - lVal << " How many sqrts away: " << index - sqrt(lVal) << " " << sqrt(rVal) - index << std::endl;
        if (i > 0) {
            std::cout << "\tDifference to prev-L: " << equidistant_vals[i-1].first - lVal  << " Indices: " << sqrt(equidistant_vals[i-1].first)-sqrt(lVal)  << std::endl;
            std::cout << "\tDifference to prev-R: " << rVal - equidistant_vals[i-1].second << " Indices: " << sqrt(rVal)-sqrt(equidistant_vals[i-1].second) << std::endl;
        }
        std::cout << std::endl;
    }
    //Print "closest" magic square of square. This would be the configuration where the tips of the cross are the closest to being square as possible.
    //Second closest?
    for(int i = 0; i < equidistant_vals.size(); i++) {
        
    }
}
