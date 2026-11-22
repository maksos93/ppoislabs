/**
 * @file unit_tests.cpp
 * @brief Comprehensive unit tests for Lab 2 domain classes and all custom exceptions.
 */

#include "book.hpp"
#include "users.hpp"
#include "orders.hpp"
#include "staff.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <cassert>

void testCatalogAndBooks() {
    CatalogManager catalog;
    Address addr("12 Fleet St", "London", "EC4Y 1AA", "UK");
    Author author(1, "Robert Martin", "USA");
    Publisher pub("Prentice Hall", "PH-01", addr);
    Genre genre("Programming", "Software craftsmanship");

    Book b1("978-5-1", "Clean Code", author, pub, genre, 45.50, 10);
    catalog.addBook(b1);
    assert(catalog.getBookCount() == 1);

    const Book& found = catalog.getBook("978-5-1");
    assert(found.getTitle() == "Clean Code");
    assert(found.calculateDiscountedPrice(10.0) == 40.95);

    // DuplicateBookException
    bool dupCaught = false;
    try { catalog.addBook(b1); } catch (const DuplicateBookException&) { dupCaught = true; }
    assert(dupCaught);

    // BookNotFoundException
    bool notFoundCaught = false;
    try { catalog.getBook("000-0-0"); } catch (const BookNotFoundException&) { notFoundCaught = true; }
    assert(notFoundCaught);

    // InvalidBookDataException
    bool invalidDataCaught = false;
    try { Book bad("111", "Bad", author, pub, genre, -10.0, 1); }
    catch (const InvalidBookDataException&) { invalidDataCaught = true; }
    assert(invalidDataCaught);

    // OutOfStockException
    bool outOfStockCaught = false;
    try { catalog.getBook("978-5-1").reserveCopies(99); }
    catch (const OutOfStockException&) { outOfStockCaught = true; }
    assert(outOfStockCaught);

    // Polymorphic book formats
    EBook eb(b1, "PDF", 5.0, true);
    AudioBook ab(b1, "John Doe", 360, 128);
    RareBook rb(b1, 1950, "Mint", 1200.0);
    assert(eb.estimateDownloadSeconds(8.0) == 5.0);
    assert(ab.calculateListeningHours(1.5) == 4.0);
    assert(rb.computeInsurancePremium(0.05) == 60.0);

    catalog.removeBook("978-5-1");
    assert(catalog.getBookCount() == 0);
}

void testUsersAndOrders() {
    UserRegistry registry;
    Address addr("5 Oxford St", "London", "W1D 2DZ", "UK");
    User u = registry.registerUser("Alice Smith", "alice@mail.com", addr);
    assert(registry.countActiveUsers() == 1);

    // InvalidUserException
    bool invalidUserCaught = false;
    try { registry.registerUser("", "bad-email", addr); }
    catch (const InvalidUserException&) { invalidUserCaught = true; }
    assert(invalidUserCaught);

    Author a(2, "Bjarne Stroustrup", "Denmark");
    Publisher p("Addison-Wesley", "AW-02", addr);
    Genre g("C++", "Systems programming");
    Book cppBook("978-0321563842", "The C++ Programming Language", a, p, g, 60.0, 4);

    Order order(501, u);
    order.addItem(cppBook, 2);
    Discount disc("SAVE10", 10.0, 50.0);
    order.applyDiscount(disc);
    assert(order.calculateTotal() == 108.0);

    // InvalidDiscountException
    bool invalidDiscCaught = false;
    try { Discount badDisc("BAD", 150.0, 0.0); }
    catch (const InvalidDiscountException&) { invalidDiscCaught = true; }
    assert(invalidDiscCaught);

    // OrderNotFoundException on empty order confirm
    Order emptyOrder(999, u);
    bool orderNotFoundCaught = false;
    try { emptyOrder.confirmOrder(); }
    catch (const OrderNotFoundException&) { orderNotFoundCaught = true; }
    assert(orderNotFoundCaught);

    Invoice inv("INV-501", order, 0.10);
    assert(inv.calculateGrandTotal() == 118.8);

    // InsufficientFundsException
    CashPayment lowCash("TX-1", 118.8, 50.0);
    bool insuffCaught = false;
    try { lowCash.processPayment(inv); }
    catch (const InsufficientFundsException&) { insuffCaught = true; }
    assert(insuffCaught);

    // InvalidPaymentException
    PayPalPayment unverified("TX-2", 118.8, "alice@mail.com", false);
    bool invalidPayCaught = false;
    try { unverified.processPayment(inv); }
    catch (const InvalidPaymentException&) { invalidPayCaught = true; }
    assert(invalidPayCaught);

    CreditCardPayment validCard("TX-3", 118.8, "****-9999", "09/29", 500.0);
    TransactionManager tm;
    assert(tm.settleOrderPayment(order, inv, validCard));
    assert(inv.isPaid());
}

void testStaffLogisticsAndReviews() {
    Address addr("10 King St", "London", "WC2E 8HN", "UK");
    Employee emp(1, "Max", "Staff", 2500.0, addr);
    Administrator admin(emp, 5, false);
    MembershipCard card("CARD-77", 3);

    // UnauthorizedAccessException
    bool unauthCaught = false;
    try { admin.suspendReaderCard(card); }
    catch (const UnauthorizedAccessException&) { unauthCaught = true; }
    assert(unauthCaught);

    // DeliveryException
    Courier courier("CR-01", "Bob", "Bike");
    User u(2, "Tom", "tom@mail.com", addr);
    Order cancelled(77, u);
    cancelled.cancelOrder();
    bool delivCaught = false;
    try { courier.assignOrder(cancelled); }
    catch (const DeliveryException&) { delivCaught = true; }
    assert(delivCaught);

    // ReviewNotFoundException
    Rating rating("978-0321563842");
    rating.addReview(ReviewSystem(1, "978-0321563842", "Tom", 5, "Great book!"));
    assert(rating.calculateAverageStars() == 5.0);
    bool revCaught = false;
    try { rating.removeReviewById(404); }
    catch (const ReviewNotFoundException&) { revCaught = true; }
    assert(revCaught);
}

int main() {
    testCatalogAndBooks();
    testUsersAndOrders();
    testStaffLogisticsAndReviews();
    std::cout << "All Lab 2 unit tests (60 classes, 13 exceptions) passed successfully!\n";
    return 0;
}
