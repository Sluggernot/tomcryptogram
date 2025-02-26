//
// Created by sluggernot on 2/24/25.
//

#include "squares_container.h"

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
    // mpz_int subtraction = 0;
    const mpz_int& atIndex = squares_ptr->at(index);
    for (unsigned int i = index-1; i >= 0; i--) {
        const mpz_int subtraction = atIndex - squares_ptr->at(i);
        if (subtraction == distance) {
            return i;
        }
        if (subtraction > distance || squares_ptr->at(i) == 0) { break; }
    }
    return -1;
}

int squares_container::findAllEquidistantValues(const unsigned int startingIndex) {
    equidistant_vals.clear();

    int index = 0;
    // mpz_int subtraction;
    const mpz_int subCheck = squares_ptr->at(startingIndex)-squares_ptr->at(0);
    for (unsigned int i = startingIndex+1; i < squares_ptr->size(); i++) {
        const mpz_int subtraction = squares_ptr->at(i)-squares_ptr->at(startingIndex);
        if (subtraction > subCheck) {break;}

        index = findSquareMatchingDistance(startingIndex, subtraction);
        if (index != -1) {
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

            if ( squaresTotal >= 2)
            {
                if (squaresTotal > 3)
                    std::cout << "THIS SHOULD BE A MAGIC SQUARE OF SQUARES!\n";
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



