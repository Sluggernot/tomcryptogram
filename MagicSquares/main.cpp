
#include <charconv>
#include <fstream>
#include <iostream>

#include "MagicSquare_data.h"
#include "squares_container.h"
#include "mpz_only.h"
#include "gmpxx.h"

//Contains the numbers for the near miss: https://www.youtube.com/watch?v=2Twa-z_WPE4&t=136s
// void someGMPexamples();

void Attempt1();
void GetInfoOnMagicSquares(int center_num, const int testA = 0, const int testB = 0);

void ProcessAllNumbersInAFile(std::string_view fileName);

void PrintHelp();

int main(int argc, char **argv) {
    int numThreads = 1;
    int startingNum = 1000;
    int boundingNum = 1; //Only test every 17th num. Program makes sure we are on a multiple of this number
    bool adjustStartByModBounding = true;
    int maxValue = -1;

    std::string_view helpPrefix = "--file=";
    std::string_view fileParamPrefix = "--file=";
    std::string_view threadCountPrefix = "--j=";
    std::string_view startNumPrefix = "--start=";
    std::string_view boundingNumPrefix = "--bound=";
    std::string_view detailsPrefix = "--details=";
    std::string_view adjustPrefix = "--dontAdjust";
    std::string_view maxValuePrefix = "--maxValue=";

    //Good if I release this code. I still want to just be able to run ./MagicSquares and go. Should probably just use my program as is... idk.
    // if (argc < 2) {
    //     PrintHelp();
    //     return 0;
    // }

    for (int i = 1; i < argc; i++) {
        std::string_view arg = argv[i];

        if (arg.starts_with(helpPrefix)) {
            PrintHelp();
            return 0;
        }
        if (arg.starts_with(adjustPrefix)) {
            adjustStartByModBounding = false;
        }
        if (arg.starts_with(fileParamPrefix)) {
            const std::string_view fileName = arg.substr(fileParamPrefix.size());
            ProcessAllNumbersInAFile(fileName);
            return 0;
        }
        if (arg.starts_with(threadCountPrefix)) {
            const std::string_view sv = arg.substr(threadCountPrefix.size());
            auto result = std::from_chars(sv.data(), sv.data() + sv.size(), numThreads);
            std::cout << "Thread count: " <<  sv << " should be same as: " << numThreads << std::endl;
        }
        if (arg.starts_with(startNumPrefix)) {
            const std::string_view sv = arg.substr(startNumPrefix.size());
            auto result = std::from_chars(sv.data(), sv.data() + sv.size(), startingNum);
            std::cout << "Starting number: " <<  sv << " should be same as: " << startingNum << std::endl;
        }
        if (arg.starts_with(boundingNumPrefix)) {
            const std::string_view sv = arg.substr(boundingNumPrefix.size());
            auto result = std::from_chars(sv.data(), sv.data() + sv.size(), boundingNum);
            std::cout << "Bounding by: " <<  sv << " should be same as: " << boundingNum << std::endl;
        }
        if (arg.starts_with(maxValuePrefix)) {
            const std::string_view sv = arg.substr(maxValuePrefix.size());
            auto result = std::from_chars(sv.data(), sv.data() + sv.size(), maxValue);
            std::cout << "Bounding by: " <<  sv << " should be same as: " << maxValue << std::endl;
        }
        if (arg.starts_with(detailsPrefix)) {
            const std::string_view sv = arg.substr(detailsPrefix.size());
            mpz_int indexToCheck(sv);
            //auto result = std::from_chars(sv.data(), sv.data() + sv.size(), indexToCheck);
            mpz_only temp;
            temp.PrintAllDataGivenAValue(indexToCheck, true);
            return 0;
        }

        //Print equidistant pairs for a given number? List?
        //Ask Chris for more info on params
    }

    //https://www.calculatorsoup.com/calculators/math/prime-number-calculator.php
    // const unsigned int val = 1216265;
    // std::cout << val << std::endl;
    // for (unsigned int i = 2; i < val/2; i++) {
    //     if (val % i == 0) { std::cout << i << std::endl;}
    // }return 0;

    // GetInfoOnMagicSquares(1000, 223, 444); return 0;

    // Try to find a way to input a number and find two more square numbers that add to a 4th square?
    // Can I find the extremes when using the x-a-b formula, given a number? Enter 1000 and if a = 1, how far can  b go? How far can both go, staying 1 apart? 1000/2+-1?

    //New attempt is close to a copy of Attempt 2 but we're using all mpzs, we're going to calculate if we need more values in the set and add them, etc.
    //So, we're using set indexing for calcs. And will have a bounding value, where we += bounding_val
    //We will consider finding a 6th square value a massive win!
    mpz_only temp; //Roughly 59GB of data if I don't divide howMany. Plenty of room to find a magic square of squares.
    std::cout << "Data initialized" << std::endl;
    // temp.isOneDouble(1000); return 0;
    // for (unsigned int i = 1; i < 20; i++) {
    //     mpz_int checkMe = 8258753;
    //     std::cout << checkMe*i << " - " << temp.PrintAllDataGivenAValue(checkMe*i, false) << std::endl;
    // }
    // return 0;//1136690
    //  1 - 4064125 - Suspicious. havent seen an indicator that any number met or exceeded 67 equidistants from 226525 to 292030?
    //BUT we were already seeing gaps from 160225 to 204425 and then to 226525
    // 17 - 6985810
    // 29 - 4064125
    // 37 - 4064125
    // 85 - 9592250 17*5
    // 697 - 57185365  least common denominator between 17 and 41 which keeps coming up as a factor of the more interesting near misses.
    temp.setStartingValueAndBounding(startingNum, boundingNum, maxValue, adjustStartByModBounding);
    //Wanted to test 5107973 from https://oeis.org/A097282
    if (numThreads > 1) {
        temp.makeThreadsAndCalculate(numThreads);
    }
    else {
        temp.start();
    }

    //Oh, right. So what holds true between LoShu and HiShu? 1 and 9 with the B diag, right?

    //Attempt 2 - Simply requires an unsigned int, defining how many square values to calc.
    //Never found a sixth square number with the formula.
    // squares_container squares(howMany);
    // squares.makeThreadsAndCalculate();

    // Attempt1();

    return 0;
}

