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
    std::cout << "SANITY CHECK: " << (((howMany/2) * (howMany/2)) == squares_set.at(howMany/2)) << std::endl;
    //Just in case.
    currentVal = 1;
    boundingVal = 1;
}

void mpz_only::setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding) {
    boundingVal = bounding;
    currentVal = starting;

    if (boundingVal < 1) {boundingVal = 1;}
    if (currentVal < 1) {currentVal = 1;}
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
    //The real limit is 2.414 * index but we can live with this.
    if (index * 2 > squares_set->crbegin()->first) {std::cout << "We have exceeded the limit of our map: " << index << std::endl; return;}

    //About to make changes to try add instead?
    const mpz_int twoX = squares_set->at(index) * 2;
    mpz_int iterR = index+1;
    mpz_int iterL = index;

    mpz_int iterRv = squares_set->at(iterR);
    while (iterRv < twoX) {
        while (iterL >= 0 && twoX < squares_set->at(--iterL) + iterRv) {}
        if (iterL < 0) { break; }
        if (squares_set->at(iterL) + iterRv == twoX) {
            equidistPairs.emplace_back(squares_set->at(iterL), iterRv);
        }
        iterRv = squares_set->at(++iterR);//If it dies we were out of values.
        // if (!squares_set->contains(iterR)) { std::cout << "WRITE CODE FOR EXPANDING MAP SIZE!" << iterR << std::endl; return;}
    }
}

void mpz_only::start() {
    while (!quit) {
        GivenAnIndexTestValue(currentVal);
        if (testEquidistantValsForSquares(currentVal, equidistant_vals)) {break;};
        currentVal+=boundingVal;

        ++counter;
        if (counter % 1000 == 0) { std::cout << currentVal << std::endl; } //So I can have some idea of where to pick up.
    }
}

