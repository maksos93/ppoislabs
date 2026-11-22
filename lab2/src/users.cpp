/**
 * @file users.cpp
 * @brief Implementation of Users, Readers, Membership, and Loyalty classes.
 */

#include "users.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <utility>

NotificationPreferences::NotificationPreferences(bool email, bool sms, std::string freq)
    : emailEnabled(email), smsEnabled(sms), digestFrequency(std::move(freq)) {}

void NotificationPreferences::enableAllChannels() {
    emailEnabled = true;
    smsEnabled = true;
}

void NotificationPreferences::disableMarketing() {
    digestFrequency = "None";
}

bool NotificationPreferences::canSendChannel(const std::string& channel) const {
    if (channel == "email") return emailEnabled;
    if (channel == "sms") return smsEnabled;
    return false;
}

User::User(int id, std::string name, std::string email, Address addr)
    : userId(id), fullName(std::move(name)), email(std::move(email)), address(std::move(addr)) {}

bool User::validateEmailFormat() const {
    return email.find('@') != std::string::npos && email.find('.') != std::string::npos;
}

void User::relocate(const Address& newAddress) {
    address = newAddress;
}

void User::applyNotificationPreferences(const NotificationPreferences& prefs) {
    preferences = prefs;
}

std::string User::getAccessTier() const {
    return "StandardUser";
}

MembershipCard::MembershipCard(std::string number, int limit)
    : cardNumber(std::move(number)), borrowLimit(limit), activeLoans(0), suspended(false) {}

bool MembershipCard::canBorrow() const {
    return !suspended && activeLoans < borrowLimit;
}

void MembershipCard::recordLoan() {
    if (!canBorrow()) {
        throw UnauthorizedAccessException("Suspended or limit reached on card " + cardNumber);
    }
    activeLoans++;
}

void MembershipCard::recordReturn() {
    if (activeLoans > 0) activeLoans--;
}

void MembershipCard::setSuspended(bool state) {
    suspended = state;
}

Reader::Reader(const User& base, MembershipCard card)
    : User(base), card(std::move(card)), unpaidFines(0.0) {}

void Reader::borrowBook(Book& book) {
    if (unpaidFines > 50.0) {
        throw UnauthorizedAccessException("Unpaid fines exceed threshold");
    }
    card.recordLoan();
    book.reserveCopies(1);
    borrowedIsbns.push_back(book.getIsbn());
}

void Reader::returnBook(Book& book) {
    auto it = std::find(borrowedIsbns.begin(), borrowedIsbns.end(), book.getIsbn());
    if (it != borrowedIsbns.end()) {
        borrowedIsbns.erase(it);
        card.recordReturn();
        book.restock(1);
    }
}

void Reader::addFine(double amount) {
    if (amount > 0.0) unpaidFines += amount;
}

void Reader::payFine(double amount) {
    if (amount <= 0.0) {
        throw InvalidPaymentException("Fine payment must be positive.");
    }
    unpaidFines = (amount >= unpaidFines) ? 0.0 : (unpaidFines - amount);
}

std::string Reader::getAccessTier() const {
    return "Reader";
}

Student::Student(const Reader& base, std::string uni, std::string studId, int gradYear)
    : Reader(base), universityName(std::move(uni)), studentIdNumber(std::move(studId)), graduationYear(gradYear) {}

bool Student::isEligibleForAcademicDiscount(int currentYear) const {
    return currentYear <= graduationYear;
}

double Student::calculateAcademicFeeDiscount(double baseFee) const {
    return baseFee * 0.5;
}

std::string Student::getAccessTier() const {
    return "StudentReader";
}

Professor::Professor(const Reader& base, std::string dept, std::string rank, int days)
    : Reader(base), department(std::move(dept)), academicRank(std::move(rank)), extendedLoanDays(days) {}

int Professor::computeDueDays(bool isReferenceMaterial) const {
    return isReferenceMaterial ? 14 : extendedLoanDays;
}

bool Professor::canRequestInterlibraryLoan() const {
    return true;
}

std::string Professor::getAccessTier() const {
    return "ProfessorReader";
}

LoyaltyAccount::LoyaltyAccount(int points, std::string tier, double spent)
    : pointsBalance(points), tierLevel(std::move(tier)), totalSpent(spent) {}

void LoyaltyAccount::accruePurchase(double amount) {
    if (amount > 0.0) {
        totalSpent += amount;
        pointsBalance += static_cast<int>(amount * 10);
        recalculateTier();
    }
}

double LoyaltyAccount::redeemPoints(int points) {
    if (points <= 0 || points > pointsBalance) {
        throw InsufficientFundsException(points, pointsBalance);
    }
    pointsBalance -= points;
    return points * 0.01;
}

void LoyaltyAccount::recalculateTier() {
    if (totalSpent >= 1000.0) tierLevel = "Platinum";
    else if (totalSpent >= 300.0) tierLevel = "Gold";
    else tierLevel = "Silver";
}

VIPCustomer::VIPCustomer(const User& base, double discountRate, std::string manager)
    : User(base), personalDiscountRate(discountRate), dedicatedManagerName(std::move(manager)) {}

double VIPCustomer::applyVipPricing(double originalPrice) const {
    return originalPrice * (1.0 - personalDiscountRate);
}

void VIPCustomer::rewardBonusPoints(int points) {
    loyalty.accruePurchase(points / 10.0);
}

bool VIPCustomer::qualifiesForFreeExpress() const {
    return personalDiscountRate >= 0.10;
}

std::string VIPCustomer::getAccessTier() const {
    return "VIPCustomer";
}

Guest::Guest(std::string token)
    : sessionToken(std::move(token)), viewedPages(0), acceptedCookies(false) {}

void Guest::trackPageView() {
    viewedPages++;
}

bool Guest::canBrowseCatalog() const {
    return !sessionToken.empty();
}

UserRegistry::UserRegistry() : nextId(1) {}

User UserRegistry::registerUser(const std::string& name, const std::string& email, const Address& addr) {
    if (name.empty() || email.find('@') == std::string::npos) {
        throw InvalidUserException(email);
    }
    if (existsByEmail(email)) {
        throw InvalidUserException("Email already registered: " + email);
    }
    User created(nextId++, name, email, addr);
    users.push_back(created);
    return created;
}

bool UserRegistry::existsByEmail(const std::string& email) const {
    for (const auto& u : users) {
        if (u.getEmail() == email) return true;
    }
    return false;
}

User UserRegistry::findById(int id) const {
    for (const auto& u : users) {
        if (u.getUserId() == id) return u;
    }
    throw InvalidUserException("User ID not found: " + std::to_string(id));
}

size_t UserRegistry::countActiveUsers() const {
    return users.size();
}
