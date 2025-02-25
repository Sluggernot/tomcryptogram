//
// Created by sluggernot on 2/21/25.
//

#ifndef MAGICSQUARE_DATA_H
#define MAGICSQUARE_DATA_H

#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;

enum val_loc {
    top_lft, top_cen, top_rgt,
    cen_lft, cen_cen, cen_rgt,
    bot_lft, bot_cen, bot_rgt,
    LAST_VAL
};

enum sum_loc {
    row_top, row_cen, row_bot,
    col_lft, col_cen, col_rgt,
    diag_ullr, diag_blur,
    LAST_SUM
};

class MagicSquare_data {
private:
    //.first = base number .second = square of .first
    std::pair<mpz_int, mpz_int> square_arr[9];
    mpz_int sums[8];//3 rows, 3 columns, Upleft to low right, bot left to upright
    mpz_int most_common_sum = 0;
    int current_focus = -1;

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
    MagicSquare_data(unsigned long long a, unsigned long long b, unsigned long long c, unsigned long long d,
        unsigned long long e, unsigned long long f, unsigned long long g, unsigned long long h, unsigned long long i);

    int getCurrentFocus() const {return current_focus;}

    void printMagicSquare() const;
    void printMagicSquare_withSums(const bool diags = true) const;
    void printMagicSquareDetails() const;
    char printLocation(const int index) const;
    bool isMagicSquare() const;
    bool contains(mpz_int& value, const int excludeIndex = -1) const;
    bool areUncommonSumsHigher() const;
    int isThereACommonElementFromBadSums();

    //Probably not useful to arbitrarily pick the lowest value.
    void swapLowest(const int counter);
    void swapTwoIndices(const int swap1, const int swap2);
    //Just finds the lowest of the 9 values and returns the index
    int getLowestValuesIndex() const;

    //Takes an index and increments or decrements by 1, by raising or lowering .first and then squaring it
    //Saves us from using sqrt, ever so far.
    void incrementAValueAtIndex(const int index, bool whileNotInArray = true);
    void decrementAValueAtIndex(const int index, bool whileNotInArray = true);

    mpz_int calculateMostCommonSum(bool print);

    void initializeAsAntiParkerSquare(unsigned long long initialVal, bool isAlreadySquared/* = false*/);
    bool isIndexSquare(int index);//Saves the sqrt result into .first
    static bool isNumberSquare(mpz_int& isSquare){mpz_int checker = sqrt(isSquare); return checker*checker == isSquare; };

};



#endif //MAGICSQUARE_DATA_H
