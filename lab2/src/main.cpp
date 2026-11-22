/**
 * @file main.cpp
 * @brief CLI application for Library and Bookstore System (Lab 2).
 */

#include <iostream>
#include "book.hpp"
#include "users.hpp"
#include "orders.hpp"
#include "staff.hpp"
#include "exceptions.hpp"

namespace {
    const int kExitChoice = 0;
    const int kAddBook = 1;
    const int kSearchBook = 2;
    const int kRemoveBook = 3;
    const int kShowStats = 4;
    const int kSimulateOrder = 5;

    void printMenu() {
        std::cout << "\n=== LIBRARY & BOOKSTORE SYSTEM (LAB 2) ===\n"
                  << "1. Add new book\n"
                  << "2. Find book by ISBN\n"
                  << "3. Remove book\n"
                  << "4. Show total catalog size\n"
                  << "5. Simulate order & payment workflow\n"
                  << "0. Exit\n"
                  << "Select option: ";
    }

    void handleAdd(CatalogManager& catalog) {
        std::string isbn, title, authorName, genreName;
        double price = 0.0;
        int stock = 0;

        std::cout << "Enter ISBN: ";
        std::cin >> isbn;
        std::cout << "Enter Title: ";
        std::cin >> title;
        std::cout << "Enter Author: ";
        std::cin >> authorName;
        std::cout << "Enter Genre: ";
        std::cin >> genreName;
        std::cout << "Enter Price: ";
        std::cin >> price;
        std::cout << "Enter Stock quantity: ";
        std::cin >> stock;

        try {
            Author author(1, authorName, "International");
            Publisher publisher("Academic Press", "PUB-01", Address("1 Main St", "London", "SW1A 1AA", "UK"));
            Genre genre(genreName, genreName + " literature");
            catalog.addBook(Book(isbn, title, author, publisher, genre, price, stock));
            std::cout << "Book successfully added to catalog.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleSearch(const CatalogManager& catalog) {
        std::string isbn;
        std::cout << "Enter ISBN to search: ";
        std::cin >> isbn;
        try {
            const Book& book = catalog.getBook(isbn);
            std::cout << "Found: " << book.getTitle() << " by " << book.getAuthor().getFullName()
                      << " | Price: $" << book.getPrice() << " | Stock: " << book.getStockQuantity() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleRemove(CatalogManager& catalog) {
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

    void handleOrderSimulation(CatalogManager& catalog) {
        try {
            if (catalog.getBookCount() == 0) {
                Author a(10, "Robert Martin", "USA");
                Publisher p("Pearson", "PRS-99", Address("221B Baker St", "London", "NW1 6XE", "UK"));
                Genre g("Software Engineering", "Programming best practices");
                catalog.addBook(Book("978-0132350884", "CleanCode", a, p, g, 45.0, 5));
            }
            UserRegistry registry;
            Address addr("10 Downing St", "London", "SW1A 2AA", "UK");
            User customer = registry.registerUser("Ivan Petrov", "ivan@example.com", addr);

            const Book& targetBook = catalog.getBook("978-0132350884");
            Order order(101, customer);
            order.addItem(targetBook, 1);
            order.applyDiscount(Discount("WELCOME10", 10.0, 20.0));

            Invoice invoice("INV-2026-101", order, 0.20);
            CreditCardPayment payment("TX-9001", invoice.calculateGrandTotal(), "****-1234", "12/28", 500.0);

            TransactionManager txManager;
            if (txManager.settleOrderPayment(order, invoice, payment)) {
                std::cout << "Order #" << order.getOrderId() << " settled! Grand total: $"
                          << invoice.calculateGrandTotal() << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Simulation error: " << e.what() << "\n";
        }
    }
}

int main() {
    CatalogManager catalog;
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
            case kSimulateOrder: handleOrderSimulation(catalog); break;
            case kExitChoice: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid option. Try again.\n"; break;
        }
    } while (choice != kExitChoice);

    return 0;
}
