//
// Created by sluggernot on 2/21/25.
//

#ifndef MAGICSQUARE_DATA_H
#define MAGICSQUARE_DATA_H

#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;

class MagicSquare_data {
private:

    std::pair<mpz_int, mpz_int> square_arr[9];

    //Helper func to get us so initialization
    void initializeSquares(std::pair<mpz_int, mpz_int>* square_arr);

public:

    MagicSquare_data() {
        //These magic numbers are just a starting point from: https://www.youtube.com/watch?v=2Twa-z_WPE4
        square_arr[0] = std::make_pair(1, 5976563248318113025ULL);
        square_arr[1] = std::make_pair(1, 15260232026299743025ULL);
        square_arr[2] = std::make_pair(1, 2577330938902409089ULL);
        square_arr[3] = std::make_pair(1, 4537991882387127025ULL);
        square_arr[4] = std::make_pair(1, 7937944177203705025ULL);
        square_arr[5] = std::make_pair(1, 11337896472020283025ULL);
        square_arr[6] = std::make_pair(1, 13298557415505000961ULL);
        square_arr[7] = std::make_pair(1, 615656328107667025ULL);
        square_arr[8] = std::make_pair(1, 9899325106089297025ULL);
        initializeSquares(square_arr);
    }

    void printMagicSquare() const;
    void printMagicSquare_withSums(const bool diags = true) const;
    void printMagicSquareDetails() const;
    char printLocation(const int index) const;
    bool isMagicSquare() const;

    //Probably not useful to arbitrarily pick the lowest value.
    void swapLowest(const int counter);
    //Just finds the lowest of the 9 values and returns the index
    int getLowestValuesIndex() const;

    //Takes an index and increments or decrements by 1, by raising or lowering .first and then squaring it
    //Saves us from using sqrt, ever so far.
    void incrementAValueAtIndex(const int index);
    void decrementAValueAtIndex(const int index);


};



#endif //MAGICSQUARE_DATA_H
