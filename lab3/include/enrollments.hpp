/**
 * @file enrollments.hpp
 * @brief Enrollments, Coupons, Receipts, and Billing definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "courses.hpp"
#include "learners.hpp"

/**
 * @class EnrollmentItem
 * @brief Represents a single Course line item with seat quantity and unit tuition.
 */
class EnrollmentItem {
private:
    Course course;   ///< Selected Course.
    int seatCount;   ///< Number of seats.
    double unitFee;  ///< Tuition per seat.
public:
    /**
     * @brief Constructs an EnrollmentItem.
     * @param course Target Course.
     * @param seats Seat count (must be > 0).
     * @throws InvalidCourseDataException if seats <= 0.
     */
    EnrollmentItem(const Course& course, int seats = 1);

    /**
     * @brief Calculates line item subtotal (unitFee * seatCount).
     * @return Subtotal amount.
     */
    double calculateSubtotal() const;

    /**
     * @brief Updates seat count for the item.
     * @param newSeats New seat count (must be > 0).
     * @throws InvalidCourseDataException if newSeats <= 0.
     */
    void updateSeatCount(int newSeats);

    /**
     * @brief Checks if item matches a given course code.
     * @param code Course code string.
     * @return True if code matches.
     */
    bool matchesCourseCode(const std::string& code) const;

    Course getCourse() const { return course; }
};

/**
 * @class Coupon
 * @brief Represents a percentage discount coupon with minimum spend requirement.
 */
class Coupon {
private:
    std::string code;      ///< Coupon code.
    double discountPct;    ///< Discount percentage in [0, 100].
    double minSpendAmount; ///< Minimum enrollment subtotal required.
public:
    /**
     * @brief Constructs a Coupon and validates percentage range.
     * @param code Coupon code.
     * @param pct Percentage in [0, 100].
     * @param minSpend Minimum spend threshold.
     * @throws InvalidCouponException if pct < 0 or pct > 100.
     */
    Coupon(std::string code = "NONE", double pct = 0.0, double minSpend = 0.0);

    /**
     * @brief Checks whether coupon applies to a given subtotal.
     * @param subtotal Raw enrollment subtotal.
     * @return True if applicable.
     */
    bool isApplicable(double subtotal) const;

    /**
     * @brief Computes monetary discount saved on a given subtotal.
     * @param subtotal Raw enrollment subtotal.
     * @return Savings amount.
     */
    double computeSavings(double subtotal) const;

    std::string getCode() const { return code; }
};

/**
 * @class PromoCampaign
 * @brief Quota-limited promotional campaign wrapping a Coupon.
 */
class PromoCampaign {
private:
    std::string title;     ///< Campaign name.
    Coupon coupon;         ///< Attached Coupon.
    bool active;           ///< Active status flag.
    int remainingUses;     ///< Remaining redemptions.
public:
    /**
     * @brief Constructs a PromoCampaign.
     * @param title Campaign title.
     * @param coupon Attached Coupon.
     * @param active Active flag.
     * @param uses Max uses quota.
     */
    PromoCampaign(std::string title = "SpringPromo", Coupon coupon = Coupon(), bool active = true, int uses = 50);

    /**
     * @brief Applies campaign discount to an amount and decrements quota.
     * @param amount Original amount.
     * @return Final amount after savings.
     */
    double applyCampaign(double amount);

    /**
     * @brief Deactivates the campaign.
     */
    void deactivate();

    /**
     * @brief Checks whether campaign is active and has remaining uses.
     * @return True if available.
     */
    bool hasQuotaLeft() const;
};

/**
 * @class Enrollment
 * @brief Represents a learner's course enrollment order with items, coupon discount, and status.
 */
class Enrollment {
private:
    int enrollmentId;              ///< Unique enrollment ID.
    Learner learner;               ///< Enrolling Learner.
    std::vector<EnrollmentItem> items; ///< Course items.
    std::string status;            ///< Status (Pending, Confirmed, Cancelled).
    double discountApplied;        ///< Applied discount value.
public:
    /**
     * @brief Constructs an Enrollment for a Learner.
     * @param id Enrollment ID.
     * @param learner Enrolling Learner.
     */
    Enrollment(int id = 1, const Learner& learner = Learner());

    int getEnrollmentId() const { return enrollmentId; }
    std::string getStatus() const { return status; }

    /**
     * @brief Adds a Course with seat count after checking seat availability.
     * @param course Course to enroll in.
     * @param seats Number of seats.
     * @throws SeatLimitExceededException if course has fewer seats than requested.
     */
    void addCourseItem(const Course& course, int seats);

    /**
     * @brief Applies a Coupon to the enrollment if eligible.
     * @param coupon Coupon to apply.
     * @throws InvalidCouponException if coupon does not apply to current subtotal.
     */
    void applyCoupon(const Coupon& coupon);

    /**
     * @brief Calculates net total after discount.
     * @return Net amount.
     */
    double calculateNetTotal() const;

    /**
     * @brief Confirms enrollment if at least one item exists.
     * @throws EnrollmentNotFoundException if items list is empty.
     */
    void confirmEnrollment();

    /**
     * @brief Cancels the enrollment.
     */
    void cancelEnrollment();
};

/**
 * @class Receipt
 * @brief Represents a billing receipt with tax calculation for an Enrollment.
 */
class Receipt {
private:
    std::string receiptNumber; ///< Unique receipt number.
    Enrollment enrollment;     ///< Associated Enrollment.
    double taxRate;            ///< Tax rate fraction (e.g., 0.20).
    bool settled;              ///< Payment status flag.
public:
    /**
     * @brief Constructs a Receipt.
     * @param number Receipt number.
     * @param enrollment Associated Enrollment.
     * @param tax Tax rate fraction.
     */
    Receipt(std::string number = "RCP-001", const Enrollment& enrollment = Enrollment(), double tax = 0.20);

