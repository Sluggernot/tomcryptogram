//
// Created by sluggernot on 2/24/25.
//

#include "squares_container.h"

#include <fstream>
#include <thread>

#define VERBOSE 0

squares_container::squares_container(unsigned long long howMany)
{
    for (unsigned long long i = 0; i < howMany; i++) {
        squares_vec.emplace_back(i*i);
    }
    squares_ptr = &squares_vec;
#if VERBOSE
    std::cout << "Biggest square: " << squares[howMany-1] << std::endl;
#endif
}

unsigned int squares_container::findSquareMatchingDistance(const unsigned int index, const mpz_int& distance) const {

    const mpz_int& atIndex = squares_ptr->at(index);
    for (long long int i = index-1; i >= 0; i--) {
        const mpz_int subtraction = atIndex - squares_ptr->at(i);
        if (subtraction == distance) {
            return i;
        }
        if (subtraction > distance || squares_ptr->at(i) == 0) { break; }
    }
    return index;
}

unsigned long squares_container::findAllEquidistantValues(const unsigned int startingIndex) {
    equidistant_vals.clear();

    //Cant be unsigned Needs to check against -1. Could be if we check against startingIndex as a checker
    long long int index = 0;
    const mpz_int subCheck = squares_ptr->at(startingIndex)-squares_ptr->at(0);
    for (unsigned int i = startingIndex+1; i < squares_ptr->size(); i++) {
        const mpz_int subtraction = squares_ptr->at(i)-squares_ptr->at(startingIndex);
        if (subtraction > subCheck) {break;}

        index = findSquareMatchingDistance(startingIndex, subtraction);
        if (index != startingIndex) {
#if VERBOSE
            std::cout << "Equidistant values: " << index << ", " << i << "\n";
#endif
            equidistant_vals.emplace_back(index, i);
        }
    }
    return equidistant_vals.size();
}

bool squares_container::testEquidistantValsForSquares(const unsigned int center,  const unsigned int distance_idx) const {
    //Generating the magic square
    //https://www.youtube.com/watch?v=uz9jOIdhzs0 11:54
    // std::cout << "\n\n";

    for (unsigned int i = distance_idx; i < equidistant_vals.size()-1; i++) {
        const mpz_int& x = squares_ptr->at(center);

        const mpz_int a = x - squares_ptr->at(equidistant_vals.at(distance_idx).first);

        const mpz_int b = x - squares_ptr->at(equidistant_vals.at(distance_idx+1).first);

        //TopCenter
        const mpz_int xPlusAPlusB = x + a + b;
        //LeftCenter
        const mpz_int xPlusAMinusB = x + a - b;
        //RightCenter
        const mpz_int xMinusAPlusB = x - a + b;
        //BottomCenter
        const mpz_int xMinusAMinusB = x - a - b;

        if (xMinusAMinusB > 0)
        {
            int squaresTotal = 0;
            if(isASquare(xPlusAPlusB))squaresTotal++;
            if(isASquare(xPlusAMinusB))squaresTotal++;
            if(isASquare(xMinusAPlusB))squaresTotal++;
            if(isASquare(xMinusAMinusB))squaresTotal++;

            if (squaresTotal > 1)
            {
                if (squaresTotal > 3)
                    std::cout << "THIS SHOULD BE A MAGIC SQUARE OF SQUARES!\n";
                else {
                    std::cout <<"Val: " << center <<  " squared.  Squares count: " << squaresTotal << " plus 5 given squares. \n";
                    return false;
                }
                std::cout << "Equidistant index1: " << equidistant_vals.at(distance_idx).first << "\n";
                std::cout << "Equidistant index2: " << equidistant_vals.at(distance_idx).second << "\n";
                std::cout << "X: " << x << "\n";
                std::cout << "A: " << a << "\n";
                std::cout << "B: " << b << "\n";

                std::cout << x-a << " " << xPlusAPlusB << " " << x-b << std::endl;
                std::cout << xPlusAMinusB << " " << x << " " << xMinusAPlusB << std::endl;
                std::cout << x+b << " " << xMinusAMinusB << " " << x+a << "\n\n";

                return true;
            }
        }
    }
    return false;
}

bool squares_container::isASquare(const mpz_int& testMe) {
    if (testMe < 0) return false;
#if VERBOSE
    std::cout << "Test Me: " << testMe << "\n";
#endif
    const mpz_int root = sqrt(testMe);
    return testMe == root*root;
}

void squares_container::makeThreadsAndCalculate() {
    //Thread stuff init
    std::atomic<int> atomic_counter(90000-1); //<<--- THIS IS THE STARTING NUMBER - 79000 last recorded number we left off at
    std::cout << "Starting with: " << atomic_counter << "\n";
    constexpr int threadCount = 6;
    std::thread worker_thread[threadCount];
    squares_container worker_data[threadCount];

    //Set pointers to this massive data.
    for (int i = 0; i < threadCount; i++) {
        worker_data[i].setPointerToSquares(*this);
    }
    std::cout << "Data built\n";

    auto lambda = [this, &atomic_counter](squares_container& data) {
        ++atomic_counter;
        while (atomic_counter < squares_vec.size()/2)
        {
            GivenAnIndexTestValue(atomic_counter, data);
            ++atomic_counter;
            if (atomic_counter % 1000 == 0)
            {
                std::cout << " Number: " << atomic_counter << "\n";
            }
        }
    };
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i] = std::thread(lambda, std::ref(worker_data[i]));
    }
    for (int i = 0; i < threadCount; i++) {
        worker_thread[i].join();
    }

    std::cout << "\nTotal count: " << atomic_counter << std::endl;
}

void squares_container::GivenAnIndexTestValue(const unsigned int index, squares_container& data) {
    const int eqValCount = data.findAllEquidistantValues(index);
    fileOutput << index << "," << index*index << "," << eqValCount << ",";
    if (eqValCount > 1) {
        for (int eqs = 0; eqs < eqValCount-1; ++eqs) {
            fileOutput<< data.getEquidistant_valAtIndex(eqs).first << "," << data.getEquidistant_valAtIndex(eqs).second << ",";
            if (data.testEquidistantValsForSquares(index, eqs)) {
                std::stringstream fileName; fileName << "MultipleSquareEqs_" << index << ".txt";
                std::ofstream file(fileName.str().c_str());
                std::cout << "WOAH! CHECK OUT " << index << std::endl;
                std::cout << "Iteration: " << index << " EQ Count: " << eqValCount << "\n";
            }
        }
        fileOutput << data.getEquidistant_valAtIndex(eqValCount-1).first << "," << data.getEquidistant_valAtIndex(eqValCount-1).second << "\n";

        std::unique_lock<std::mutex> lock(gLocker);
        if (index % 1000 == 0) {
            std::stringstream fileName;
            fileName << index-1000 << "-" << index << "_" << GetTimestamp() << ".csv";
            std::ofstream file(fileName.str().c_str());
            file << fileOutput.str() << "\n";
            fileOutput.str("");   // Clear the contents
            fileOutput.clear();
        }
    }
    else {
        if (eqValCount > 0) {
            fileOutput<< data.getEquidistant_valAtIndex(0).first << "," << data.getEquidistant_valAtIndex(0).second << ",";
        }
        fileOutput << "\n";
    }
}

