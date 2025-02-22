//
// Created by sluggernot on 2/21/25.
//

#include "MagicSquare_data.h"

#include <map>
#include <set>

void MagicSquare_data::printMagicSquare() const {
    for (int i = 0; i < 9; i++) {
        std::cout << square_arr[i].second << " ";
        if (i != 0 && ((i+1) % 3) == 0) { std::cout << std::endl;}
    }
}

void MagicSquare_data::printMagicSquare_withSums(const bool diags) const {
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

char MagicSquare_data::printLocation(const int index) const {
    if (index > 8)return ' ';

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

void MagicSquare_data::printMagicSquareDetails() const {
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
    for (const auto & i : square_arr) {
        sum += i.second;
    }
    std::cout << "Sum: " << sum << std::endl;

    std::cout << "\n\nAscending list: " << std::endl;
    std::set<int> numbers;
    std::vector<int> the_numbers;
    int firstLowest = getLowestValuesIndex();
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
    for (const int the_number : the_numbers) {
        std::cout << "Sqrt: " << square_arr[the_number].first << " Value: " << square_arr[the_number].second << std::endl;
    }
}

bool MagicSquare_data::isMagicSquare() const {

    //Are all numbers unique?
    for (int i = 0; i < 9; i++) {
        for (int j = i+1; j < 9; j++) {
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

    std::cout << "Magic square of square of squares found!" << std::endl;
    printMagicSquare();
    return true;
}

bool MagicSquare_data::contains(mpz_int &value, const int excludeIndex) const {
    for (int i = 0; i < LAST_VAL; ++i) {
        if (i == excludeIndex) { continue; }
        if (square_arr[i].second == value) { return true; }
    }
    return false;
}

bool MagicSquare_data::areUncommonSumsHigher() const {
    int higher = 0;
    int lower = 0;
    for (const auto & i : square_arr) {
        if (i.second > most_common_sum) {++higher;}
        else if (i.second < most_common_sum) {++lower;}
    }
    return higher > lower;
}

int MagicSquare_data::isThereACommonElementFromBadSums() {
    current_focus = -1; //reset focus

    if (areUncommonSumsHigher()) {
        //If top row and left column sums are greater than most_common
        if (sums[row_top] > most_common_sum && sums[col_lft] > most_common_sum) {
            //Upper left is one to work on
            current_focus = top_lft;
        }
        if (sums[row_cen] > most_common_sum && sums[col_lft] > most_common_sum) {
            current_focus = cen_lft;
        }
        if (sums[row_bot] > most_common_sum && sums[col_lft] > most_common_sum) {
            current_focus = bot_lft;
        }
        if (sums[row_top] > most_common_sum && sums[col_cen] > most_common_sum) {
            current_focus = top_cen;
        }
        if (sums[row_bot] > most_common_sum && sums[col_cen] > most_common_sum) {
            current_focus = bot_cen;
        }
        if (sums[row_top] > most_common_sum && sums[col_rgt] > most_common_sum) {
            current_focus = top_rgt;
        }
        if (sums[row_cen] > most_common_sum && sums[col_rgt] > most_common_sum) {
            current_focus = cen_rgt;
        }
        if (sums[row_bot] > most_common_sum && sums[col_rgt] > most_common_sum) {
            current_focus = bot_rgt;
        }
    }
    else {
        if (sums[row_top] < most_common_sum && sums[col_lft] < most_common_sum) {
            current_focus = top_lft;
        }
        if (sums[row_cen] < most_common_sum && sums[col_lft] < most_common_sum) {
            current_focus = cen_lft;
        }
        if (sums[row_bot] < most_common_sum && sums[col_lft] < most_common_sum) {
            current_focus = bot_lft;
        }
        if (sums[row_top] < most_common_sum && sums[col_cen] < most_common_sum) {
            current_focus = top_cen;
        }
        if (sums[row_bot] < most_common_sum && sums[col_cen] < most_common_sum) {
            current_focus = bot_cen;
        }
        if (sums[row_top] < most_common_sum && sums[col_rgt] < most_common_sum) {
            current_focus = top_rgt;
        }
        if (sums[row_cen] < most_common_sum && sums[col_rgt] < most_common_sum) {
            current_focus = cen_rgt;
        }
        if (sums[row_bot] < most_common_sum && sums[col_rgt] < most_common_sum) {
            current_focus = bot_rgt;
        }
    }
    return current_focus;
}

void MagicSquare_data::swapLowest(const int counter) {
    square_arr[getLowestValuesIndex()].second = counter * counter;
}

void MagicSquare_data::swapTwoIndices(const int swap1, const int swap2) {
    std::swap(square_arr[swap1], square_arr[swap2]);
}

//Get the index into the array of the lowest value. Can use first or second.
int MagicSquare_data::getLowestValuesIndex() const {
    int lowest = 0;
    for (int i = 1; i < 9; i++) {
        if (square_arr[i].second < square_arr[lowest].second) {
            lowest = i;
        }
    }
    return lowest;
}

void MagicSquare_data::incrementAValueAtIndex(const int index, bool whileNotInArray) {
    if (index >= LAST_VAL) {std::cout << "INVALID INDEX!\n"; return;}
    ++square_arr[index].first;
    square_arr[index].second = square_arr[index].first * square_arr[index].first;

    if (whileNotInArray) {
        if (contains(square_arr[index].second, index)) {
            incrementAValueAtIndex(index, whileNotInArray);
        }
    }
}

void MagicSquare_data::decrementAValueAtIndex(const int index, bool whileNotInArray) {
    if (index >= LAST_VAL) {std::cout << "INVALID INDEX!\n"; return;}
    --square_arr[index].first;
    square_arr[index].second = square_arr[index].first * square_arr[index].first;

    if (whileNotInArray) {
     if (contains(square_arr[index].second, index)) {
        decrementAValueAtIndex(index, whileNotInArray);
     }
    }
}

mpz_int MagicSquare_data::calculateMostCommonSum(bool print = false) {
    std::map<mpz_int, int> setOfSums;
    //row 1 and init
    mpz_int sum = square_arr[0].second + square_arr[1].second + square_arr[2].second;
    setOfSums[sum] = 1;
    sums[0] = sum;

    sum = square_arr[3].second + square_arr[4].second + square_arr[5].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[1] = sum;

    sum = square_arr[6].second + square_arr[7].second + square_arr[8].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[2] = sum;

    //col 1
    sum = square_arr[0].second + square_arr[3].second + square_arr[6].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[3] = sum;

    sum = square_arr[1].second + square_arr[4].second + square_arr[7].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[4] = sum;

    sum = square_arr[2].second + square_arr[5].second + square_arr[8].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[5] = sum;

    //Diagonals
    sum = square_arr[0].second + square_arr[4].second + square_arr[8].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[6] = sum;

    sum = square_arr[2].second + square_arr[4].second + square_arr[6].second;
    if (setOfSums.contains(sum)) setOfSums[sum]++;
    else setOfSums[sum] = 1;
    sums[7] = sum;

    auto commonSum = setOfSums[sum];
    for (const auto& commonSumIt : setOfSums) {
        //if (print) std::cout << commonSumIt.first << " Count:" << commonSumIt.second << "\n";
        if (commonSumIt.second > commonSum) {
            sum = commonSumIt.first;
            commonSum = commonSumIt.second;
        }
    }

    if (print) {
        std::cout <<"\nMost common sum: " << sum << " Count: " << setOfSums[sum] << "\n";
    }
    most_common_sum = sum;
    return setOfSums[sum];
}

void MagicSquare_data::initializeSquares(std::pair<mpz_int, mpz_int>* square_arr) {
    for (int i = 0; i < 9; ++i) {
        square_arr[i].first = sqrt(square_arr[i].second);
    }
}
