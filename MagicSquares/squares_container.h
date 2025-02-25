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
    std::vector<std::pair<int, int>> equidistant_vals;
    //std::vector<std::pair<int, int>> best_vals;

public:
    explicit squares_container(unsigned int howMany = 0);

    std::vector<mpz_int>* getPointerToSquares() { return &squares_vec; };
    void setPointerToSquares(squares_container& inContainer){squares_ptr = inContainer.getPointerToSquares();};

    int findSquareMatchingDistance(const int index, const mpz_int& distance) const;

    //Given a number, iterate the container, looking for double equidistant values
    //Dont forget to look for more values as we try out the ones we have
    int findAllEquidistantValues(const int startingIndex);
    int getEquidistantCount() const{ return equidistant_vals.size(); };

    bool testEquidistantValsForSquares(const int center, const int distance_idx) const;

    static bool isASquare(const mpz_int testMe);

    static void GivenAnIndexTestValue(unsigned int index, squares_container& data);

//    mpz_int getValue(int index) const{ return squares.at(index); };
};

inline void squares_container::GivenAnIndexTestValue(unsigned int index, squares_container& data) {
    int eqValCount = data.findAllEquidistantValues(index);
    // std::cout << "Iteration: " << i << " EQ Count: " << squares.getEquidistantCount() << "\n";
    if (eqValCount > 1)
        for (int eqs = 0; eqs < data.getEquidistantCount()-1; ++eqs) {
            if (data.testEquidistantValsForSquares(index, eqs)) {
                std::cout << "WOAH! CHECK OUT " << index << std::endl;
                std::cout << "Iteration: " << index << " EQ Count: " << data.getEquidistantCount() << "\n";
            }
        }
}

#endif //SQUARES_CONTAINER_H