void ProcessAllNumbersInAFile(std::string_view fileName) {
    mpz_only temp;
    std::cout << "Data initialized" << std::endl;
    std::fstream file;
    file.open(fileName.data(), std::ios::in);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return;
    }

    if (file.is_open()) {
        std::cout << "File opened" << std::endl;
        std::string line;
        while (!file.eof()) {
            std::getline(file, line);
            try {
                mpz_int num(line); // Convert string directly to mpz_int
                std::cout << num << " - ";
                num = temp.PrintAllDataGivenAValue(num, false);
                std::cout << num << "\n";
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number: " << line << '\n';
            }
        }
        file.close();
    }
    else {
        std::cerr << "Unable to open file " << fileName << '\n';
    }
}


void Attempt1()
{
    /* To get a magic square:
     * Make a list of squares up to X.
     *
     * How do we want to get started filling the square?
     * Then how do we pick which number to change and what to?
     *
     * What are the chances theres just like a 49 in the middle square of some stupidly large numbers?
     * Pretty much zero, right? If the other 8 numbers are gigantic, they cant be 49 apart, right?
     */

    //MagicSquare_data data;
    MagicSquare_data data(127*127, 46*46, 58*58, 2*2, 113*113, 94*94, 74*74, 82*82, 97*97);
    mpz_int howManySquaresDidWeProcess = 0;
    // data.printMagicSquare_withSums(true);
    // data.printMagicSquareDetails();


    //Get most common value from rows, cols, diags
    mpz_int commonSum = data.calculateMostCommonSum(false);
    //Are the other sums mostly higher or lower than the most common?
    bool bIsHigher = data.areUncommonSumsHigher(); //No need to call this here. It's called from isThereACommonElementFromBadSums()
    //Or, we find a row and column whose sums are both higher or lower than the common.
    while (data.isThereACommonElementFromBadSums() != -1) //Checks if most other sums are higher or lower and sets current_focus if there is
    {
        ++howManySquaresDidWeProcess;
        //Get the common element of the row and column and increment until the row or column meet or exceed the common
        if (bIsHigher) //If higher, decrement
            data.decrementAValueAtIndex(data.getCurrentFocus());
        else
            data.incrementAValueAtIndex(data.getCurrentFocus());
        //Reassess
        if (data.isMagicSquare())
            return;//Go apeshit, at this time

        data.calculateMostCommonSum(false);
        bIsHigher = data.areUncommonSumsHigher();
    }

    data.printMagicSquare_withSums(true);
    data.printMagicSquareDetails();

    std::cout << "\n\nProcessed magic square count: " << howManySquaresDidWeProcess << "\n\n" << std::endl;
}

