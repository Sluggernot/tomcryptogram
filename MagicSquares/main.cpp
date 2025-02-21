
#include <iostream>
#include <set>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/tommath.hpp>
#include <boost/multiprecision/gmp.hpp>

#include <vector>

using namespace boost::multiprecision;

void printMagicSquare(const std::pair<mpz_int, mpz_int>* square_arr);
void printMagicSquare_withSums(const std::pair<mpz_int, mpz_int>* square_arr, const bool diags = true);

bool isMagicSquare(const std::pair<mpz_int, mpz_int>* square_arr);

//Probably not useful to arbitrarily pick the lowest value.
void swapLowest(std::pair<mpz_int, mpz_int>* square_arr, const int counter);
int getLowestValuesIndex(const std::pair<mpz_int, mpz_int>* square_arr);

void printMagicSquareDetails(const std::pair<mpz_int, mpz_int>* square_arr);

//Contains the numbers for the near miss: https://www.youtube.com/watch?v=2Twa-z_WPE4&t=136s
void someGMPexamples();

void incrementAValue(std::pair<mpz_int, mpz_int>& val);
void decrementAValue(std::pair<mpz_int, mpz_int>& val);
void initializeSquares(std::pair<mpz_int, mpz_int>* square_arr) {
    for (int i = 0; i < 9; ++i) {
        square_arr[i].first = sqrt(square_arr[i].second);
    }
}

int main() {

/* To get a magic square:
 * Make a list of squares up to X.
 *
 * How do we want to get started filling the square?
 * Then how do we pick which number to change and what to?
 *
 * What are the chances theres just like a 49 in the middle square of some stupidly large numbers?
 * Pretty much zero, right? If the other 8 numbers are gigantic, they cant be 49 apart, right?
 */
//    someGMPexamples();
    // std::vector<mpz_int> numbers;

    std::pair<mpz_int, mpz_int> current_square[9];
    mpz_int number;
    current_square[0] = std::make_pair(1, 5976563248318113025ULL);
    current_square[1] = std::make_pair(1, 15260232026299743025ULL);
    current_square[2] = std::make_pair(1, 2577330938902409089ULL);
    current_square[3] = std::make_pair(1, 4537991882387127025ULL);
    current_square[4] = std::make_pair(1, 7937944177203705025ULL);
    current_square[5] = std::make_pair(1, 11337896472020283025ULL);
    current_square[6] = std::make_pair(1, 13298557415505000961ULL);
    current_square[7] = std::make_pair(1, 615656328107667025ULL);
    current_square[8] = std::make_pair(1, 9899325106089297025ULL);
    initializeSquares(current_square);

    printMagicSquare_withSums(current_square, true);
    printMagicSquareDetails(current_square);
    std::cout << "\n\n";

    //getLowest value in the array and increment it
    incrementAValue(current_square[getLowestValuesIndex(current_square)]);

    printMagicSquare_withSums(current_square, true);
    // printMagicSquareDetails(current_square);
    std::cout << "\n\n";


    // std::cout << "\n\nProbably didnt find the magic...\n\n" << std::endl;
    // printMagicSquare(current_square);

    return 0;
}

void someGMPexamples() {

    //Quick initialization but mpz_ints cant mult?
    mpz_int current_square[9];
    current_square[0] = 5976563248318113025ULL;
    current_square[1] = 15260232026299743025ULL;
    current_square[2] = 2577330938902409089ULL;
    current_square[3] = 4537991882387127025ULL;
    current_square[4] = 7937944177203705025ULL;
    current_square[5] = 11337896472020283025ULL;
    current_square[6] = 13298557415505000961ULL;
    current_square[7] = 615656328107667025ULL;
    current_square[8] = 9899325106089297025ULL;

    // printMagicSquare_withSums(current_square, true);
    // printMagicSquareDetails(current_square);
    std::cout << "\n\n";

    mpz_int sqrt = 1;
    mpz_ptr temp;

    mpz_t sq_me, sq_out, test;
    mpz_init(sq_me);
    mpz_init(sq_out);
    mpz_init(test);
    mpz_set_ui(sq_me, 2577330938902409089);

    mpz_sqrt(sq_out, sq_me);
    std::cout << sq_me << " " << sq_out << std::endl;
    //This didn't appear to do anything?
    mpz_mul(sq_me, sq_out, sq_out);
    std::cout << sq_me << " " << sq_out << std::endl;

    std::cout <<"A: " << current_square[0] << std::endl;
    current_square[0] = sq_out;
    std::cout <<"A: " << current_square[0] << std::endl;
    current_square[0] = current_square[0]*current_square[0];
    std::cout <<"A: " << current_square[0] << std::endl;

    // mpz_int v = 1;
    // std::cout << "Sizeof v:" << sizeof(v) << " Value of v: " << v << " END \n\n";
    // // Do some arithmetic:
    // for(unsigned i = 1; i <= 1000; ++i)
    //     v *= i;
    // std::cout << "Sizeof v:" << sizeof(v) << " Value of v: " << v << " END \n\n";

    // Access the underlying representation:
    // mpz_t z;
    // mpz_init(z);
    // mpz_set(z, v.backend().data());
    // mpz_clear(z);
}

