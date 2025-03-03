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
    const mpz_int idxVal = squares_set->at(index);
    mpz_int iterR = index+1;
    mpz_int iterL = index-1;
    mpz_int subtraction = squares_set->at(iterR)-idxVal;
    if (!squares_set->contains(iterR)) {
        //Add vals to squares_set until we are above iterR by 1? Leaving this in here until I need it so I can see what values
        std::cout << "WRITE CODE FOR EXPANDING MAP SIZE!" << iterR << std::endl;
        return;
    }
    while (subtraction < idxVal) {
        while (squares_set->contains(iterL) && idxVal - squares_set->at(iterL) < subtraction) {
            --iterL;
        }
        if (!squares_set->contains(iterL)) {break;}
        if (idxVal - squares_set->at(iterL) == subtraction) {
            equidistPairs.emplace_back(iterL, iterR);
        }
        ++iterR;
        ++iterL;
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

bool mpz_only::testEquidistantValsForSquares(const mpz_int& index, std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs, const std::map<mpz_int, mpz_int>* squares_set) {
    if (equidistPairs.size() < 2) return false;
    if (equidistPairs.size() > 60) std::cout << index << " has " << equidistPairs.size() << " pairs.\n";

    const mpz_int& x = squares_set->at(index);
    for (unsigned int i = 0; i < equidistPairs.size()-1; i++) {
        const mpz_int a = x - squares_set->at(equidistPairs.at(i).first);

        for (unsigned int j = i+1; j < equidistPairs.size(); j++) {

            const mpz_int b = x - squares_set->at(equidistPairs.at(j).first);

            //BottomCenter
            const mpz_int xMinusAMinusB = x - a - b;
            //shit cant just check squares_set.contains(). We are calculating the squareness of the value with x-a-b. Not the square root.
            if (isASquare(xMinusAMinusB))
            {
                //TopCenter
                const mpz_int xPlusAPlusB = x + a + b;
                //LeftCenter
                const mpz_int xPlusAMinusB = x + a - b;
                //RightCenter
                const mpz_int xMinusAPlusB = x - a + b;

                int squaresTotal = 1;
                if(isASquare(xPlusAPlusB) )++squaresTotal;
                if(isASquare(xPlusAMinusB))++squaresTotal;
                if(isASquare(xMinusAPlusB))++squaresTotal;

                if (squaresTotal > 1)
                {
                    if (squaresTotal > 3)
                        std::cout << "THIS SHOULD BE A MAGIC SQUARE OF SQUARES!\n";
                    else {
                        std::cout <<"FINALLY FOUND 2 OR MORE SQUARES: " << index <<  " squared.  Squares count: " << squaresTotal << " plus 5 given squares. \n";
                        continue;
                    }
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
            findAllEquidistantValues(currentVal, worker.t_equidistant_vals, worker.t_squares_set_ptr);
            if (testEquidistantValsForSquares(worker.t_currentVal, worker.t_equidistant_vals, worker.t_squares_set_ptr)) {break;}
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
