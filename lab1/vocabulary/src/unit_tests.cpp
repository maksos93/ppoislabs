/**
 * @file unit_tests.cpp
 * @brief Unit tests for the Dictionary class.
 */

#include "dictionary.hpp"
#include <iostream>
#include <cassert>
#include <utility>
#include <stdexcept>
#include <cstdio>

void runTests() {
    // 1. Проверка пустого словаря при инициализации
    Dictionary emptyDict;
    assert(emptyDict.size() == 0);

    // 2. Константный оператор [] возвращает "WORD_NOT_FOUND" для отсутствующего слова и не меняет размер
    const Dictionary& constEmpty = emptyDict;
    assert(constEmpty["unknown"] == "WORD_NOT_FOUND");
    assert(emptyDict.size() == 0);

    // 3. Добавление элементов через оператор += (std::string и const char*)
    Dictionary d1;
    d1 += std::make_pair(std::string("cat"), std::string("кот"));
    d1 += std::make_pair("dog", "собака");
    d1 += std::make_pair("bird", "птица");
    assert(d1.size() == 3);

    // 4. Поиск значений по ключу через оператор []
    assert(d1["cat"] == "кот");
    assert(d1["dog"] == "собака");
    assert(d1["bird"] == "птица");

    // 5. Обновление существующего значения через неконстантный оператор []
    d1["cat"] = "кошка";
    assert(d1["cat"] == "кошка");
    assert(d1.size() == 3);

    // 6. Неконстантный оператор [] для нового ключа добавляет его с пустой строкой
    d1["fox"] = "лиса";
    assert(d1["fox"] == "лиса");
    assert(d1.size() == 4);

    // 7. Удаление узла без детей (листа) через оператор -=
    d1 -= "bird";
    assert(d1.size() == 3);
    const Dictionary& constD1 = d1;
    assert(constD1["bird"] == "WORD_NOT_FOUND");

    // 8. Удаление несуществующего ключа не меняет размер и структуру
    d1 -= "nonexistent_word";
    assert(d1.size() == 3);

    // 9. Удаление узла с двумя детьми (проверка BST-перестройки)
    Dictionary bstDict;
    bstDict += std::make_pair("m", "м");
    bstDict += std::make_pair("d", "д");
    bstDict += std::make_pair("r", "р");
    bstDict += std::make_pair("b", "б");
    bstDict += std::make_pair("f", "ф");
    bstDict += std::make_pair("p", "п");
    bstDict += std::make_pair("z", "з");
    bstDict -= "d";
    assert(bstDict.size() == 6);
    assert(bstDict["b"] == "б");
    assert(bstDict["f"] == "ф");
    assert(bstDict["m"] == "м");
    assert(bstDict["r"] == "р");

    // 10. Конструктор копирования и оператор равенства ==
    Dictionary d2(d1);
    assert(d2.size() == d1.size());
    assert(d2 == d1);

    // 11. Оператор неравенства != после модификации копии
    d2 += std::make_pair("fish", "рыба");
    assert(d2.size() == d1.size() + 1);
    assert(d2 != d1);

    // 12. Оператор присваивания копированием и самоприсваивание
    Dictionary d3;
    d3 += std::make_pair("apple", "яблоко");
    d3 = d1;
    assert(d3 == d1);
    assert(d3.size() == d1.size());

    d3 = d3;
    assert(d3 == d1);
    assert(d3.size() == d1.size());

    // 13. Перезапись существующего перевода через оператор +=
    d1 += std::make_pair("dog", "пёс");
    assert(d1["dog"] == "пёс");
    assert(d1.size() == 3);

    // 14. Загрузка словаря из файла и проверка исключения при отсутствии файла
    const char* testFile = "test_dict_temp.txt";
    {
        std::ofstream out(testFile);
        out << "sun солнце\nmoon луна\nstar звезда\n";
    }
    Dictionary fileDict;
    fileDict.loadFromFile(testFile);
    assert(fileDict.size() == 3);
    assert(fileDict["sun"] == "солнце");
    assert(fileDict["moon"] == "луна");
    assert(fileDict["star"] == "звезда");
    std::remove(testFile);

    bool exceptionThrown = false;
    try {
        fileDict.loadFromFile("missing_file_12345.txt");
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // 15. Очистка словаря удалением всех элементов по очереди
    Dictionary d5;
    d5 += std::make_pair("one", "один");
    d5 += std::make_pair("two", "два");
    d5 -= "one";
    d5 -= "two";
    assert(d5.size() == 0);
    assert(d5 == emptyDict);

    std::cout << "All 15 Dictionary unit-tests passed successfully!\n";
}

int main() {
    runTests();
    return 0;
}