/**
 * @file book.hpp
 * @brief Book and Catalog management definitions.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

class Book {
private:
    std::string isbn;
    std::string title;
    std::string author;
    double price;
    int stockQuantity;

public:
    Book(const std::string& isbn, const std::string& title, 
         const std::string& author, double price, int stock);

    std::string getIsbn() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    double getPrice() const;
    int getStockQuantity() const;

    void setStockQuantity(int quantity);
    void updatePrice(double newPrice);
};

class Catalog {
private:
    std::map<std::string, Book> books;

public:
    Catalog() = default;

    void addBook(const Book& book);
    void removeBook(const std::string& isbn);
    Book& getBook(const std::string& isbn);
    const Book& getBook(const std::string& isbn) const;
    size_t getBookCount() const;
};