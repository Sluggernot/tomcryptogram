//
// Created by sluggernot on 2/24/25.
//

#ifndef SQUARES_CONTAINER_H
#define SQUARES_CONTAINER_H

#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;


class squares_container {
private:

    std::vector<mpz_int> squares;
    std::vector<std::pair<int, int>> equidistant_vals;
    std::vector<std::pair<int, int>> best_vals;

public:
    explicit squares_container(unsigned int howMany = 1000);

    int findSquareMatchingDistance(const int index, const mpz_int& distance) const;

    //Given a number, iterate the container, looking for double equidistant values
    //Dont forget to look for more values as we try out the ones we have
    void findAllEquidistantValues(const int startingIndex);
    int getEquidistantCount() const{ return equidistant_vals.size(); };

    bool testEquidistantValsForSquares(const int center, const int distance_idx) const;

    static bool isASquare(const mpz_int testMe);

//    mpz_int getValue(int index) const{ return squares.at(index); };
};

#endif //SQUARES_CONTAINER_H
