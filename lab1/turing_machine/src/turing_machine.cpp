/**
 * @file turing_machine.cpp
 * @brief Implementation of the TuringMachine class methods.
 */

#include "turing_machine.hpp"
#include <stdexcept>
#include <algorithm>

TuringMachine::TuringMachine(const std::string& startState, const std::string& haltState)
    : headPosition(0), currentState(startState), haltState(haltState) {
    tape.push_back('_'); // Инициализация пустой ленты
}

char TuringMachine::charFromDirection(Direction dir) const {
    if (dir == Direction::LEFT) return 'L';
    if (dir == Direction::RIGHT) return 'R';
    return 'S';
}

TuringMachine::Direction TuringMachine::directionFromChar(char c) const {
    if (c == 'L' || c == 'l') return Direction::LEFT;
    if (c == 'R' || c == 'r') return Direction::RIGHT;
    return Direction::STAY;
}

void TuringMachine::expandTapeIfNeeded(int position) {
    if (position < 0) {
        tape.insert(tape.begin(), '_');
        headPosition++;
    } else if (static_cast<size_t>(position) >= tape.size()) {
        tape.push_back('_');
    }
}

void TuringMachine::addRule(const std::string& state, char symbol, 
                            const std::string& nextState, char writeSymbol, Direction dir) {
    rules[{state, symbol}] = {nextState, writeSymbol, dir};
}

void TuringMachine::setTape(const std::string& inputTape, int initialHeadPos) {
    tape.clear();
    if (inputTape.empty()) {
        tape.push_back('_');
    } else {
        for (char c : inputTape) {
            tape.push_back(c);
        }
    }
    headPosition = std::clamp(initialHeadPos, 0, static_cast<int>(tape.size()) - 1);
}

bool TuringMachine::step() {
    if (isHalted()) {
        return false;
    }

    expandTapeIfNeeded(headPosition);
    char currentSymbol = tape[headPosition];

    auto it = rules.find({currentState, currentSymbol});
    if (it == rules.end()) {
        // Нет правила перехода — аварийная остановка
        return false;
    }

    const Transition& tx = it->second;
    tape[headPosition] = tx.writeSymbol;
    currentState = tx.nextState;

    if (tx.moveDirection == Direction::LEFT) {
        headPosition--;
    } else if (tx.moveDirection == Direction::RIGHT) {
        headPosition++;
    }

    expandTapeIfNeeded(headPosition);
    return true;
}

void TuringMachine::run() {
    while (!isHalted()) {
        if (!step()) {
            break;
        }
    }
}

bool TuringMachine::isHalted() const {
    return currentState == haltState;
}

std::string TuringMachine::getTapeAsString() const {
    std::string result(tape.begin(), tape.end());
    // Удаляем пустые символы с конца строки
    while (!result.empty() && result.back() == '_') {
        result.pop_back();
    }
    return result;
}

std::string TuringMachine::getCurrentState() const {
    return currentState;
}

int TuringMachine::getHeadPosition() const {
    return headPosition;
}