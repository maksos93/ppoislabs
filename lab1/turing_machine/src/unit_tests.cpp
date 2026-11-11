/**
 * @file unit_tests.cpp
 * @brief Unit tests for the TuringMachine class.
 */

#include "turing_machine.hpp"
#include <iostream>
#include <cassert>

void runTests() {
    // Тест простого автомата, инвертирующего нули и единицы
    TuringMachine tm("q0", "q_halt");
    
    tm.addRule("q0", '0', "q0", '1', TuringMachine::Direction::RIGHT);
    tm.addRule("q0", '1', "q0", '0', TuringMachine::Direction::RIGHT);
    tm.addRule("q0", '_', "q_halt", '_', TuringMachine::Direction::STAY);

    tm.setTape("1010", 0);
    tm.run();

    assert(tm.getTapeAsString() == "0101");
    assert(tm.isHalted() == true);

    std::cout << "All Turing Machine unit-tests passed successfully!\n";
}

int main() {
    runTests();
    return 0;
}