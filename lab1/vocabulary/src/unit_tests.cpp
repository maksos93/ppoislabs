/**
 * @file unit_tests.cpp
 * @brief Unit tests for the Dictionary class.
 */

#include "dictionary.hpp"
#include <iostream>
#include <cassert>

void runTests() {
    Dictionary d1;
    d1 += std::make_pair("cat", "кот");
    d1 += std::make_pair("dog", "собака");
    d1 += std::make_pair("bird", "птица");
    assert(d1.size() == 3);

    assert(d1["cat"] == "кот");
    assert(d1["dog"] == "собака");

    d1["cat"] = "кошка";
    assert(d1["cat"] == "кошка");

    d1 -= "dog";
    assert(d1.size() == 2);
    assert(d1["dog"] == "WORD_NOT_FOUND");

    Dictionary d2 = d1;
    assert(d2.size() == 2);
    assert(d2 == d1);

    d2 += std::make_pair("fish", "рыба");
    assert(d2 != d1);

    std::cout << "All Dictionary unit-tests passed successfully!\n";
}

int main() {
    runTests();
    return 0;
}