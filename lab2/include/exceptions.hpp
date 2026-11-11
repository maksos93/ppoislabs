/**
 * @file exceptions.hpp
 * @brief Custom exception classes for the Library system.
 */

#pragma once

#include <stdexcept>
#include <string>

class LibraryException : public std::runtime_error {
public:
    explicit LibraryException(const std::string& message) : std::runtime_error(message) {}
};

class BookNotFoundException : public LibraryException {
public:
    explicit BookNotFoundException(const std::string& isbn) 
        : LibraryException("Book not found with ISBN: " + isbn) {}
};

class DuplicateBookException : public LibraryException {
public:
    explicit DuplicateBookException(const std::string& isbn) 
        : LibraryException("Duplicate book with ISBN: " + isbn) {}
};

class OutOfStockException : public LibraryException {
public:
    explicit OutOfStockException(const std::string& title) 
        : LibraryException("Out of stock for book: " + title) {}
};

class InvalidUserException : public LibraryException {
public:
    explicit InvalidUserException(const std::string& user) 
        : LibraryException("Invalid user data: " + user) {}
};

class OrderNotFoundException : public LibraryException {
public:
    explicit OrderNotFoundException(int orderId) 
        : LibraryException("Order not found with ID: " + std::to_string(orderId)) {}
};

class InvalidPaymentException : public LibraryException {
public:
    explicit InvalidPaymentException(const std::string& details) 
        : LibraryException("Invalid payment processing: " + details) {}
};

class InsufficientFundsException : public LibraryException {
public:
    explicit InsufficientFundsException(double required, double available) 
        : LibraryException("Insufficient funds. Required: " + std::to_string(required) + 
                           ", Available: " + std::to_string(available)) {}
};

class DeliveryException : public LibraryException {
public:
    explicit DeliveryException(const std::string& reason) 
        : LibraryException("Delivery error: " + reason) {}
};

class UnauthorizedAccessException : public LibraryException {
public:
    explicit UnauthorizedAccessException(const std::string& role) 
        : LibraryException("Unauthorized access for role: " + role) {}
};

class InvalidDiscountException : public LibraryException {
public:
    explicit InvalidDiscountException(double value) 
        : LibraryException("Invalid discount value: " + std::to_string(value)) {}
};

class ReviewNotFoundException : public LibraryException {
public:
    explicit ReviewNotFoundException(int reviewId) 
        : LibraryException("Review not found with ID: " + std::to_string(reviewId)) {}
};