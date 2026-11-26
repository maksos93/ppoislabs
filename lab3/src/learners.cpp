/**
 * @file learners.cpp
 * @brief Implementation of Learners, Students, Study Passes, Subscriptions, and Reward Wallet.
 */

#include "learners.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <utility>

AlertSettings::AlertSettings(bool email, bool push, std::string cadence)
    : emailAlerts(email), pushAlerts(push), reminderCadence(std::move(cadence)) {}

void AlertSettings::enableAllAlerts() {
    emailAlerts = true;
    pushAlerts = true;
}

void AlertSettings::muteDigests() {
    reminderCadence = "None";
}

bool AlertSettings::isChannelActive(const std::string& channel) const {
    if (channel == "email") return emailAlerts;
    if (channel == "push") return pushAlerts;
    return false;
}

Learner::Learner(int id, std::string name, std::string email, ContactAddress addr)
    : learnerId(id), fullName(std::move(name)), email(std::move(email)), address(std::move(addr)) {}

bool Learner::validateEmail() const {
    return email.find('@') != std::string::npos && email.find('.') != std::string::npos;
}

void Learner::updateAddress(const ContactAddress& newAddress) {
    address = newAddress;
}

void Learner::applyAlertSettings(const AlertSettings& settings) {
    alerts = settings;
}

std::string Learner::getAccountTier() const {
    return "StandardLearner";
}

StudyPass::StudyPass(std::string code, int maxCourses)
    : passCode(std::move(code)), maxActiveCourses(maxCourses), activeCourses(0), frozen(false) {}

bool StudyPass::canEnrollMore() const {
    return !frozen && activeCourses < maxActiveCourses;
}

void StudyPass::activateCourseSlot() {
    if (!canEnrollMore()) {
        throw AccessDeniedException("StudyPass frozen or max courses reached: " + passCode);
    }
    activeCourses++;
}

void StudyPass::releaseCourseSlot() {
    if (activeCourses > 0) activeCourses--;
}

void StudyPass::setFrozen(bool state) {
    frozen = state;
}

StudentLearner::StudentLearner(const Learner& base, StudyPass pass)
    : Learner(base), pass(std::move(pass)), overdueBalance(0.0) {}

void StudentLearner::enrollCourse(Course& course) {
    if (overdueBalance > 50.0) {
        throw AccessDeniedException("Overdue balance exceeds limit");
    }
    pass.activateCourseSlot();
    course.reserveSeats(1);
    enrolledCodes.push_back(course.getCode());
}

void StudentLearner::dropCourse(Course& course) {
    auto it = std::find(enrolledCodes.begin(), enrolledCodes.end(), course.getCode());
    if (it != enrolledCodes.end()) {
        enrolledCodes.erase(it);
        pass.releaseCourseSlot();
        course.expandSeats(1);
    }
}

void StudentLearner::chargeFee(double amount) {
    if (amount > 0.0) overdueBalance += amount;
}

void StudentLearner::settleBalance(double amount) {
    if (amount <= 0.0) {
        throw InvalidBillingException("Settlement amount must be positive.");
    }
    overdueBalance = (amount >= overdueBalance) ? 0.0 : (overdueBalance - amount);
}

std::string StudentLearner::getAccountTier() const {
    return "StudentLearner";
}

UniversityStudent::UniversityStudent(const StudentLearner& base, std::string uni, std::string cardId, int gradYear)
    : StudentLearner(base), universityName(std::move(uni)), studentCardId(std::move(cardId)), graduationYear(gradYear) {}

bool UniversityStudent::isEligibleForStudentDiscount(int currentYear) const {
    return currentYear <= graduationYear;
}

double UniversityStudent::calculateStudentFee(double baseFee) const {
    return baseFee * 0.5;
}

std::string UniversityStudent::getAccountTier() const {
    return "UniversityStudent";
}

CorporateLearner::CorporateLearner(const StudentLearner& base, std::string company, std::string dept, int days)
    : StudentLearner(base), companyName(std::move(company)), department(std::move(dept)), accessWindowDays(days) {}

int CorporateLearner::computeCompletionDays(bool isSelfPaced) const {
    return isSelfPaced ? accessWindowDays : 30;
}

bool CorporateLearner::canTransferTeamSeat() const {
    return true;
}

std::string CorporateLearner::getAccountTier() const {
    return "CorporateLearner";
}

RewardWallet::RewardWallet(int pts, std::string tier, double spend)
    : points(pts), tierName(std::move(tier)), totalSpend(spend) {}

void RewardWallet::creditPurchase(double amount) {
    if (amount > 0.0) {
        totalSpend += amount;
        points += static_cast<int>(amount * 10);
        refreshTier();
    }
}

double RewardWallet::redeemPoints(int pts) {
    if (pts <= 0 || pts > points) {
        throw InsufficientBalanceException(pts, points);
    }
    points -= pts;
    return pts * 0.01;
}

void RewardWallet::refreshTier() {
    if (totalSpend >= 1000.0) tierName = "Platinum";
    else if (totalSpend >= 300.0) tierName = "Gold";
    else tierName = "Bronze";
}

ProSubscriber::ProSubscriber(const Learner& base, double rate, std::string advisor)
    : Learner(base), discountRate(rate), advisorName(std::move(advisor)) {}

double ProSubscriber::applyProPrice(double originalPrice) const {
    return originalPrice * (1.0 - discountRate);
}

void ProSubscriber::grantBonusPoints(int bonusPoints) {
    wallet.creditPurchase(bonusPoints / 10.0);
}

bool ProSubscriber::qualifiesForFreeCodeReview() const {
    return discountRate >= 0.10;
}

std::string ProSubscriber::getAccountTier() const {
    return "ProSubscriber";
}

Visitor::Visitor(std::string sid)
    : sessionId(std::move(sid)), previewsViewed(0), agreedToTerms(false) {}

void Visitor::recordPreviewView() {
    previewsViewed++;
}

bool Visitor::canViewFreePreviews() const {
    return !sessionId.empty();
}

LearnerRegistry::LearnerRegistry() : nextId(1) {}

Learner LearnerRegistry::registerLearner(const std::string& name, const std::string& email, const ContactAddress& addr) {
    if (name.empty() || email.find('@') == std::string::npos) {
        throw InvalidAccountException(email);
    }
    if (existsByEmail(email)) {
        throw InvalidAccountException("Email already registered: " + email);
    }
    Learner created(nextId++, name, email, addr);
    learners.push_back(created);
    return created;
}

bool LearnerRegistry::existsByEmail(const std::string& email) const {
    for (const auto& l : learners) {
        if (l.getEmail() == email) return true;
    }
    return false;
}

Learner LearnerRegistry::findById(int id) const {
    for (const auto& l : learners) {
        if (l.getLearnerId() == id) return l;
    }
    throw InvalidAccountException("Learner ID not found: " + std::to_string(id));
}
