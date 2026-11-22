/**
 * @file users.hpp
 * @brief Users, Readers, Membership, and Loyalty management definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "book.hpp"

/**
 * @class NotificationPreferences
 * @brief Stores communication channel preferences for a user account.
 */
class NotificationPreferences {
private:
    bool emailEnabled;           ///< Whether email notifications are enabled.
    bool smsEnabled;             ///< Whether SMS alerts are enabled.
    std::string digestFrequency; ///< Summary digest schedule (e.g., Weekly, None).
public:
    /**
     * @brief Constructs NotificationPreferences with channel flags and digest frequency.
     * @param email Email enabled flag.
     * @param sms SMS enabled flag.
     * @param freq Digest frequency string.
     */
    NotificationPreferences(bool email = true, bool sms = false, std::string freq = "Weekly");

    /**
     * @brief Enables both email and SMS notification channels.
     */
    void enableAllChannels();

    /**
     * @brief Disables marketing digests by setting frequency to "None".
     */
    void disableMarketing();

    /**
     * @brief Checks whether notifications can be sent over a specific channel.
     * @param channel Channel name ("email" or "sms").
     * @return True if the requested channel is active.
     */
    bool canSendChannel(const std::string& channel) const;
};

/**
 * @class User
 * @brief Base class representing a registered system user with contact info and preferences.
 */
class User {
protected:
    int userId;                          ///< Unique numeric user ID.
    std::string fullName;                ///< User's full name.
    std::string email;                   ///< User's email address.
    Address address;                     ///< User's postal address.
    NotificationPreferences preferences; ///< User's notification settings.
public:
    /**
     * @brief Constructs a User account.
     * @param id Unique user ID.
     * @param name Full name.
     * @param email Email address.
     * @param addr Postal Address.
     */
    User(int id = 0, std::string name = "", std::string email = "", Address addr = Address());

    /**
     * @brief Virtual destructor for user hierarchy.
     */
    virtual ~User() = default;

    int getUserId() const { return userId; }
    std::string getFullName() const { return fullName; }
    std::string getEmail() const { return email; }

    /**
     * @brief Validates basic email syntax ('@' and '.' presence).
     * @return True if email format appears valid.
     */
    bool validateEmailFormat() const;

    /**
     * @brief Updates the user's postal address.
     * @param newAddress New Address instance.
     */
    void relocate(const Address& newAddress);

    /**
     * @brief Applies updated notification preferences to the user profile.
     * @param prefs New NotificationPreferences object.
     */
    void applyNotificationPreferences(const NotificationPreferences& prefs);

    /**
     * @brief Returns the user's access tier label.
     * @return Access tier string.
     */
    virtual std::string getAccessTier() const;
};

/**
 * @class MembershipCard
 * @brief Represents a library reader card governing active loans and suspension state.
 */
class MembershipCard {
private:
    std::string cardNumber; ///< Unique library card number.
    int borrowLimit;        ///< Maximum concurrent borrowed books allowed.
    int activeLoans;        ///< Current count of active loans.
    bool suspended;         ///< Suspension status flag.
public:
    /**
     * @brief Constructs a MembershipCard.
     * @param number Card number string.
     * @param limit Maximum simultaneous loans.
     */
    MembershipCard(std::string number = "CARD-000", int limit = 5);

    /**
     * @brief Checks whether the card is active and below its borrow limit.
     * @return True if borrowing is permitted.
     */
    bool canBorrow() const;

    /**
     * @brief Increments active loan count if borrowing is permitted.
     * @throws UnauthorizedAccessException if card is suspended or limit is reached.
     */
    void recordLoan();

    /**
     * @brief Decrements active loan count upon book return.
     */
    void recordReturn();

    /**
     * @brief Sets the suspension status of the membership card.
     * @param state True to suspend, false to activate.
     */
    void setSuspended(bool state);
};

/**
 * @class Reader
 * @brief Represents a library patron capable of borrowing and returning books and paying fines.
 */
