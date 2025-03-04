
#include <iostream>

#include "MagicSquare_data.h"
#include "squares_container.h"
#include "mpz_only.h"

//Contains the numbers for the near miss: https://www.youtube.com/watch?v=2Twa-z_WPE4&t=136s
// void someGMPexamples();

void Attempt1();

int main() {

    // unsigned int val = 17 * 29 * 37;
    // std::cout << val << std::endl;
    // for (unsigned int i = 2; i < val; i++) {
    //     if (val % i == 0) { std::cout << i << std::endl;}
    // }
    // return 0;

    //Create a massive list of squares
    constexpr unsigned int howMany = 429496729/10;

    //New attempt is close to a copy of Attempt 2 but we're using all mpzs, we're going to calculate if we need more values in the set and add them, etc.
    //So, we're using set indexing for calcs. And will have a bounding value, where we += bounding_val
    //We will consider finding a 6th square value a massive win!
    mpz_only temp(howMany); //Roughly 59GB of data. Plenty of room to find a magic square of squares.
    std::cout << "Data initialized" << std::endl;
    //  1 - 332032 - Suspicious. havent seen an indicator that any number met or exceeded 67 equidistants from 226525 to 292030?
        //BUT we were already seeing gaps from 160225 to 204425 and then to 226525
    // 17 - 461040
    // 29 - 4467044 4,782,071 - wtf why did I start over with 29?
    // 37 - 2407997
    temp.setStartingValueAndBounding(461040-1, 17);
    //Wanted to test 5107973 from https://oeis.org/A097282 - Got the expected 40 vals pretty quickly but the drop-off in time to calculate 1000 is unknown.
    temp.makeThreadsAndCalculate();
    //temp.start();

    //Attempt 2 - Simply requires an unsigned int, defining how many square values to calc.
    //Never found a sixth square number with the formula.
    // squares_container squares(howMany);
    // squares.makeThreadsAndCalculate();

    // Attempt1();

    return 0;
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
