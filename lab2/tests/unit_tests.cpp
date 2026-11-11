/**
 * @file unit_tests.cpp
 * @brief Unit tests for Lab 2 domain classes and exceptions.
 */

#include "book.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <cassert>

void runTests() {
    Catalog catalog;
    
    // Тест 1: Добавление и поиск
    Book b1("978-5-1", "Clean Code", "Robert Martin", 45.50, 10);
    catalog.addBook(b1);
    assert(catalog.getBookCount() == 1);
    
    const Book& found = catalog.getBook("978-5-1");
    assert(found.getTitle() == "Clean Code");
    assert(found.getPrice() == 45.50);

    // Тест 2: Дубликаты (проверка исключения)
    bool duplicateCaught = false;
    try {
        catalog.addBook(b1);
    } catch (const DuplicateBookException& e) {
        duplicateCaught = true;
    }
    assert(duplicateCaught);

    // Тест 3: Несуществующая книга
    bool notFoundCaught = false;
    try {
        catalog.getBook("000-0-0");
    } catch (const BookNotFoundException& e) {
        notFoundCaught = true;
    }
    assert(notFoundCaught);

    // Тест 4: Удаление
    catalog.removeBook("978-5-1");
    assert(catalog.getBookCount() == 0);

    std::cout << "All Lab 2 unit tests passed successfully!\n";
}

int main() {
    runTests();
    return 0;
}