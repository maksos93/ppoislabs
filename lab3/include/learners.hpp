/**
 * @file learners.hpp
 * @brief Learners, Students, Study Passes, Subscriptions, and Reward Wallet definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "courses.hpp"

/**
 * @class AlertSettings
 * @brief Stores notification preferences for deadlines, reminders, and digests.
 */
class AlertSettings {
private:
    bool emailAlerts;          ///< Email reminders flag.
    bool pushAlerts;           ///< Push/SMS reminders flag.
    std::string reminderCadence;///< Reminder schedule (e.g., Daily, Weekly, None).
public:
    /**
     * @brief Constructs AlertSettings.
     * @param email Email alerts flag.
     * @param push Push alerts flag.
     * @param cadence Reminder cadence string.
     */
    AlertSettings(bool email = true, bool push = false, std::string cadence = "Weekly");

    /**
     * @brief Enables all notification channels.
     */
    void enableAllAlerts();

    /**
     * @brief Disables promotional/digest reminders.
     */
    void muteDigests();

    /**
     * @brief Checks whether a specific channel ("email" or "push") is active.
     * @param channel Channel name.
     * @return True if active.
     */
    bool isChannelActive(const std::string& channel) const;
};

/**
 * @class Learner
 * @brief Base class representing a registered platform learner.
 */
class Learner {
protected:
    int learnerId;          ///< Unique learner ID.
    std::string fullName;   ///< Learner full name.
    std::string email;      ///< Learner email address.
    ContactAddress address; ///< Learner address.
    AlertSettings alerts;   ///< Notification settings.
public:
    /**
     * @brief Constructs a Learner.
     * @param id Learner ID.
     * @param name Full name.
     * @param email Email address.
     * @param addr ContactAddress.
     */
    Learner(int id = 0, std::string name = "", std::string email = "", ContactAddress addr = ContactAddress());

    virtual ~Learner() = default;

    int getLearnerId() const { return learnerId; }
    std::string getFullName() const { return fullName; }
    std::string getEmail() const { return email; }

    /**
     * @brief Validates basic email format ('@' and '.').
     * @return True if email looks valid.
     */
    bool validateEmail() const;

    /**
     * @brief Updates the learner's address.
     * @param newAddress New ContactAddress.
     */
    void updateAddress(const ContactAddress& newAddress);

    /**
     * @brief Applies new AlertSettings.
     * @param settings New AlertSettings.
     */
    void applyAlertSettings(const AlertSettings& settings);

    /**
     * @brief Returns the learner's account tier name.
     * @return Tier string.
     */
    virtual std::string getAccountTier() const;
};

/**
 * @class StudyPass
 * @brief Controls active course seats, suspension status, and borrowing/seat limits.
 */
class StudyPass {
private:
    std::string passCode; ///< Unique pass code.
    int maxActiveCourses; ///< Maximum simultaneous courses allowed.
    int activeCourses;    ///< Current active courses count.
    bool frozen;          ///< Freeze/suspension flag.
public:
    /**
     * @brief Constructs a StudyPass.
     * @param code Pass code.
     * @param maxCourses Max active courses.
     */
    StudyPass(std::string code = "PASS-000", int maxCourses = 5);

    /**
     * @brief Checks whether learner can enroll in another course.
     * @return True if not frozen and activeCourses < maxActiveCourses.
     */
    bool canEnrollMore() const;

    /**
     * @brief Increments active course count.
     * @throws AccessDeniedException if frozen or limit reached.
     */
    void activateCourseSlot();

    /**
     * @brief Decrements active course count upon completion/drop.
     */
    void releaseCourseSlot();

    /**
     * @brief Freezes or unfreezes the pass.
     * @param state True to freeze, false to unfreeze.
     */
    void setFrozen(bool state);
};

/**
 * @class StudentLearner
 * @brief Learner enrolled in active courses with tuition/penalty balance tracking.
 */
class StudentLearner : public Learner {
protected:
    StudyPass pass;                  ///< Learner's StudyPass.
    std::vector<std::string> enrolledCodes; ///< Codes of enrolled courses.
    double overdueBalance;           ///< Unpaid balance or retake fees.
public:
    /**
     * @brief Constructs a StudentLearner.
     * @param base Base Learner.
     * @param pass StudyPass instance.
     */
    StudentLearner(const Learner& base, StudyPass pass = StudyPass());

    /**
     * @brief Enrolls in a Course, reserving one seat and activating a pass slot.
     * @param course Target Course.
     * @throws AccessDeniedException if overdueBalance > 50 or pass is blocked.
     * @throws SeatLimitExceededException if course has no seats.
     */
    void enrollCourse(Course& course);

    /**
     * @brief Drops an enrolled Course and frees a seat/slot.
     * @param course Course to drop.
     */
    void dropCourse(Course& course);

    /**
     * @brief Adds a fee or penalty to overdueBalance.
     * @param amount Amount to add.
     */
    void chargeFee(double amount);

    /**
     * @brief Pays down overdueBalance.
     * @param amount Payment amount (must be > 0).
     * @throws InvalidBillingException if amount <= 0.
     */
    void settleBalance(double amount);

    std::string getAccountTier() const override;
};

/**
 * @class UniversityStudent
 * @brief StudentLearner eligible for academic discounts based on graduation year.
 */
class UniversityStudent : public StudentLearner {
private:
    std::string universityName; ///< University name.
    std::string studentCardId;  ///< Student ID string.
    int graduationYear;         ///< Expected graduation year.
public:
    /**
     * @brief Constructs a UniversityStudent.
     * @param base Base StudentLearner.
     * @param uni University name.
     * @param cardId Student card ID.
     * @param gradYear Graduation year.
     */
    UniversityStudent(const StudentLearner& base, std::string uni = "Tech University",
                      std::string cardId = "STU-100", int gradYear = 2027);