bool isSquare(const int* square) {

    int threex = square[val_loc::cen_cen]*3;
    return (
        square[val_loc::bot_lft] + square[val_loc::bot_cen] + square[val_loc::bot_rgt] == threex &&
        square[val_loc::cen_lft] + square[val_loc::cen_cen] + square[val_loc::cen_rgt] == threex &&
        square[val_loc::top_lft] + square[val_loc::top_cen] + square[val_loc::top_rgt] == threex &&

        square[val_loc::top_lft] + square[val_loc::cen_lft] + square[val_loc::bot_lft] == threex &&
        square[val_loc::top_cen] + square[val_loc::cen_cen] + square[val_loc::bot_cen] == threex &&
        square[val_loc::top_rgt] + square[val_loc::cen_rgt] + square[val_loc::bot_rgt] == threex
        );
}

bool isLoShu(const int* square) {

    if (!isSquare(square)){std::cout << "WOAH! NOT EVEN SQUARE!" << std::endl; return false;}

    if (square[val_loc::bot_lft] > square[val_loc::cen_lft] && square[val_loc::cen_lft] > square[val_loc::top_lft]) {return false;}

    if (square[val_loc::bot_lft] < square[val_loc::cen_lft] && square[val_loc::cen_lft] > square[val_loc::top_lft]) {return true;}

    std::cout << "WHAT THE F? My checks dont work!\n";
    return false;
}

