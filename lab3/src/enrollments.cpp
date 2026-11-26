/**
 * @file enrollments.cpp
 * @brief Implementation of Enrollments, Coupons, Receipts, and Billing.
 */

#include "enrollments.hpp"
#include "exceptions.hpp"
#include <utility>

EnrollmentItem::EnrollmentItem(const Course& course, int seats)
    : course(course), seatCount(seats), unitFee(course.getTuitionFee()) {
    if (seats <= 0) {
        throw InvalidCourseDataException("Seat count must be positive.");
    }
}

double EnrollmentItem::calculateSubtotal() const {
    return unitFee * seatCount;
}

void EnrollmentItem::updateSeatCount(int newSeats) {
    if (newSeats <= 0) {
        throw InvalidCourseDataException("Seat count must be positive.");
    }
    seatCount = newSeats;
}

bool EnrollmentItem::matchesCourseCode(const std::string& code) const {
    return course.getCode() == code;
}

Coupon::Coupon(std::string code, double pct, double minSpend)
    : code(std::move(code)), discountPct(pct), minSpendAmount(minSpend) {
    if (pct < 0.0 || pct > 100.0) {
        throw InvalidCouponException(pct);
    }
}

bool Coupon::isApplicable(double subtotal) const {
    return subtotal >= minSpendAmount && discountPct > 0.0;
}

double Coupon::computeSavings(double subtotal) const {
    if (!isApplicable(subtotal)) return 0.0;
    return subtotal * (discountPct / 100.0);
}

PromoCampaign::PromoCampaign(std::string title, Coupon coupon, bool active, int uses)
    : title(std::move(title)), coupon(std::move(coupon)), active(active), remainingUses(uses) {}

double PromoCampaign::applyCampaign(double amount) {
    if (!active || remainingUses <= 0) return amount;
    double savings = coupon.computeSavings(amount);
    if (savings > 0.0) {
        remainingUses--;
    }
    return amount - savings;
}

void PromoCampaign::deactivate() {
    active = false;
}

bool PromoCampaign::hasQuotaLeft() const {
    return active && remainingUses > 0;
}

Enrollment::Enrollment(int id, const Learner& learner)
    : enrollmentId(id), learner(learner), status("Pending"), discountApplied(0.0) {}

void Enrollment::addCourseItem(const Course& course, int seats) {
    if (course.getAvailableSeats() < seats) {
        throw SeatLimitExceededException(course.getTitle());
    }
    items.emplace_back(course, seats);
}

void Enrollment::applyCoupon(const Coupon& coupon) {
    double raw = 0.0;
    for (const auto& item : items) raw += item.calculateSubtotal();
    if (!coupon.isApplicable(raw)) {
        throw InvalidCouponException(raw);
    }
    discountApplied = coupon.computeSavings(raw);
}

double Enrollment::calculateNetTotal() const {
    double raw = 0.0;
    for (const auto& item : items) raw += item.calculateSubtotal();
    return (raw > discountApplied) ? (raw - discountApplied) : 0.0;
}

void Enrollment::confirmEnrollment() {
    if (items.empty()) {
        throw EnrollmentNotFoundException(enrollmentId);
    }
    status = "Confirmed";
}

void Enrollment::cancelEnrollment() {
    status = "Cancelled";
}

Receipt::Receipt(std::string number, const Enrollment& enrollment, double tax)
    : receiptNumber(std::move(number)), enrollment(enrollment), taxRate(tax), settled(false) {}

double Receipt::calculateTax() const {
    return enrollment.calculateNetTotal() * taxRate;
}

double Receipt::calculateGrandTotal() const {
    return enrollment.calculateNetTotal() + calculateTax();
}

void Receipt::markSettled() {
    settled = true;
}

BillingTransaction::BillingTransaction(std::string id, double amount, std::string currency)
    : txId(std::move(id)), amount(amount), currency(std::move(currency)), paid(false) {}

bool BillingTransaction::executePayment(Receipt& receipt) {
    if (amount < receipt.calculateGrandTotal()) {
        throw InsufficientBalanceException(receipt.calculateGrandTotal(), amount);
    }
    paid = true;
    receipt.markSettled();
    return true;
}

void BillingTransaction::reverseTransaction() {
    paid = false;
}

CardBilling::CardBilling(std::string id, double amount, std::string pan, std::string exp, double limit)
    : BillingTransaction(std::move(id), amount, "USD"), maskedPan(std::move(pan)),
      expiry(std::move(exp)), availableLimit(limit) {}

bool CardBilling::checkExpiry(int currentYear) const {
    return currentYear <= 2030 && !expiry.empty();
}

bool CardBilling::executePayment(Receipt& receipt) {
    double due = receipt.calculateGrandTotal();
    if (availableLimit < due) {
        throw InsufficientBalanceException(due, availableLimit);
    }
    availableLimit -= due;
    paid = true;
    receipt.markSettled();
    return true;
}

WalletBilling::WalletBilling(std::string id, double amount, double balance, double bonus)
    : BillingTransaction(std::move(id), amount, "USD"), walletBalance(balance), bonusCredit(bonus) {}

double WalletBilling::getTotalFunds() const {
    return walletBalance + bonusCredit;
}

bool WalletBilling::executePayment(Receipt& receipt) {
    double due = receipt.calculateGrandTotal();
    double totalFunds = getTotalFunds();
    if (totalFunds < due) {
        throw InsufficientBalanceException(due, totalFunds);
    }
    walletBalance = (walletBalance >= due) ? (walletBalance - due) : 0.0;
    paid = true;
    receipt.markSettled();
    return true;
}

BankTransferBilling::BankTransferBilling(std::string id, double amount, std::string email, bool verified)
    : BillingTransaction(std::move(id), amount, "USD"), senderEmail(std::move(email)), referenceVerified(verified) {}

bool BankTransferBilling::verifyTransfer() const {
    return referenceVerified && senderEmail.find('@') != std::string::npos;
}

bool BankTransferBilling::executePayment(Receipt& receipt) {
    if (!verifyTransfer()) {
        throw InvalidBillingException("Bank transfer not verified for " + senderEmail);
    }
    return BillingTransaction::executePayment(receipt);
}

BillingManager::BillingManager() : totalCollected(0.0) {}

void BillingManager::recordReceipt(const Receipt& receipt) {
    receipts.push_back(receipt);
}

bool BillingManager::settleEnrollment(Enrollment& enrollment, Receipt& receipt, BillingTransaction& tx) {
    if (tx.executePayment(receipt)) {
        enrollment.confirmEnrollment();
        totalCollected += receipt.calculateGrandTotal();
        receipts.push_back(receipt);
        return true;
    }
    return false;
}

size_t BillingManager::countSettledReceipts() const {
    size_t count = 0;
    for (const auto& r : receipts) {
        if (r.isSettled()) count++;
    }
    return count;
}
