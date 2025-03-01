//
// Created by sluggernot on 2/27/25.
//

#include "mpz_only.h"
#include "MagicSquare_data.h"

mpz_only::mpz_only(const unsigned long long howMany) {
    std::cout << "Map max size: " << squares_set.max_size() << std::endl;
    std::cout << "Inserting   : " << howMany << std::endl;
    mpz_int squareRoot = 0;
    mpz_int squareVal = 0;
    maxValInContainer = howMany;
    for (unsigned long long i = 0; i < howMany; i++) {
        squareRoot = i;
        squareVal = squareRoot * squareRoot;
        squares_set[squareRoot] = squareVal;
    }
    std::cout << "Squares_set size: " << squares_set.size() << std::endl;
    //Just in case.
    startingVal = 1;
    boundingVal = 1;
}

void mpz_only::setStartingValueAndBounding(const mpz_int& starting, const mpz_int& bounding) {
    boundingVal = bounding;
    startingVal = starting;

    if (boundingVal < 0) {boundingVal = 1;}
    if (startingVal < 0) {startingVal = 1;}
    if (startingVal < boundingVal) {startingVal = boundingVal;}
    std::cout << "Starting value: " << startingVal << " Bounding: " << boundingVal << std::endl;
    if (startingVal % boundingVal != 0) {
        startingVal = startingVal - (startingVal % boundingVal);
        if (startingVal % boundingVal != 0) { std::cout << "I forgot how modulus worked?" << std::endl; }

        std::cout << "Starting value: " << startingVal << " Bounding: " << boundingVal << std::endl;
    }
}

void mpz_only::findAllEquidistantValues(const mpz_int& index) {
    equidistant_vals.clear();
    if (!squares_set.contains(index)) {quit = true; std::cout << "Index not found: " << index << std::endl; return;}
    //Probably make the set a map.
    const mpz_int idxVal = squares_set.at(index);
    mpz_int iterR = index+1;
    mpz_int iterL = index-1;
    mpz_int subtraction = squares_set.at(iterR)-idxVal;
    if (!squares_set.contains(iterR)) {
        //Add vals to squares_set until we are above iterR by 1? Leaving this in here until I need it so I can see what values
        quit = true;
        std::cout << "WRITE CODE FOR EXPANDING MAP SIZE!" << iterR << std::endl;
        return;
    }
    while (subtraction < idxVal) {
        while (squares_set.contains(iterL) && idxVal - squares_set.at(iterL) < subtraction) {
            --iterL;
        }
        if (!squares_set.contains(iterL)) {break;}
        if (idxVal - squares_set.at(iterL) == subtraction) {
            equidistant_vals.emplace_back(iterL, iterR);
        }
        ++iterR;
        ++iterL;
        if (!squares_set.contains(iterR)) { quit = true; std::cout << "WRITE CODE FOR EXPANDING MAP SIZE!" << iterR << std::endl; return;}
        subtraction = squares_set.at(iterR)-idxVal;
    }
}

void mpz_only::start() {
    while (!quit) {
        GivenAnIndexTestValue(startingVal);
        if (testEquidistantValsForSquares()) {break;};
        startingVal+=boundingVal;

        ++counter;
        if (counter % 1000 == 0) { std::cout << startingVal << std::endl; }
    }
}

void mpz_only::GivenAnIndexTestValue(const mpz_int &index) {
    findAllEquidistantValues(index);
    if (equidistant_vals.size() >= mostEquidistants) {
        std::cout << "Met or exceeded highest equidistant count: " << index << "," << index*index << "," << equidistant_vals.size() << std::endl;
        mostEquidistants = equidistant_vals.size();
    }
    //fileOutput << index << "," << index*index << "," << equidistant_vals.size() << ",";
}

bool isASquare(const mpz_int& testMe) {
    if (testMe < 0) return false;
#if VERBOSE
    std::cout << "Test Me: " << testMe << "\n";
#endif
    const mpz_int root = sqrt(testMe);
    return testMe == root*root;
}

bool mpz_only::testEquidistantValsForSquares() const {
    if (equidistant_vals.size() < 2) return false;

    for (unsigned int i = 0; i < equidistant_vals.size()-1; i++)
    for (unsigned int j = i+1; j < equidistant_vals.size(); j++) {
        const mpz_int& x = squares_set.at(startingVal);

        const mpz_int a = x - squares_set.at(equidistant_vals.at(i).first);
        const mpz_int b = x - squares_set.at(equidistant_vals.at(j).first);

        //BottomCenter
        const mpz_int xMinusAMinusB = x - a - b;
        if (xMinusAMinusB > 0 && squares_set.contains(xMinusAMinusB))
        {
            //TopCenter
            const mpz_int xPlusAPlusB = x + a + b;
            //LeftCenter
            const mpz_int xPlusAMinusB = x + a - b;
            //RightCenter
            const mpz_int xMinusAPlusB = x - a + b;

            int squaresTotal = 1;
            if(squares_set.contains(xPlusAPlusB) )++squaresTotal;
            if(squares_set.contains(xPlusAMinusB))++squaresTotal;
            if(squares_set.contains(xMinusAPlusB))++squaresTotal;

            if (squaresTotal > 1)
            {
                if (squaresTotal > 3)
                    std::cout << "THIS SHOULD BE A MAGIC SQUARE OF SQUARES!\n";
                else {
                    std::cout <<"Val: " << startingVal <<  " squared.  Squares count: " << squaresTotal << " plus 5 given squares. \n";
                    return false;
                }
                MagicSquare_data checkMe;
                checkMe.set9(
                    x-a, xPlusAPlusB, x-b,
                    xPlusAMinusB, x, xMinusAPlusB,
                    x+b, xMinusAMinusB, x+a);
                checkMe.printMagicSquare_withSums(true);
                checkMe.printMagicSquareDetails();
                if (checkMe.isMagicSquare()) return true;

                return true;
                //wtf its time to make a magic square here and do a real test. wtf man
                // std::cout << "Supposed val: " << xPlusAPlusB << " - " <<  squares_set.at(xPlusAPlusB) << std::endl;
                // std::cout << "Supposed val: " << xPlusAMinusB << " - " <<  squares_set.at(xPlusAMinusB) << std::endl;
                // std::cout << "Supposed val: " << xMinusAPlusB << " - " <<  squares_set.at(xMinusAPlusB) << std::endl;
                // std::cout << "Supposed val: " << xMinusAMinusB << " - " <<  squares_set.at(xMinusAMinusB) << std::endl;
                //
                // std::cout << "Squares_set size: " << squares_set.size() << std::endl;
                return true;

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