void GetInfoOnMagicSquares(const int center_num, const int testA, const int testB) {

    std::cout << "\nHiShus are generated when A > .5*B. wtf? " << std::endl;
    //Start from the ends and work in
    int LoShuCount = 0;
    int HiShuCount = 0;

    int a;
    int b;
    if (testA == 0) a = 1;
    else a = testA;
    if (testB == 0) b = 3;
    else b = testB;

    int total = 0;
    int vals[val_loc::LAST_VAL];
    while (a+b < center_num) {
        vals[val_loc::bot_lft] = center_num+a;
        vals[val_loc::bot_rgt] = center_num+b;
        vals[val_loc::bot_cen] = center_num-a-b;

        vals[val_loc::top_lft] = center_num-b;
        vals[val_loc::top_rgt] = center_num-a;
        vals[val_loc::top_cen] = center_num+a+b;

        vals[val_loc::cen_lft] = center_num-a+b;
        vals[val_loc::cen_rgt] = center_num+a-b;
        vals[val_loc::cen_cen] = center_num;

        if (isLoShu(vals)){ LoShuCount++; std::cout << "a: " << a << " b: " << b << std::endl; }
        else { HiShuCount++; }
        ++a;
        ++b;
        if (b == 2*a){++a; ++b;}
        ++total;
        if (testA != 0 && testB != 0) {
            std::cout << "First third: LoShus: " << LoShuCount << " HiShus: " << HiShuCount << " out of: " << total << std::endl;
            return;
        }
    }
    std::cout << "Values are now at a: " << a << " b: " << b << std::endl;
    std::cout << "First third: LoShus: " << LoShuCount << " HiShus: " << HiShuCount << " out of: " << total << std::endl;

    bool wasLoShu = false;
    bool tempIsLoShu = false;
    --a;
    while (a > 0) {
        vals[val_loc::bot_lft] = center_num+a;
        vals[val_loc::bot_rgt] = center_num+b;
        vals[val_loc::bot_cen] = center_num-a-b;

        vals[val_loc::top_lft] = center_num-b;
        vals[val_loc::top_rgt] = center_num-a;
        vals[val_loc::top_cen] = center_num+a+b;

        vals[val_loc::cen_lft] = center_num-a+b;
        vals[val_loc::cen_rgt] = center_num+a-b;
        vals[val_loc::cen_cen] = center_num;
        wasLoShu = tempIsLoShu;
        tempIsLoShu = isLoShu(vals);
        if (wasLoShu!=tempIsLoShu) {std::cout << "Changing over from HiShu to LoShu: was a: " << a+1 << " b: " << b << " now a: " << a << " b: " << b << std::endl;}
        if (tempIsLoShu){ LoShuCount++; }
        else { HiShuCount++; }
        --a;
        ++total;
    }
    ++a;
    std::cout << "A back down to 1: LoShus: " << LoShuCount << " HiShus: " << HiShuCount << " out of: " << total << std::endl;

    while (a+b < center_num) ++b;
    --b;
    std::cout << "Put b up at b: " << b << " with A at: " << a << std::endl;
    while (b > 3) {
        vals[val_loc::bot_lft] = center_num+a;
        vals[val_loc::bot_rgt] = center_num+b;
        vals[val_loc::bot_cen] = center_num-a-b;

        vals[val_loc::top_lft] = center_num-b;
        vals[val_loc::top_rgt] = center_num-a;
        vals[val_loc::top_cen] = center_num+a+b;

        vals[val_loc::cen_lft] = center_num-a+b;
        vals[val_loc::cen_rgt] = center_num+a-b;
        vals[val_loc::cen_cen] = center_num;
        wasLoShu = tempIsLoShu;
        tempIsLoShu = isLoShu(vals);
        if (wasLoShu!=tempIsLoShu) {std::cout << "Changing over from HiShu to LoShu: was a: " << a+1 << " b: " << b << " now a: " << a << " b: " << b << std::endl;}

        if (tempIsLoShu){ LoShuCount++; }
        else { HiShuCount++; }
        --b;
        ++total;
    }

    std::cout << "Take b back down to 4: LoShus: " << LoShuCount << " HiShus: " << HiShuCount << " out of: " << total << std::endl;
}

void PrintHelp() {

}