void mpz_only::returnWorkerValAndReadyNext(mpz_int& index) {
    //Consider just bounding self here. No mutex lock. So if I start with INDEX and += bounding. I can't match some other value. Except where bounding < thread count?
    ++counter;
    //This is "faster" but certain threads in specific multiples can lag WAY behind other threads with simpler multiples.
    // if (counter > threadNum) {
    //     if (index % 1000 == 0) { std::cout << index << " about to do: " << index+boundingVal*threadNum << std::endl; }
    //     index = index+boundingVal*threadNum;//Everyone is boundingVal*threads apart.
    //     return;
    // }

    std::unique_lock<std::mutex> lock(mpzOnlyMutex);
    const auto ret = currentVal;
    currentVal += boundingVal;
    if (counter % 1000 == 0) { std::cout << currentVal << std::endl; }
    index = ret;
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

bool mpz_only::testEquidistantValsForSquares(const mpz_int& index, const std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs) {
    if (equidistPairs.size() < 4) return false; //Need to have 4 pairs for 2 diags and two tips of the cross
    if (equidistPairs.size() > 67) std::cout << index << " has " << equidistPairs.size() << " pairs. Largest val: " << equidistPairs.at(equidistPairs.size()-1).second << std::endl;
    // if (index%1000 == 0) {
    //     const mpz_int x = index * index;//squares_set->at(index);
    //     const auto bottomPair =equidistPairs.at(equidistPairs.size()-1);
    //     std::cout << "Sanity check " << ((bottomPair.second - bottomPair.first) / 8)*4 + bottomPair.first - x << std::endl;
    //     std::cout << "Sanity check " << bottomPair.second - x << "  " << bottomPair.first - x << std::endl;
    //     std::cout << "Sanity check " << bottomPair.second + bottomPair.first - x - x << std::endl;
    // }
#ifdef TEST1
    //I was able to prove this is bad for testing. The distance between all teh values does NOT have to be the same.
    for (int i = equidistPairs.size()-1; i >= 3; i--) {
        //Get a distance of "1" from lowest. If equidist pairs contains first_plus_one check for plus two and three. That would be yahtzee.
        const mpz_int one_unit = ((equidistPairs.at(i).second - equidistPairs.at(i).first) / 8);
        const mpz_int first_plus_one = one_unit + equidistPairs.at(i).first;
        for (int j = i-1; j >= 2; j--) {
            if (equidistPairs.at(j).first > first_plus_one+one_unit) { break;}
            if (equidistPairs.at(j).first != first_plus_one) { continue;}
            std::cout << "Found a buddy: " << index << "\n";
            const mpz_int second_plus_one = equidistPairs.at(j).first + one_unit;
            for (int k = j-1; k < equidistPairs.size(); k--) {
                if (equidistPairs.at(k).first > second_plus_one+one_unit) { break;}
                if (equidistPairs.at(k).first != second_plus_one) { continue;}
                std::cout << "Found a twofer: " << index << "\n";
                const mpz_int third_plus_one = equidistPairs.at(k).first + one_unit;
                for (int l = k+1; l < equidistPairs.size(); l++) {
                    if (equidistPairs.at(k).first > second_plus_one+one_unit) { break;}
                    if (equidistPairs.at(k).first != second_plus_one) { continue;}
                    std::cout << "Shouldbe a magic square of squares?! " << index << "\n";
                    return true;
                }
            }
        }
    }
    return false;
#endif

    //Potential best checking strat: Start at equidist[last] and try to find a pair where .first + .second + a different .second = 3X.
    for (int i = equidistPairs.size()-1; i >= 2; i--) {
        const mpz_int& bot_center = equidistPairs.at(i).first;
        const mpz_int& top_center = equidistPairs.at(i).second;
        for (int j = 0; j < i; j++) {
            const mpz_int botPlusA = bot_center + equidistPairs.at(j).second;
            const mpz_int topPlusA = top_center + equidistPairs.at(j).first;
            for (int k = j+1; k < i; k++) {
                // if (abs(botPlusA + equidistPairs.at(k).second - threeX) < 1000) {
                //     std::cout << "Index: " << index << " had a near miss bot row: " << abs(botPlusA + equidistPairs.at(k).second - threeX) << std::endl;
                // }
                if (topPlusA+equidistPairs.at(k).first == botPlusA+equidistPairs.at(k).second) {
                    std::cout << "FOUND 3 PAIRS, EQUIDISTANT FROM INDEX:" << index << std::endl;
                }
                else continue;
                std::cout << "Found a twofer: Recode the shit after this line." << index << "\n";

                const mpz_int x = index * index;//squares_set->at(index);
                const mpz_int threeX = x*3;//Doubt we need this at all.
                const mpz_int PlusAMinusB = equidistPairs.at(j).second + equidistPairs.at(k).first;
                for (int l = 0; l < equidistPairs.size(); l++) {
                    if (equidistPairs.at(l).first + PlusAMinusB == threeX || equidistPairs.at(l).second + PlusAMinusB == threeX) {
                        std::cout << "HOLY SHIT! Go hog wild and get some logic to test this shit. It may be that we have fucked it up:" << index << std::endl;
                        std::cout << "Top and bot center: " << equidistPairs.at(i).first << "  " << equidistPairs.at(i).second << std::endl;
                        std::cout << "Unsure: " << equidistPairs.at(j).first << "  " << equidistPairs.at(j).second << " - " <<
                                                   equidistPairs.at(k).first << "  " << equidistPairs.at(k).second << " - " <<
                                                   equidistPairs.at(l).first << "  " << equidistPairs.at(l).second << " - " << std::endl;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void mpz_only::makeThreadsAndCalculate() {
    std::cout << "About to make the workers." << std::endl;
    constexpr int threadCount = 14;
    mpz_threadWorker worker_thread[threadCount];
    std::cout << "Starting threads..." << std::endl;

    //Make them all point to the precalculated data.
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i].t_squares_set_ptr = &squares_set;
        worker_thread[i].t_threadNum = i;
    }
    std::cout << "Data is set." << std::endl;

    auto lambda = [this](mpz_threadWorker& worker) {
        while (true)
        {
            if (++worker.t_threadIterCounter % 1000==0){ std::cout << "Thread " << worker.t_threadNum << " did 1000: on index: " << worker.t_currentVal << std::endl;}

            returnWorkerValAndReadyNext(worker.t_currentVal);
            findAllEquidistantValues(worker.t_currentVal, worker.t_equidistant_vals, worker.t_squares_set_ptr);
            if (testEquidistantValsForSquares(worker.t_currentVal, worker.t_equidistant_vals)) {
                std::cout << "found one" <<std::endl;
                break;
            }
        }
    };
    threadNum = threadCount;
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
    std::cout <<"Index: " << index << " Value: " << squares_set.at(index) << "  Equidistant count: " <<  equidistant_vals.size() << "\n";
//    return;
    for (int i = 0; i < equidistant_vals.size(); i++) {
        mpz_int& lVal = equidistant_vals[i].first;
        mpz_int& rVal = equidistant_vals[i].second;
        std::cout << sqrt(lVal) << ", " << lVal << "  -  " << sqrt(rVal) << ", " << rVal << "\n";
        if (isASquare(squares_set.at(index) - lVal)) std::cout <<"The difference to X: " << squares_set.at(index) << " - " << lVal << " is SQUARE ROOT.\n";
        if (isASquare(rVal - lVal)) std::cout << "The difference between equidistants, themselves: " << sqrt(rVal - lVal) << " is SQUARE ROOT.\n";

        std::cout << "\tDifference from X to values: " << squares_set.at(index) - lVal << " How many sqrts away: " << index - sqrt(lVal) << " " << sqrt(rVal) - index << "\n";
        if (i > 0) {
            std::cout << "\tDifference to prev-L: " << equidistant_vals[i-1].first - lVal  << " Indices: " << sqrt(equidistant_vals[i-1].first)-sqrt(lVal)  << "\n";
            std::cout << "\tDifference to prev-R: " << rVal - equidistant_vals[i-1].second << " Indices: " << sqrt(rVal)-sqrt(equidistant_vals[i-1].second) << "\n";
        }
        std::cout << "\n";
    }
    //Print "closest" magic square of square. This would be the configuration where the tips of the cross are the closest to being square as possible.
    //Second closest?
    const mpz_int& x = squares_set.at(index);
    const mpz_int threex = x+x+x;
    mpz_int closest = equidistant_vals.at(equidistant_vals.size()-1).second;
    mpz_int total = 0;//equidistant_vals.at(equidistant_vals.size()-1).second;
    mpz_int closestA = 0;//equidistant_vals.at(equidistant_vals.size()-1).second;
    mpz_int totalA = 0;//equidistant_vals.at(equidistant_vals.size()-1).second;
    int Xa, Xb, Xc, Xd;
    int XAa, XAb, XAc, XAd;
    for(int i = 0; i < equidistant_vals.size(); i++) {
        for (int j = 0; j < equidistant_vals.size(); j++) {
            if (j == i) continue;
            for (int k = 0; k < equidistant_vals.size(); k++) {
                if (k == i || k == j) { continue;}
                const mpz_int a = x - equidistant_vals.at(i).first;
                const mpz_int b = x - equidistant_vals.at(j).first;
                const mpz_int MinusAMinusB = x-a-b;
                const mpz_int PlusAMinusB =  x+a-b;
                const mpz_int PlusAPlusB =  x+a+b;

                for (int l = 0; l < equidistant_vals.size(); l++) {
                    if (l == i || l == j || l == k) { continue;}

                    total = abs(MinusAMinusB-equidistant_vals.at(l).first) + abs(PlusAMinusB-equidistant_vals.at(k).first);
                    if (total < closest) {
                        std::cout << total << std::endl;
                        closest = total;
                        Xa = i; Xb = j; Xc = k; Xd = l;
                    }
                    //Also make an: x + a - b + PlusAMinusB - 3x closest to 0 total and shit and see if theyre the same
                    totalA = abs(threex - PlusAPlusB - equidistant_vals.at(k).first);
                    totalA += abs(threex - MinusAMinusB + equidistant_vals.at(l).first);
                    if (closestA == 0 || totalA < closestA) {
                        closestA = totalA;
                        XAa = i; XAb = j; XAc = k; XAd = l;
                    }

                }
            }
        }
    }
    std::cout << "We want each Sum to add to : " << threex << std::endl;
    bool didntMatch = false;
    if (Xa != XAa || Xb != XAb || Xc != XAc || Xd != XAd) {
        std::cout << "Some indices didnt match both tests?! : " << Xa << " " << XAa << " - " \
        << Xb << " " << XAb << " - " << Xc << " " << XAc << " - " << Xd << " " << XAd << std::endl;
        didntMatch = true;
    }
    std::cout <<"\nIndices: " << "A: " << Xa << " B: " << Xb << " C: " << Xc << " Xd: " << Xd << std::endl;
    const MagicSquare_data checkMe(
                    equidistant_vals.at(Xa).first, equidistant_vals.at(Xd).second, equidistant_vals.at(Xb).first,
                    equidistant_vals.at(Xc).first, x,                              equidistant_vals.at(Xc).second,
                    equidistant_vals.at(Xb).second, equidistant_vals.at(Xd).first, equidistant_vals.at(Xa).second);
    checkMe.printMagicSquare_withSums(true);
    checkMe.printMagicSquareDetails();
    if (!didntMatch) { return; }

    std::cout <<"\nIndices: " << "A: " << XAa << " B: " << XAb << " C: " << XAc << " Xd: " << XAd << std::endl;
    const MagicSquare_data checkMe2(
                    equidistant_vals.at(XAa).first, equidistant_vals.at(XAd).second, equidistant_vals.at(XAb).first,
                    equidistant_vals.at(XAc).first, x,                               equidistant_vals.at(XAc).second,
                    equidistant_vals.at(XAb).second, equidistant_vals.at(XAd).first, equidistant_vals.at(XAa).second);
    checkMe2.printMagicSquare_withSums(true);
    checkMe2.printMagicSquareDetails();

}

void mpz_only::isOneDouble(const mpz_int& startingPlace = 0) const {

    mpz_int closest_diff = 1;//squares_set.at(maxValInContainer-1); //Stupidly large number to start with
    //mpz_int temp_diff = squares_set.at(maxValInContainer-1); //Stupidly large number to start with
    mpz_int closestIdxHalf = 0;
    mpz_int closestIdxDoub = 0;

    mpz_int startingPoint = 10;
    if (startingPlace > 10) { startingPoint = startingPlace; }
    for (mpz_int i = startingPoint; i < squares_set.size(); ++i ) {
        const mpz_int twoTimes = squares_set.at(i)*2;
        for (mpz_int j = i+1; j < squares_set.size(); ++j) {
            if (squares_set.at(j) > twoTimes+twoTimes/4) { break; } //Some arbitrary allowance to go over but not go to end of all squares.
            if (squares_set.at(j) == twoTimes) { //Impossible. This is more of a sanity check.
                std::cout << "Wtf? Exactly double? Index: " << i << " is half of index: " << j << std::endl;
                return;
            }
            if (abs(squares_set.at(j) - twoTimes) <= closest_diff) {
                closest_diff = abs(squares_set.at(j) - twoTimes);
                closestIdxHalf = i;
                closestIdxDoub = j;
                std::cout << "Half: idx: " << i << " val: " << squares_set.at(i) << " It's doub: idx:" << j << " val: " << squares_set.at(j) << " ValDiff: " << squares_set.at(j) - twoTimes << std::endl;
                i = i*2 + (i/10*4);//sqrt(squares_set.at(i)*5);
                break;
            }
        }
    }
    if (closestIdxHalf == startingPlace) { std::cout << "Start and closest to half were the same \n"; }
    std::cout << "Closest indices to being double: " << closestIdxHalf << " roughly half of idx: " << closestIdxDoub << "  with a diff of ";
    std::cout << abs(squares_set.at(closestIdxHalf) *2 - squares_set.at(closestIdxDoub)) << " " << std::endl;
}
