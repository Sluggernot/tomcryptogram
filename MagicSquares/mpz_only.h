//
// Created by sluggernot on 2/27/25.
//

#ifndef MPZ_ONLY_H
#define MPZ_ONLY_H

#include <map>
#include <boost/multiprecision/gmp.hpp>

#include "squares_container.h"

using namespace boost::multiprecision;


class mpz_only {

    std::map<mpz_int, mpz_int> squares_set;
    std::vector<std::pair<mpz_int, mpz_int>> equidistant_vals;
    mpz_int startingVal, boundingVal, maxValInContainer;
    bool quit = false;
    int counter = 0;
    unsigned long mostEquidistants = 14;

    std::stringstream fileOutput;

public:

    explicit mpz_only(const unsigned long long howMany = 0);

    void setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding);

    //findAllEquidistantValues
    void findAllEquidistantValues(const mpz_int& index);

    void start();

private:
    void GivenAnIndexTestValue(const mpz_int& index);
    bool testEquidistantValsForSquares() const;

};

#endif //MPZ_ONLY_H