    /**
     * @brief Calculates tax on the enrollment net total.
     * @return Tax amount.
     */
    double calculateTax() const;

    /**
     * @brief Calculates total payable including tax.
     * @return Grand total.
     */
    double calculateGrandTotal() const;

    /**
     * @brief Marks receipt as settled/paid.
     */
    void markSettled();

    bool isSettled() const { return settled; }
};

/**
 * @class BillingTransaction
 * @brief Base billing payment transaction settling a Receipt.
 */
class BillingTransaction {
protected:
    std::string txId;  ///< Transaction ID.
    double amount;     ///< Transaction amount.
    std::string currency;///< Currency code.
    bool paid;         ///< Completion flag.
public:
    /**
     * @brief Constructs a BillingTransaction.
     * @param id Transaction ID.
     * @param amount Amount.
     * @param currency Currency code.
     */
    BillingTransaction(std::string id = "TX-000", double amount = 0.0, std::string currency = "USD");

    virtual ~BillingTransaction() = default;

    /**
     * @brief Processes payment against a Receipt.
     * @param receipt Target Receipt.
     * @return True if paid successfully.
     * @throws InsufficientBalanceException if amount < receipt grand total.
     */
    virtual bool executePayment(Receipt& receipt);

    /**
     * @brief Marks transaction as refunded/reversed.
     */
    void reverseTransaction();

    bool isPaid() const { return paid; }
};

/**
 * @class CardBilling
 * @brief Payment by credit/debit card checking card expiration and available limit.
 */
class CardBilling : public BillingTransaction {
private:
    std::string maskedPan;     ///< Masked card number.
    std::string expiry;        ///< Expiration date string.
    double availableLimit;   ///< Available credit limit.
public:
    /**
     * @brief Constructs a CardBilling transaction.
     * @param id Transaction ID.
     * @param amount Amount.
     * @param pan Masked card number.
     * @param exp Expiration date.
     * @param limit Available credit limit.
     */
    CardBilling(std::string id, double amount, std::string pan, std::string exp, double limit);

    /**
     * @brief Verifies card expiration against currentYear.
     * @param currentYear Current calendar year.
     * @return True if valid.
     */
    bool checkExpiry(int currentYear) const;

    /**
     * @brief Charges card limit and settles the Receipt.
     * @param receipt Target Receipt.
     * @return True if successful.
     * @throws InsufficientBalanceException if availableLimit < receipt grand total.
     */
    bool executePayment(Receipt& receipt) override;
};

/**
 * @class WalletBilling
 * @brief Payment from learner's stored wallet balance.
 */
class WalletBilling : public BillingTransaction {
private:
    double walletBalance; ///< Available wallet balance.
    double bonusCredit;   ///< Promotional bonus credit.
public:
    /**
     * @brief Constructs a WalletBilling transaction.
     * @param id Transaction ID.
     * @param amount Amount.
     * @param balance Wallet balance.
     * @param bonus Bonus credit.
     */
    WalletBilling(std::string id, double amount, double balance, double bonus = 0.0);

    /**
     * @brief Computes total spendable funds (walletBalance + bonusCredit).
     * @return Total available funds.
     */
    double getTotalFunds() const;

    /**
     * @brief Deducts funds and settles Receipt.
     * @param receipt Target Receipt.
     * @return True if paid.
     * @throws InsufficientBalanceException if total funds < receipt grand total.
     */
    bool executePayment(Receipt& receipt) override;
};

/**
 * @class BankTransferBilling
 * @brief Payment via bank transfer requiring verified IBAN/reference.
 */
class BankTransferBilling : public BillingTransaction {
private:
    std::string senderEmail;  ///< Payer email.
    bool referenceVerified;   ///< Whether bank reference was verified.
public:
    /**
     * @brief Constructs a BankTransferBilling transaction.
     * @param id Transaction ID.
     * @param amount Amount.
     * @param email Payer email.
     * @param verified Verification status.
     */
    BankTransferBilling(std::string id, double amount, std::string email, bool verified);

    /**
     * @brief Verifies bank transfer reference and email format.
     * @return True if verified.
     */
    bool verifyTransfer() const;

    /**
     * @brief Executes transfer payment after verification.
     * @param receipt Target Receipt.
     * @return True if settled.
     * @throws InvalidBillingException if transfer is not verified.
     * @throws InsufficientBalanceException if amount is insufficient.
     */
    bool executePayment(Receipt& receipt) override;
};

/**
 * @class BillingManager
 * @brief Coordinates enrollment confirmation, receipt settlement, and revenue tracking.
 */
class BillingManager {
private:
    std::vector<Receipt> receipts; ///< Recorded receipts.
    double totalCollected;         ///< Total collected revenue.
public:
    BillingManager();

    /**
     * @brief Registers a Receipt in the ledger.
     * @param receipt Receipt to record.
     */
    void recordReceipt(const Receipt& receipt);

    /**
     * @brief Processes payment, confirms enrollment, and updates collected revenue.
     * @param enrollment Target Enrollment.
     * @param receipt Target Receipt.
     * @param tx BillingTransaction to execute.
     * @return True if settled.
     */
    bool settleEnrollment(Enrollment& enrollment, Receipt& receipt, BillingTransaction& tx);

    double getTotalCollected() const { return totalCollected; }

    /**
     * @brief Counts settled receipts.
     * @return Count of settled receipts.
     */
    size_t countSettledReceipts() const;
};