void boost_precision_cpp_int() {
    using boost::multiprecision::cpp_int;
    //
    // Print all the factorials that will fit inside a 128-bit integer.
    //
    // Begin by building a big table of factorials, once we know just how
    // large the largest is, we'll be able to "pretty format" the results.
    //
    // Calculate the largest number that will fit inside 128 bits, we could
    // also have used numeric_limits<int128_t>::max() for this value:
    cpp_int limit = (cpp_int(1) << 128) - 1;
    //
    // Our table of values:
    std::vector<cpp_int> results;
    //
    // Initial values:
    unsigned i = 1;
    cpp_int factorial = 1;
    //
    // Cycle through the factorials till we reach the limit:
    while(factorial < limit)
    {
        results.push_back(factorial);
        ++i;
        factorial *= i;
    }
    //
    // Lets see how many digits the largest factorial was:
    unsigned digits = results.back().str().size();
    //
    // Now print them out, using right justification, while we're at it
    // we'll indicate the limit of each integer type, so begin by defining
    // the limits for 16, 32, 64 etc bit integers:
    cpp_int limits[] = {
        (cpp_int(1) << 16) - 1,
        (cpp_int(1) << 32) - 1,
        (cpp_int(1) << 64) - 1,
        (cpp_int(1) << 128) - 1,
     };
    std::string bit_counts[] = { "16", "32", "64", "128" };
    unsigned current_limit = 0;
    for(const auto & result : results)
    {
        if(limits[current_limit] < result)
        {
            std::string message = "Limit of " + bit_counts[current_limit] + " bit integers";
            std::cout << std::setfill('.') << std::setw(digits+1) << std::right << message << std::setfill(' ') << std::endl;
            ++current_limit;
        }
        std::cout << std::setw(digits + 1) << std::right << result << std::endl;
    }

    cpp_int big_int = (cpp_int(0));
    big_int = ~big_int;

    std::cout << limit << std::endl;
    std::cout << std::setw(digits + 1) << std::right << limit << std::endl;

}

void printMagicSquare(const std::pair<mpz_int, mpz_int>* square_arr) {
    for (int i = 0; i < 9; i++) {
        std::cout << square_arr[i].second << " ";
        if (i != 0 && ((i+1) % 3) == 0) { std::cout << std::endl;}
    }
}

void printMagicSquare_withSums(const std::pair<mpz_int, mpz_int>* square_arr, const bool diags) {
    mpz_int sum = 0;
    for (int i = 0; i < 9; i++) {
        std::cout << square_arr[i].second << "\t ";
        sum += square_arr[i].second;
        if (i != 0 && ((i+1) % 3) == 0) { std::cout <<"\t - " << sum << std::endl; sum = 0;}
    }
    std::cout << "==================================================" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << square_arr[i].second + square_arr[i+3].second + square_arr[i+6].second << "\t ";
    }
    if (diags) {
        std::cout << "\nDiagonals:\nUpper Left to Low Right: ";
        std::cout << square_arr[0].second + square_arr[4].second + square_arr[8].second << std::endl;
        std::cout << "Bottom Left to Up Right: ";
        std::cout << square_arr[2].second + square_arr[4].second + square_arr[6].second << std::endl;
    }
}

char printLocation(int index) {
    if (index == 0 || index == 3 || index == 6) {
        std::cout << "Left ";
    }
    else if (index == 1 || index == 4 || index == 7) {
        std::cout << "Center ";
    }
    else if (index == 2 || index == 5 || index == 8) {
        std::cout << "Right ";
    }

    if (index == 0 || index == 1 || index == 2) {
        std::cout << "Top\n";
    }
    else if (index == 3 || index == 4 || index == 5) {
        std::cout << "Center\n";
    }
    else if (index == 6 || index == 7 || index == 8) {
        std::cout << "Bottom\n";
    }
    return ' ';
}

