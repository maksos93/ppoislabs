/**
 * @file main.cpp
 * @brief CLI application for Library System (Lab 2).
 */

#include <iostream>
#include "book.hpp"
#include "exceptions.hpp"

namespace {
    const int kExitChoice = 0;
    const int kAddBook = 1;
    const int kSearchBook = 2;
    const int kRemoveBook = 3;
    const int kShowStats = 4;

    void printMenu() {
        std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM (LAB 2) ===\n"
                  << "1. Add new book\n"
                  << "2. Find book by ISBN\n"
                  << "3. Remove book\n"
                  << "4. Show total catalog size\n"
                  << "0. Exit\n"
                  << "Select option: ";
    }

    void handleAdd(Catalog& catalog) {
        std::string isbn, title, author;
        double price = 0.0;
        int stock = 0;

        std::cout << "Enter ISBN: ";
        std::cin >> isbn;
        std::cout << "Enter Title: ";
        std::cin >> title;
        std::cout << "Enter Author: ";
        std::cin >> author;
        std::cout << "Enter Price: ";
        std::cin >> price;
        std::cout << "Enter Stock quantity: ";
        std::cin >> stock;

        try {
            catalog.addBook(Book(isbn, title, author, price, stock));
            std::cout << "Book successfully added to catalog.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleSearch(const Catalog& catalog) {
        std::string isbn;
        std::cout << "Enter ISBN to search: ";
        std::cin >> isbn;
        try {
            const Book& book = catalog.getBook(isbn);
            std::cout << "Found: " << book.getTitle() << " by " << book.getAuthor() 
                      << " | Price: $" << book.getPrice() << " | Stock: " << book.getStockQuantity() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleRemove(Catalog& catalog) {
        std::string isbn;
        std::cout << "Enter ISBN to remove: ";
        std::cin >> isbn;
        try {
            catalog.removeBook(isbn);
            std::cout << "Book removed successfully.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    Catalog catalog;
    int choice = -1;

    do {
        printMenu();
        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case kAddBook: handleAdd(catalog); break;
            case kSearchBook: handleSearch(catalog); break;
            case kRemoveBook: handleRemove(catalog); break;
            case kShowStats: std::cout << "Total books in catalog: " << catalog.getBookCount() << "\n"; break;
            case kExitChoice: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid option. Try again.\n"; break;
        }
    } while (choice != kExitChoice);

    return 0;
}