class Reader : public User {
protected:
    MembershipCard card;                    ///< Reader's library membership card.
    std::vector<std::string> borrowedIsbns; ///< List of currently borrowed book ISBNs.
    double unpaidFines;                     ///< Accumulated overdue fine balance.
public:
    /**
     * @brief Constructs a Reader from a base User and MembershipCard.
     * @param base Base User account.
     * @param card Library MembershipCard.
     */
    Reader(const User& base, MembershipCard card = MembershipCard());

    /**
     * @brief Borrows a book, reserving one copy and recording the loan on the card.
     * @param book Book to borrow.
     * @throws UnauthorizedAccessException if unpaid fines exceed threshold or card cannot borrow.
     * @throws OutOfStockException if no copies are available.
     */
    void borrowBook(Book& book);

    /**
     * @brief Returns a previously borrowed book and restores stock quantity.
     * @param book Book being returned.
     */
    void returnBook(Book& book);

    /**
     * @brief Adds an overdue or damage fine to the reader's balance.
     * @param amount Fine amount to add.
     */
    void addFine(double amount);

    /**
     * @brief Pays down the reader's unpaid fine balance.
     * @param amount Payment amount (must be > 0).
     * @throws InvalidPaymentException if amount <= 0.
     */
    void payFine(double amount);

    /**
     * @brief Returns the reader access tier label.
     * @return "Reader" tier string.
     */
    std::string getAccessTier() const override;
};

/**
 * @class Student
 * @brief Specialized Reader representing an enrolled university student eligible for fee discounts.
 */
class Student : public Reader {
private:
    std::string universityName;  ///< Affiliated university name.
    std::string studentIdNumber; ///< Student ID card code.
    int graduationYear;          ///< Expected graduation year.
public:
    /**
     * @brief Constructs a Student reader profile.
     * @param base Base Reader profile.
     * @param uni University name.
     * @param studId Student ID number.
     * @param gradYear Expected graduation year.
     */
    Student(const Reader& base, std::string uni = "State University", std::string studId = "S-100", int gradYear = 2027);

    /**
     * @brief Checks whether student status is still active in the given year.
     * @param currentYear Current calendar year.
     * @return True if currentYear <= graduationYear.
     */
    bool isEligibleForAcademicDiscount(int currentYear) const;

    /**
     * @brief Calculates the discounted academic fee (50% reduction).
     * @param baseFee Standard fee amount.
     * @return Discounted fee.
     */
    double calculateAcademicFeeDiscount(double baseFee) const;

    /**
     * @brief Returns the student access tier label.
     * @return "StudentReader" string.
     */
    std::string getAccessTier() const override;
};

/**
 * @class Professor
 * @brief Specialized Reader representing faculty members with extended loan periods.
 */
class Professor : public Reader {
private:
    std::string department;   ///< Academic department.
    std::string academicRank; ///< Faculty rank (e.g., Associate, Full).
    int extendedLoanDays;     ///< Standard extended loan duration in days.
public:
    /**
     * @brief Constructs a Professor reader profile.
     * @param base Base Reader profile.
     * @param dept Department name.
     * @param rank Academic rank.
     * @param days Extended loan period in days.
     */
    Professor(const Reader& base, std::string dept = "Computer Science", std::string rank = "Associate", int days = 60);

    /**
     * @brief Computes loan duration in days depending on whether the item is reference material.
     * @param isReferenceMaterial True if item is restricted reference material.
     * @return Allowed loan duration in days.
     */
    int computeDueDays(bool isReferenceMaterial) const;

    /**
     * @brief Checks whether the professor is permitted to initiate interlibrary loans.
     * @return Always true for faculty accounts.
     */
    bool canRequestInterlibraryLoan() const;

    /**
     * @brief Returns the professor access tier label.
     * @return "ProfessorReader" string.
     */
    std::string getAccessTier() const override;
};

/**
 * @class LoyaltyAccount
 * @brief Tracks reward points, tier status, and cumulative spend for bookstore customers.
 */
class LoyaltyAccount {
private:
    int pointsBalance;     ///< Current redeemable loyalty points.
    std::string tierLevel; ///< Membership tier (Silver, Gold, Platinum).
    double totalSpent;     ///< Cumulative purchase spend.
public:
    /**
     * @brief Constructs a LoyaltyAccount.
     * @param points Initial points balance.
     * @param tier Initial tier label.
     * @param spent Initial cumulative spend.
     */
    LoyaltyAccount(int points = 0, std::string tier = "Silver", double spent = 0.0);

