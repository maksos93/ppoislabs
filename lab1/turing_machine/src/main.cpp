/**
 * @file main.cpp
 * @brief CLI application to demonstrate Turing Machine execution.
 */

#include <iostream>
#include "turing_machine.hpp"

namespace {
    const int kExitChoice = 0;
    const int kRunDemo = 1;
    const int kStepDemo = 2;

    void printMenu() {
        std::cout << "\n=== TURING MACHINE SIMULATOR ===\n"
                  << "1. Run binary incrementer demo\n"
                  << "2. Step-by-step execution demo\n"
                  << "0. Exit\n"
                  << "Select option: ";
    }

    // Пример машины: двоичный инкрементатор (добавляет 1 к числу в двоичной системе)
    void configureBinaryIncrementer(TuringMachine& tm) {
        // Состояния: q0 (идем вправо до конца), q1 (перенос единицы влево), q_halt (стоп)
        tm.addRule("q0", '0', "q0", '0', TuringMachine::Direction::RIGHT);
        tm.addRule("q0", '1', "q0", '1', TuringMachine::Direction::RIGHT);
        tm.addRule("q0", '_', "q1", '_', TuringMachine::Direction::LEFT);

        tm.addRule("q1", '1', "q1", '0', TuringMachine::Direction::LEFT);
        tm.addRule("q1", '0', "q_halt", '1', TuringMachine::Direction::STAY);
        tm.addRule("q1", '_', "q_halt", '1', TuringMachine::Direction::STAY);
    }

    void runDemo(bool stepByStep) {
        TuringMachine tm("q0", "q_halt");
        configureBinaryIncrementer(tm);

        std::string input;
        std::cout << "Enter binary number (e.g. 1011): ";
        std::cin >> input;

        tm.setTape(input, 0);
        std::cout << "Initial tape: " << tm.getTapeAsString() << "\n";

        if (stepByStep) {
            int stepCount = 0;
            while (!tm.isHalted()) {
                std::cout << "Step " << ++stepCount << " | State: " << tm.getCurrentState() 
                          << " | Tape: " << tm.getTapeAsString() 
                          << " | Head at pos: " << tm.getHeadPosition() << "\n";
                if (!tm.step()) {
                    std::cout << "Halted due to missing transition.\n";
                    break;
                }
            }
        } else {
            tm.run();
        }

        std::cout << "Final tape result: " << tm.getTapeAsString() << "\n";
    }
}

int main() {
    int choice = -1;
    do {
        printMenu();
        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case kRunDemo: runDemo(false); break;
            case kStepDemo: runDemo(true); break;
            case kExitChoice: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice.\n"; break;
        }
    } while (choice != kExitChoice);

    return 0;
}