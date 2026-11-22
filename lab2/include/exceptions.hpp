/**
 * @file exceptions.hpp
 * @brief Custom exception classes for the Library and Bookstore domain.
 */

#pragma once

#include <stdexcept>
#include <string>

/**
 * @class LibraryException
 * @brief Base runtime exception for all domain errors in the library and bookstore system.
 */
class LibraryException : public std::runtime_error {
public:
    /**
     * @brief Constructs a base LibraryException with an error description.
     * @param message Human-readable error message.
     */
    explicit LibraryException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @class BookNotFoundException
 * @brief Thrown when a requested book cannot be located in the catalog by ISBN.
 */
class BookNotFoundException : public LibraryException {
public:
    /**
     * @brief Constructs BookNotFoundException for a missing ISBN.
     * @param isbn ISBN identifier that was not found.
     */
    explicit BookNotFoundException(const std::string& isbn)
        : LibraryException("Book not found with ISBN: " + isbn) {}
};

/**
 * @class DuplicateBookException
 * @brief Thrown when attempting to register a book whose ISBN already exists in the catalog.
 */
class DuplicateBookException : public LibraryException {
public:
    /**
     * @brief Constructs DuplicateBookException for a duplicate ISBN.
     * @param isbn Duplicate ISBN identifier.
     */
    explicit DuplicateBookException(const std::string& isbn)
        : LibraryException("Duplicate book with ISBN: " + isbn) {}
};

/**
 * @class OutOfStockException
 * @brief Thrown when requested copies exceed available stock quantity.
 */
class OutOfStockException : public LibraryException {
public:
    /**
     * @brief Constructs OutOfStockException for an unavailable book.
     * @param title Title of the book that is out of stock.
     */
    explicit OutOfStockException(const std::string& title)
        : LibraryException("Out of stock for book: " + title) {}
};

/**
 * @class InvalidBookDataException
 * @brief Thrown when book attributes (price, stock, quantity, appraisal) violate domain constraints.
 */
class InvalidBookDataException : public LibraryException {
public:
    /**
     * @brief Constructs InvalidBookDataException with validation details.
     * @param details Description of the invalid book attribute.
     */
    explicit InvalidBookDataException(const std::string& details)
        : LibraryException("Invalid book data: " + details) {}
};

/**
 * @class InvalidUserException
 * @brief Thrown when user registration, lookup, or profile data is invalid.
 */
class InvalidUserException : public LibraryException {
public:
    /**
     * @brief Constructs InvalidUserException with user error context.
     * @param user Details of the invalid user record or identifier.
     */
    explicit InvalidUserException(const std::string& user)
        : LibraryException("Invalid user data: " + user) {}
};

/**
 * @class OrderNotFoundException
 * @brief Thrown when an order is missing or cannot be confirmed due to empty state.
 */
class OrderNotFoundException : public LibraryException {
public:
    /**
     * @brief Constructs OrderNotFoundException for a specific order ID.
     * @param orderId Numeric identifier of the order.
     */
    explicit OrderNotFoundException(int orderId)
        : LibraryException("Order not found with ID: " + std::to_string(orderId)) {}
};

/**
 * @class InvalidPaymentException
 * @brief Thrown when payment verification or settlement fails.
 */
class InvalidPaymentException : public LibraryException {
public:
    /**
     * @brief Constructs InvalidPaymentException with failure details.
     * @param details Reason why payment processing failed.
     */
    explicit InvalidPaymentException(const std::string& details)
        : LibraryException("Invalid payment processing: " + details) {}
};

/**
 * @class InsufficientFundsException
 * @brief Thrown when tendered cash, credit limit, or loyalty points are below the required amount.
 */
class InsufficientFundsException : public LibraryException {
public:
    /**
     * @brief Constructs InsufficientFundsException comparing required and available balances.
     * @param required Amount required to complete the operation.
     * @param available Amount actually available.
     */
    explicit InsufficientFundsException(double required, double available)
        : LibraryException("Insufficient funds. Required: " + std::to_string(required) +
                           ", Available: " + std::to_string(available)) {}
};

/**
 * @class DeliveryException
 * @brief Thrown when shipment dispatch or courier order assignment encounters an error.
 */
class DeliveryException : public LibraryException {
public:
    /**
     * @brief Constructs DeliveryException with logistics error details.
     * @param reason Explanation of the delivery failure.
     */
    explicit DeliveryException(const std::string& reason)
        : LibraryException("Delivery error: " + reason) {}
};

/**
 * @class UnauthorizedAccessException
 * @brief Thrown when a user, card, or staff role lacks permission for a restricted operation.
 */
class UnauthorizedAccessException : public LibraryException {
public:
    /**
     * @brief Constructs UnauthorizedAccessException for a role or security context.
     * @param role Role or entity that failed authorization.
     */
    explicit UnauthorizedAccessException(const std::string& role)
        : LibraryException("Unauthorized access for role: " + role) {}
};

/**
 * @class InvalidDiscountException
 * @brief Thrown when a discount percentage or eligibility condition is invalid.
 */
class InvalidDiscountException : public LibraryException {
public:
    /**
     * @brief Constructs InvalidDiscountException for an invalid discount value.
     * @param value Numeric value that triggered the validation error.
     */
    explicit InvalidDiscountException(double value)
        : LibraryException("Invalid discount value: " + std::to_string(value)) {}
};

/**
 * @class ReviewNotFoundException
 * @brief Thrown when attempting to access or delete a review ID that does not exist.
 */
class ReviewNotFoundException : public LibraryException {
public:
    /**
     * @brief Constructs ReviewNotFoundException for a missing review ID.
     * @param reviewId Numeric identifier of the missing review.
     */
    explicit ReviewNotFoundException(int reviewId)
        : LibraryException("Review not found with ID: " + std::to_string(reviewId)) {}
};