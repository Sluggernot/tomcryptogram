//
// Created by sluggernot on 2/24/25.
//

#ifndef SQUARES_CONTAINER_H
#define SQUARES_CONTAINER_H

#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;


class squares_container {
private:

    std::vector<mpz_int> squares_vec;
    std::vector<mpz_int>* squares_ptr = nullptr;
    std::vector<std::pair<unsigned long long, unsigned long long>> equidistant_vals;
    //std::vector<std::pair<int, int>> best_vals;

public:
    explicit squares_container(unsigned long long howMany = 0);

    std::vector<mpz_int>* getPointerToSquares() { return &squares_vec; };
    void setPointerToSquares(squares_container& inContainer){squares_ptr = inContainer.getPointerToSquares();};

    //Returns an index of a matching square distance. Or -1 if none found.
    unsigned int findSquareMatchingDistance(const unsigned int index, const mpz_int& distance) const;

    //Given a number, iterate the container, looking for double equidistant values
    //Returns count of how many pairs of equidistance values there are
    int findAllEquidistantValues(const unsigned int startingIndex);
    // int getEquidistantCount() const{ return equidistant_vals.size(); };

    bool testEquidistantValsForSquares(const unsigned int center, const unsigned int distance_idx) const;

    static bool isASquare(const mpz_int& testMe);

    static void GivenAnIndexTestValue(unsigned int index, squares_container& data);

//    mpz_int getValue(int index) const{ return squares.at(index); };
};

inline void squares_container::GivenAnIndexTestValue(unsigned int index, squares_container& data) {
    int eqValCount = data.findAllEquidistantValues(index);
    // std::cout << "Iteration: " << i << " EQ Count: " << squares.getEquidistantCount() << "\n";
    if (eqValCount > 1) {
        if (eqValCount > 13)
            std::cout <<"XVal: " << index*index << " EquidistantCount: " << eqValCount << "\n";

        for (int eqs = 0; eqs < eqValCount-1; ++eqs) {
            if (data.testEquidistantValsForSquares(index, eqs)) {
                std::cout << "WOAH! CHECK OUT " << index << std::endl;
                std::cout << "Iteration: " << index << " EQ Count: " << eqValCount << "\n";
            }
        }
    }
}

#endif //SQUARES_CONTAINER_H
