//
// Created by sluggernot on 2/24/25.
//

#ifndef SQUARES_CONTAINER_H
#define SQUARES_CONTAINER_H

#include <boost/multiprecision/gmp.hpp>

//https://www.youtube.com/watch?v=uz9jOIdhzs0 - 11:54
// MagicSquare_data data(
//     2879*2879, 16492609, 2125*2125,
//     5992609, 3125*3125, 13538641,
//     3875*3875, 3038641, 3353*3353);
// MagicSquare_data data(
//     3508129, 10819825, 3024121,
//     5300017, 5784025, 6268033,
//     8543929, 748225, 8059921);
// data.printMagicSquare_withSums(true);
// data.printMagicSquareDetails();
// if (data.isMagicSquare()) std::cout << "OK, shit yourself" << std::endl;
// else std::cout << "Yeah, it's not a magic square of squares man.\n";
// return 0;

//https://www.youtube.com/watch?v=uz9jOIdhzs0 13:52 -
//If the Euler brick does exist, one of the 3 diagonals MUST be divisible by 17, 29 or 37...
//The formula for generating the anti-parkers would be like: generate a shitload of numbers, starting with a pythagorean prime squared
//Then pythagorean prime += that prime squared.
//Example: 17sq, (17+17)sq, and so on.
//Then find double equidistant values from a center value.
//Then do some adding and subtracting with those equidistant values and check if they're all square.
//
//   x-a  | x+a+b |  x-b
//  x+a-b |   x   | x-a+b
//   x+b  | x-a-b |  x+a
//Where x, a and b have some relation in distance from each other.
//It appears we must start at LEAST at 3125 squared.
//Ok, so she was hand calculating squares had 4 square numbers that had square numbers equidistant from that number.
//Example: 10sq, 34sq, 50sq, 62sq, 70sq - Are all 5 successive numbers the same difference between them?
//Or do 70 and 10 simply need to be the same to their neighbor?

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
