/**
 * @file book.cpp
 * @brief Implementation of Book and Catalog methods.
 */

#include "book.hpp"
#include "exceptions.hpp"

Book::Book(const std::string& isbn, const std::string& title, 
           const std::string& author, double price, int stock)
    : isbn(isbn), title(title), author(author), price(price), stockQuantity(stock) {}

std::string Book::getIsbn() const { return isbn; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
double Book::getPrice() const { return price; }
int Book::getStockQuantity() const { return stockQuantity; }

void Book::setStockQuantity(int quantity) {
    if (quantity < 0) {
        throw InvalidPaymentException("Stock quantity cannot be negative.");
    }
    stockQuantity = quantity;
}

void Book::updatePrice(double newPrice) {
    if (newPrice < 0.0) {
        throw InvalidPaymentException("Book price cannot be negative.");
    }
    price = newPrice;
}

void Catalog::addBook(const Book& book) {
    if (books.find(book.getIsbn()) != books.end()) {
        throw DuplicateBookException(book.getIsbn());
    }
    books.insert({book.getIsbn(), book});
}

void Catalog::removeBook(const std::string& isbn) {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    books.erase(it);
}

Book& Catalog::getBook(const std::string& isbn) {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    return it->second;
}

const Book& Catalog::getBook(const std::string& isbn) const {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    return it->second;
}

size_t Catalog::getBookCount() const {
    return books.size();
}