    /**
     * @brief Credits points for a purchase and updates loyalty tier.
     * @param amount Purchase total in currency units.
     */
    void accruePurchase(double amount);

    /**
     * @brief Redeems loyalty points for monetary credit.
     * @param points Number of points to redeem.
     * @return Monetary discount value ($0.01 per point).
     * @throws InsufficientFundsException if points <= 0 or points > pointsBalance.
     */
    double redeemPoints(int points);

    /**
     * @brief Recalculates tier level based on cumulative spend thresholds.
     */
    void recalculateTier();

    int getPointsBalance() const { return pointsBalance; }
};

/**
 * @class VIPCustomer
 * @brief Premium bookstore customer with personal discount rate, loyalty account, and manager.
 */
class VIPCustomer : public User {
private:
    LoyaltyAccount loyalty;           ///< Attached loyalty rewards account.
    double personalDiscountRate;      ///< Personal discount fraction (e.g., 0.15 for 15%).
    std::string dedicatedManagerName; ///< Assigned account manager name.
public:
    /**
     * @brief Constructs a VIPCustomer profile.
     * @param base Base User account.
     * @param discountRate Personal discount fraction.
     * @param manager Dedicated manager name.
     */
    VIPCustomer(const User& base, double discountRate = 0.15, std::string manager = "Anna");

    /**
     * @brief Applies the customer's personal VIP discount to a price.
     * @param originalPrice Base item or order price.
     * @return Discounted VIP price.
     */
    double applyVipPricing(double originalPrice) const;

    /**
     * @brief Awards bonus loyalty points to the VIP customer's account.
     * @param points Bonus points to credit.
     */
    void rewardBonusPoints(int points);

    /**
     * @brief Checks whether the customer qualifies for complimentary express delivery.
     * @return True if personalDiscountRate >= 0.10.
     */
    bool qualifiesForFreeExpress() const;

    /**
     * @brief Returns the VIP access tier label.
     * @return "VIPCustomer" string.
     */
    std::string getAccessTier() const override;
};

/**
 * @class Guest
 * @brief Represents an unauthenticated visitor browsing the online catalog.
 */
class Guest {
private:
    std::string sessionToken; ///< Temporary browser session identifier.
    int viewedPages;          ///< Number of pages viewed during session.
    bool acceptedCookies;     ///< Cookie consent flag.
public:
    /**
     * @brief Constructs a Guest session.
     * @param token Temporary session token.
     */
    Guest(std::string token = "GUEST-TMP");

    /**
     * @brief Increments the counter of viewed catalog pages.
     */
    void trackPageView();

    /**
     * @brief Verifies whether the guest has an active session token to browse.
     * @return True if sessionToken is non-empty.
     */
    bool canBrowseCatalog() const;
};

/**
 * @class UserRegistry
 * @brief Manages registration, uniqueness validation, and lookup of system users.
 */
class UserRegistry {
private:
    std::vector<User> users; ///< Registered user accounts.
    int nextId;              ///< Auto-incrementing user ID counter.
public:
    UserRegistry();

    /**
     * @brief Registers a new User after validating name and unique email.
     * @param name User's full name.
     * @param email User's unique email address.
     * @param addr User's Address.
     * @return Newly created User instance.
     * @throws InvalidUserException if name/email is invalid or email already exists.
     */
    User registerUser(const std::string& name, const std::string& email, const Address& addr);

    /**
     * @brief Checks whether an email address is already registered.
     * @param email Email string to check.
     * @return True if email exists in registry.
     */
    bool existsByEmail(const std::string& email) const;

    /**
     * @brief Finds a registered User by numeric ID.
     * @param id Target user ID.
     * @return Matching User object.
     * @throws InvalidUserException if no user has the given ID.
     */
    User findById(int id) const;

    /**
     * @brief Returns total number of registered users.
     * @return User count.
     */
    size_t countActiveUsers() const;
};