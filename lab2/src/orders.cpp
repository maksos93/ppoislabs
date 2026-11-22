/**
 * @file orders.cpp
 * @brief Implementation of Orders, Invoices, Payments, and Promotions.
 */

#include "orders.hpp"
#include "exceptions.hpp"
#include <utility>

OrderItem::OrderItem(const Book& book, int qty)
    : book(book), quantity(qty), unitPrice(book.getPrice()) {
    if (qty <= 0) {
        throw InvalidBookDataException("Order item quantity must be positive.");
    }
}

double OrderItem::calculateSubtotal() const {
    return unitPrice * quantity;
}

void OrderItem::changeQuantity(int newQty) {
    if (newQty <= 0) {
        throw InvalidBookDataException("Quantity must be positive.");
    }
    quantity = newQty;
}

bool OrderItem::matchesIsbn(const std::string& isbn) const {
    return book.getIsbn() == isbn;
}

Discount::Discount(std::string code, double pct, double minAmount)
    : code(std::move(code)), percentage(pct), minOrderAmount(minAmount) {
    if (pct < 0.0 || pct > 100.0) {
        throw InvalidDiscountException(pct);
    }
}

bool Discount::isApplicable(double orderTotal) const {
    return orderTotal >= minOrderAmount && percentage > 0.0;
}

double Discount::computeSavings(double orderTotal) const {
    if (!isApplicable(orderTotal)) return 0.0;
    return orderTotal * (percentage / 100.0);
}

Promotion::Promotion(std::string name, Discount disc, bool active, int limit)
    : campaignName(std::move(name)), attachedDiscount(std::move(disc)), active(active), usageLimit(limit) {}

double Promotion::applyPromotion(double amount) {
    if (!active || usageLimit <= 0) return amount;
    double savings = attachedDiscount.computeSavings(amount);
    if (savings > 0.0) {
        usageLimit--;
    }
    return amount - savings;
}

void Promotion::deactivateCampaign() {
    active = false;
}

bool Promotion::hasRemainingQuota() const {
    return active && usageLimit > 0;
}

Order::Order(int id, const User& customer)
    : orderId(id), customer(customer), status("Draft"), discountAmount(0.0) {}

void Order::addItem(const Book& book, int quantity) {
    if (book.getStockQuantity() < quantity) {
        throw OutOfStockException(book.getTitle());
    }
    items.emplace_back(book, quantity);
}

void Order::applyDiscount(const Discount& discount) {
    double raw = 0.0;
    for (const auto& item : items) raw += item.calculateSubtotal();
    if (!discount.isApplicable(raw)) {
        throw InvalidDiscountException(raw);
    }
    discountAmount = discount.computeSavings(raw);
}

double Order::calculateTotal() const {
    double raw = 0.0;
    for (const auto& item : items) raw += item.calculateSubtotal();
    return (raw > discountAmount) ? (raw - discountAmount) : 0.0;
}

void Order::confirmOrder() {
    if (items.empty()) {
        throw OrderNotFoundException(orderId);
    }
    status = "Confirmed";
}

void Order::cancelOrder() {
    status = "Cancelled";
}

Invoice::Invoice(std::string number, const Order& order, double tax)
    : invoiceNumber(std::move(number)), order(order), taxRate(tax), paid(false) {}

double Invoice::calculateTaxAmount() const {
    return order.calculateTotal() * taxRate;
}

double Invoice::calculateGrandTotal() const {
    return order.calculateTotal() + calculateTaxAmount();
}

void Invoice::markAsPaid() {
    paid = true;
}

Payment::Payment(std::string txId, double amount, std::string currency)
    : transactionId(std::move(txId)), amount(amount), currency(std::move(currency)), completed(false) {}

bool Payment::processPayment(Invoice& invoice) {
    if (amount < invoice.calculateGrandTotal()) {
        throw InsufficientFundsException(invoice.calculateGrandTotal(), amount);
    }
    completed = true;
    invoice.markAsPaid();
    return true;
}

void Payment::refundPayment() {
    completed = false;
}

CreditCardPayment::CreditCardPayment(std::string txId, double amount, std::string card, std::string exp, double credit)
    : Payment(std::move(txId), amount, "USD"), maskedCardNumber(std::move(card)),
      expiryDate(std::move(exp)), availableCredit(credit) {}

bool CreditCardPayment::verifyCardExpiry(int currentYear) const {
    return currentYear <= 2030 && !expiryDate.empty();
}

bool CreditCardPayment::processPayment(Invoice& invoice) {
    double due = invoice.calculateGrandTotal();
    if (availableCredit < due) {
        throw InsufficientFundsException(due, availableCredit);
    }
    availableCredit -= due;
    completed = true;
    invoice.markAsPaid();
    return true;
}

CashPayment::CashPayment(std::string txId, double amount, double tendered)
    : Payment(std::move(txId), amount, "USD"), cashTendered(tendered), changeReturned(0.0) {}

double CashPayment::calculateChange() const {
    return (cashTendered >= amount) ? (cashTendered - amount) : 0.0;
}

bool CashPayment::processPayment(Invoice& invoice) {
    double due = invoice.calculateGrandTotal();
    if (cashTendered < due) {
        throw InsufficientFundsException(due, cashTendered);
    }
    changeReturned = cashTendered - due;
    completed = true;
    invoice.markAsPaid();
    return true;
}

PayPalPayment::PayPalPayment(std::string txId, double amount, std::string email, bool verified)
    : Payment(std::move(txId), amount, "USD"), payerEmail(std::move(email)), twoFactorVerified(verified) {}

bool PayPalPayment::verifyAccountSecurity() const {
    return twoFactorVerified && payerEmail.find('@') != std::string::npos;
}

bool PayPalPayment::processPayment(Invoice& invoice) {
    if (!verifyAccountSecurity()) {
        throw InvalidPaymentException("PayPal 2FA verification failed for " + payerEmail);
    }
    return Payment::processPayment(invoice);
}

TransactionManager::TransactionManager() : totalRevenue(0.0) {}

void TransactionManager::registerInvoice(const Invoice& invoice) {
    invoices.push_back(invoice);
}

bool TransactionManager::settleOrderPayment(Order& order, Invoice& invoice, Payment& payment) {
    if (payment.processPayment(invoice)) {
        order.confirmOrder();
        totalRevenue += invoice.calculateGrandTotal();
        invoices.push_back(invoice);
        return true;
    }
    return false;
}

size_t TransactionManager::countPaidInvoices() const {
    size_t count = 0;
    for (const auto& inv : invoices) {
        if (inv.isPaid()) count++;
    }
    return count;
}