// void someGMPexamples() {
//
//     //Quick initialization but mpz_ints cant mult?
//     mpz_int current_square[9];
//     current_square[0] = 5976563248318113025ULL;
//     current_square[1] = 15260232026299743025ULL;
//     current_square[2] = 2577330938902409089ULL;
//     current_square[3] = 4537991882387127025ULL;
//     current_square[4] = 7937944177203705025ULL;
//     current_square[5] = 11337896472020283025ULL;
//     current_square[6] = 13298557415505000961ULL;
//     current_square[7] = 615656328107667025ULL;
//     current_square[8] = 9899325106089297025ULL;
//
//     // printMagicSquare_withSums(current_square, true);
//     // printMagicSquareDetails(current_square);
//     std::cout << "\n\n";
//
//     mpz_int sqrt = 1;
//     mpz_ptr temp;
//
//     mpz_t sq_me, sq_out, test;
//     mpz_init(sq_me);
//     mpz_init(sq_out);
//     mpz_init(test);
//     mpz_set_ui(sq_me, 2577330938902409089);
//
//     mpz_sqrt(sq_out, sq_me);
//     std::cout << sq_me << " " << sq_out << std::endl;
//     //This didn't appear to do anything?
//     mpz_mul(sq_me, sq_out, sq_out);
//     std::cout << sq_me << " " << sq_out << std::endl;
//
//     std::cout <<"A: " << current_square[0] << std::endl;
//     current_square[0] = sq_out;
//     std::cout <<"A: " << current_square[0] << std::endl;
//     current_square[0] = current_square[0]*current_square[0];
//     std::cout <<"A: " << current_square[0] << std::endl;
//
//     // mpz_int v = 1;
//     // std::cout << "Sizeof v:" << sizeof(v) << " Value of v: " << v << " END \n\n";
//     // // Do some arithmetic:
//     // for(unsigned i = 1; i <= 1000; ++i)
//     //     v *= i;
//     // std::cout << "Sizeof v:" << sizeof(v) << " Value of v: " << v << " END \n\n";
//
//     // Access the underlying representation:
//     // mpz_t z;
//     // mpz_init(z);
//     // mpz_set(z, v.backend().data());
//     // mpz_clear(z);
// }

// void boost_precision_cpp_int() {
//     using boost::multiprecision::cpp_int;
//     //
//     // Print all the factorials that will fit inside a 128-bit integer.
//     //
//     // Begin by building a big table of factorials, once we know just how
//     // large the largest is, we'll be able to "pretty format" the results.
//     //
//     // Calculate the largest number that will fit inside 128 bits, we could
//     // also have used numeric_limits<int128_t>::max() for this value:
//     cpp_int limit = (cpp_int(1) << 128) - 1;
//     //
//     // Our table of values:
//     std::vector<cpp_int> results;
//     //
//     // Initial values:
//     unsigned i = 1;
//     cpp_int factorial = 1;
//     //
//     // Cycle through the factorials till we reach the limit:
//     while(factorial < limit)
//     {
//         results.push_back(factorial);
//         ++i;
//         factorial *= i;
//     }
//     //
//     // Lets see how many digits the largest factorial was:
//     unsigned digits = results.back().str().size();
//     //
//     // Now print them out, using right justification, while we're at it
//     // we'll indicate the limit of each integer type, so begin by defining
//     // the limits for 16, 32, 64 etc bit integers:
//     cpp_int limits[] = {
//         (cpp_int(1) << 16) - 1,
//         (cpp_int(1) << 32) - 1,
//         (cpp_int(1) << 64) - 1,
//         (cpp_int(1) << 128) - 1,
//      };
//     std::string bit_counts[] = { "16", "32", "64", "128" };
//     unsigned current_limit = 0;
//     for(const auto & result : results)
//     {
//         if(limits[current_limit] < result)
//         {
//             std::string message = "Limit of " + bit_counts[current_limit] + " bit integers";
//             std::cout << std::setfill('.') << std::setw(digits+1) << std::right << message << std::setfill(' ') << std::endl;
//             ++current_limit;
//         }
//         std::cout << std::setw(digits + 1) << std::right << result << std::endl;
//     }
//
//     cpp_int big_int = (cpp_int(0));
//     big_int = ~big_int;
//
//     std::cout << limit << std::endl;
//     std::cout << std::setw(digits + 1) << std::right << limit << std::endl;
//
// }


//Init some square numbers to reference
//                                 1605406783
// for (unsigned long long i = 1; i < 1000000000ULL; i++) {
//     number = i * i;
//     numbers.push_back(number);
// }

//init current_square
// for (int i = 0; i < 9; i++) {
//     current_square[i] = numbers[i];
// }
// unsigned long long counter = 1151798940000ULL;
// unsigned long long ull_max = ~(0ULL);
// while (!isMagicSquare(current_square) && counter < ull_max-1) {
//     if (counter % 10000 == 0) std::cout << counter << "\n";
//     swapLowest(current_square, counter++);
// }