void printMagicSquareDetails(const std::pair<mpz_int, mpz_int>* square_arr) {
    int number = 0;
    for (int i = 1; i < 9; i++) {
        if (square_arr[i].second < square_arr[number].second) {
            number = i;
        }
    }
    std::cout << "Smallest: " << printLocation(number) << " Val: " << square_arr[number].second << std::endl;
    number = 0;
    for (int i = 1; i < 9; i++) {
        if (square_arr[i].second > square_arr[number].second) {
            number = i;
        }
    }
    std::cout << "Largest:" << printLocation(number) << " Val: " << square_arr[number].second << std::endl;

    mpz_int sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += square_arr[i].second;
    }
    std::cout << "Sum: " << sum << std::endl;

    std::cout << "\n\nAscending list: " << std::endl;
    std::set<int> numbers;
    std::vector<int> the_numbers;
    int firstLowest = getLowestValuesIndex(square_arr);
    numbers.insert(firstLowest);
    the_numbers.push_back(firstLowest);
    bool found = false;
    while (the_numbers.size() < 9) {
        for (int i = 0; i < 9; i++) {
            if (!found && !numbers.contains(i) && square_arr[i] > square_arr[firstLowest]) {
                found = true;
                firstLowest = i;
            }
            else if (found && !numbers.contains(i) && square_arr[i] < square_arr[firstLowest]) {
                firstLowest = i;
            }
        }
        the_numbers.push_back(firstLowest);
        numbers.insert(firstLowest);
        found = false;
    }
    for (int i = 0; i < the_numbers.size(); i++) {
        std::cout << "Sqrt: " << square_arr[the_numbers[i]].first << " Value: " << square_arr[the_numbers[i]].second << std::endl;
    }
}

bool isMagicSquare(const std::pair<mpz_int, mpz_int>* square_arr) {

    //Are all numbers unique?
    for (int i = 0; i <= 9; i++) {
        for (int j = i+1; j <= 9; j++) {
            //if two values are the same, it's not a magic square
            if (square_arr[i].second != square_arr[j].second) { return false; }
        }
    }

    //Add and check cols, rows, diags.
    const mpz_int row1 = square_arr[0].second + square_arr[1].second + square_arr[2].second;
    const mpz_int row2 = square_arr[3].second + square_arr[4].second + square_arr[5].second;
    if (row1 != row2) { return false; }
    const mpz_int row3 = square_arr[6].second + square_arr[7].second + square_arr[8].second;
    if (row2 != row3) { return false; }

    const mpz_int col1 = square_arr[0].second + square_arr[3].second + square_arr[6].second;
    if (col1 != row1) { return false; }
    const mpz_int col2 = square_arr[1].second + square_arr[4].second + square_arr[7].second;
    if (col1 != col2) { return false; }
    const mpz_int col3 = square_arr[2].second + square_arr[5].second + square_arr[8].second;
    if (col2 != col3) { return false; }

    //Diagonals
    const mpz_int dia1 = square_arr[0].second + square_arr[4].second + square_arr[8].second;
    if (dia1 != col3) { return false; }
    const mpz_int dia2 = square_arr[2].second + square_arr[4].second + square_arr[6].second;
    if (dia1 != dia2) { return false; }

    std::cout << "Perfect square found!" << std::endl;
    printMagicSquare(square_arr);
    return true;
}

void swapLowest(std::pair<mpz_int, mpz_int>* square_arr, const int counter) {
    int lowest = getLowestValuesIndex(square_arr);
    square_arr[lowest].second = counter * counter;
}

//Get the index into the array of the lowest value. Can use first or second.
int getLowestValuesIndex(const std::pair<mpz_int, mpz_int>* square_arr) {
    int lowest = 0;
    for (int i = 1; i < 9; i++) {
        if (square_arr[i].second < square_arr[lowest].second) {
            lowest = i;
        }
    }
    return lowest;
}

void incrementAValue(std::pair<mpz_int, mpz_int>& val) {
    ++val.first;
    val.second = val.first * val.first;
}

void decrementAValue(std::pair<mpz_int, mpz_int>& val) {
    --val.first;
    val.second = val.first * val.first;
}

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
