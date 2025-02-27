//
// Created by sluggernot on 2/24/25.
//

#ifndef SQUARES_CONTAINER_H
#define SQUARES_CONTAINER_H

#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;

inline std::mutex gLocker;

class squares_container {
private:

    std::vector<mpz_int> squares_vec;
    std::vector<mpz_int>* squares_ptr = nullptr;
    std::vector<std::pair<unsigned long long, unsigned long long>> equidistant_vals;

    std::stringstream fileOutput;

public:
    explicit squares_container(unsigned long long howMany = 0);

    std::vector<mpz_int>* getPointerToSquares() { return &squares_vec; };
    void setPointerToSquares(squares_container& inContainer){squares_ptr = inContainer.getPointerToSquares();};

    //Returns an index of a matching square distance. Or -1 if none found.
    unsigned int findSquareMatchingDistance(const unsigned int index, const mpz_int& distance) const;

    //Given a number, iterate the container, looking for double equidistant values
    //Returns count of how many pairs of equidistant values there are
    unsigned long findAllEquidistantValues(const unsigned int startingIndex);

    bool testEquidistantValsForSquares(const unsigned int center, const unsigned int distance_idx) const;

    static bool isASquare(const mpz_int& testMe);

    void GivenAnIndexTestValue(unsigned int index, squares_container& data);

    static std::string GetTimestamp();

    void makeThreadsAndCalculate();
    std::pair<unsigned long long, unsigned long long>& getEquidistant_valAtIndex(const unsigned int idx){ return equidistant_vals[idx]; };
};


inline std::string squares_container::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "%Y%m%d_%H%M%S");
    return ss.str();
}


#endif //SQUARES_CONTAINER_H
