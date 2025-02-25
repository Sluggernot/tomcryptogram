
#include <iostream>

#include "MagicSquare_data.h"
#include "squares_container.h"

//Contains the numbers for the near miss: https://www.youtube.com/watch?v=2Twa-z_WPE4&t=136s
// void someGMPexamples();

void Attempt1();

int main() {

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

    constexpr int starterNum = 17;
    //Create a massive list of squares
    squares_container squares(429496729);
    unsigned int counter = 0;
    //Iterate the squares, bounding by our pythagorean number, finding all equidistant values
    for (unsigned int i = starterNum; i < 429496729; i+=starterNum)
    {
        squares.findAllEquidistantValues(i);
        // std::cout << "Iteration: " << i << " EQ Count: " << squares.getEquidistantCount() << "\n";
        if (squares.getEquidistantCount() > 1)
        for (int eqs = 0; eqs < squares.getEquidistantCount()-1; ++eqs) {
            if (squares.testEquidistantValsForSquares(i, eqs)) {
                std::cout << "WOAH! CHECK OUT " << i << std::endl;
                std::cout << "Iteration: " << i << " EQ Count: " << squares.getEquidistantCount() << "\n";
            }
        }
        counter++;
        if (counter % 10000 == 0) {
            std::cout << "Count: " << counter << " Num: " << i << "\n";
        }
    }


    //OH SHIT! Try them as a cross as well.

    //Track the ones with more than 5 matching sums? Why did I say 5?


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