    /**
     * @brief Checks whether student is still studying in currentYear.
     * @param currentYear Current year.
     * @return True if currentYear <= graduationYear.
     */
    bool isEligibleForStudentDiscount(int currentYear) const;

    /**
     * @brief Computes discounted fee (50% off for students).
     * @param baseFee Original fee.
     * @return Discounted fee.
     */
    double calculateStudentFee(double baseFee) const;

    std::string getAccountTier() const override;
};

/**
 * @class CorporateLearner
 * @brief StudentLearner sponsored by a company with extended study windows.
 */
class CorporateLearner : public StudentLearner {
private:
    std::string companyName; ///< Sponsoring company name.
    std::string department;  ///< Employee department.
    int accessWindowDays;  ///< Study window in days.
public:
    /**
     * @brief Constructs a CorporateLearner.
     * @param base Base StudentLearner.
     * @param company Company name.
     * @param dept Department name.
     * @param days Study window days.
     */
    CorporateLearner(const StudentLearner& base, std::string company = "Acme Corp",
                     std::string dept = "Engineering", int days = 90);

    /**
     * @brief Calculates allowed completion days based on whether course is self-paced.
     * @param isSelfPaced True if self-paced.
     * @return Allowed days.
     */
    int computeCompletionDays(bool isSelfPaced) const;

    /**
     * @brief Checks whether learner can request team seat transfers.
     * @return Always true for corporate learners.
     */
    bool canTransferTeamSeat() const;

    std::string getAccountTier() const override;
};

/**
 * @class RewardWallet
 * @brief Stores learning points, subscription tier, and total learning spend.
 */
class RewardWallet {
private:
    int points;           ///< Redeemable reward points.
    std::string tierName; ///< Loyalty tier (Bronze, Gold, Platinum).
    double totalSpend;    ///< Cumulative spend.
public:
    /**
     * @brief Constructs a RewardWallet.
     * @param pts Initial points.
     * @param tier Initial tier name.
     * @param spend Initial total spend.
     */
    RewardWallet(int pts = 0, std::string tier = "Bronze", double spend = 0.0);

    /**
     * @brief Awards points for a purchase and updates tier.
     * @param amount Purchase amount.
     */
    void creditPurchase(double amount);

    /**
     * @brief Redeems points for a monetary discount ($0.01 per point).
     * @param pts Points to redeem.
     * @return Discount value.
     * @throws InsufficientBalanceException if pts <= 0 or pts > points.
     */
    double redeemPoints(int pts);

    /**
     * @brief Updates tier based on totalSpend thresholds.
     */
    void refreshTier();

    int getPoints() const { return points; }
};

/**
 * @class ProSubscriber
 * @brief Premium subscriber with personal discount rate, reward wallet, and learning advisor.
 */
class ProSubscriber : public Learner {
private:
    RewardWallet wallet;      ///< Subscriber's RewardWallet.
    double discountRate;      ///< Personal discount fraction (e.g., 0.15).
    std::string advisorName;  ///< Assigned learning advisor name.
public:
    /**
     * @brief Constructs a ProSubscriber.
     * @param base Base Learner.
     * @param rate Discount fraction.
     * @param advisor Advisor name.
     */
    ProSubscriber(const Learner& base, double rate = 0.15, std::string advisor = "Elena");

    /**
     * @brief Applies personal subscriber discount to a course price.
     * @param originalPrice Original price.
     * @return Discounted price.
     */
    double applyProPrice(double originalPrice) const;

    /**
     * @brief Grants bonus points to the subscriber's wallet.
     * @param bonusPoints Bonus points to grant.
     */
    void grantBonusPoints(int bonusPoints);

    /**
     * @brief Checks whether subscriber gets free 1-on-1 code reviews.
     * @return True if discountRate >= 0.10.
     */
    bool qualifiesForFreeCodeReview() const;

    std::string getAccountTier() const override;
};

/**
 * @class Visitor
 * @brief Unregistered visitor browsing free previews and public courses.
 */
class Visitor {
private:
    std::string sessionId;  ///< Anonymous session ID.
    int previewsViewed;    ///< Count of preview lessons viewed.
    bool agreedToTerms;     ///< Terms acceptance flag.
public:
    /**
     * @brief Constructs a Visitor session.
     * @param sid Session ID.
     */
    Visitor(std::string sid = "VISITOR-00");

    /**
     * @brief Increments viewed previews counter.
     */
    void recordPreviewView();

    /**
     * @brief Checks whether visitor has an active session ID.
     * @return True if sessionId is non-empty.
     */
    bool canViewFreePreviews() const;
};

/**
 * @class LearnerRegistry
 * @brief Registers and looks up platform learners by ID or email.
 */
class LearnerRegistry {
private:
    std::vector<Learner> learners; ///< Registered learners.
    int nextId;                    ///< Next auto-increment ID.
public:
    LearnerRegistry();

    /**
     * @brief Registers a new Learner after validating name and unique email.
     * @param name Full name.
     * @param email Unique email.
     * @param addr ContactAddress.
     * @return Created Learner.
     * @throws InvalidAccountException if invalid or duplicate email.
     */
    Learner registerLearner(const std::string& name, const std::string& email, const ContactAddress& addr);

    /**
     * @brief Checks if a learner with email already exists.
     * @param email Email string.
     * @return True if found.
     */
    bool existsByEmail(const std::string& email) const;

    /**
     * @brief Finds a Learner by numeric ID.
     * @param id Learner ID.
     * @return Matching Learner.
     * @throws InvalidAccountException if not found.
     */
    Learner findById(int id) const;

    size_t countLearners() const { return learners.size(); }
};
