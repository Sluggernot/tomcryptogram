//
// Created by sluggernot on 2/27/25.
//

#ifndef MPZ_ONLY_H
#define MPZ_ONLY_H

#include <map>
#include <thread>
#include <boost/multiprecision/gmp.hpp>

#include "squares_container.h"

using namespace boost::multiprecision;

inline std::mutex mpzOnlyMutex;

struct mpz_threadWorker {
    mpz_threadWorker()=default;
    ~mpz_threadWorker()=default;
    std::thread t_worker_thread;
    const std::map<mpz_int, mpz_int>* t_squares_set_ptr= nullptr;
    std::vector<std::pair<mpz_int, mpz_int>> t_equidistant_vals;
    mpz_int t_currentVal = 0;
    int t_threadIterCounter = 0;
    int t_threadNum = 0;
};

class mpz_only {

    std::map<mpz_int, mpz_int> squares_set;
    std::vector<std::pair<mpz_int, mpz_int>> equidistant_vals;
    mpz_int currentVal, boundingVal, maxValInContainer;
    bool quit = false;
    std::atomic<int> counter = 0;
    unsigned long mostEquidistants = 41;
    unsigned int threadNum = 1;

    std::stringstream fileOutput;

public:

    explicit mpz_only(const unsigned long long howMany = 0);

    void setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding);

    static void findAllEquidistantValues(const mpz_int& index, std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs, const std::map<mpz_int, mpz_int>* squares_set);
    static bool testEquidistantValsForSquares(const mpz_int& index, const std::vector<std::pair<mpz_int, mpz_int>>& equidistPairs);

    void start();

    void returnWorkerValAndReadyNext(mpz_int& index);
    void makeThreadsAndCalculate();

    //Just want to see any patterns possible with
    void PrintAllDataGivenAValue(const mpz_int& index);

    void isOneDouble(const mpz_int& startingPlace) const;

private:
    void GivenAnIndexTestValue(const mpz_int& index);
};

#endif //MPZ_ONLY_H
