/**
 * @file turing_machine.hpp
 * @brief Заголовочный файл класса TuringMachine.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * @class TuringMachine
 * @brief Класс, реализующий симулятор машины Тьюринга.
 */
class TuringMachine {
public:
    /** @brief Направления движения головки ленты. */
    enum class Direction { LEFT, RIGHT, STAY };

    /**
     * @struct Transition
     * @brief Правило перехода для машины Тьюринга.
     */
    struct Transition {
        std::string nextState;
        char writeSymbol;
        Direction moveDirection;
    };

private:
    std::vector<char> tape;
    int headPosition;
    std::string currentState;
    std::string haltState;
    std::map<std::pair<std::string, char>, Transition> rules;

    void expandTapeIfNeeded(int position);
    char charFromDirection(Direction dir) const;
    Direction directionFromChar(char c) const;

public:
    /**
     * @brief Конструктор машины Тьюринга.
     * @param startState Начальное состояние.
     * @param haltState Состояние остановки.
     */
    TuringMachine(const std::string& startState, const std::string& haltState);

    /**
     * @brief Добавление правила перехода в таблицу переходов.
     * @param state Текущее состояние.
        * @param symbol Считываемый символ.
     * @param nextState Следующее состояние.
     * @param writeSymbol Символ для записи на ленту.
     * @param dir Направление движения головки.
     */
    void addRule(const std::string& state, char symbol, 
                 const std::string& nextState, char writeSymbol, Direction dir);
    
    /**
     * @brief Установка содержимого ленты.
     * @param inputTape Строка с начальными символами.
     * @param initialHeadPos Начальная позиция головки.
     */
    void setTape(const std::string& inputTape, int initialHeadPos = 0);
    
    /**
     * @brief Выполнение одного шага (такта) машины.
     * @return true, если шаг выполнен успешно, false в случае остановки.
     */
    bool step();

    /** @brief Запуск машины до достижения состояния остановки. */
    void run();

    /** @brief Получение текущего содержимого ленты в виде строки. */
    std::string getTapeAsString() const;

    /** @brief Получение текущего состояния машины. */
    std::string getCurrentState() const;

    /** @brief Получение текущей позиции головки на ленте. */
    int getHeadPosition() const;

    /** @brief Проверка, остановлена ли машина. */
    bool isHalted() const;
};