/**
 * @file orders.hpp
 * @brief Orders, Invoices, Payments, and Promotions definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "book.hpp"
#include "users.hpp"

/**
 * @class OrderItem
 * @brief Represents a single line item (Book + quantity + unit price) within an Order.
 */
class OrderItem {
private:
    Book book;        ///< Ordered Book snapshot.
    int quantity;     ///< Number of copies ordered.
    double unitPrice; ///< Unit price captured at order creation.
public:
    /**
     * @brief Constructs an OrderItem with positive quantity validation.
     * @param book Book being ordered.
     * @param qty Quantity (must be > 0).
     * @throws InvalidBookDataException if qty <= 0.
     */
    OrderItem(const Book& book, int qty = 1);

    /**
     * @brief Calculates line subtotal (unitPrice * quantity).
     * @return Subtotal amount.
     */
    double calculateSubtotal() const;

    /**
     * @brief Updates the ordered quantity for this line item.
     * @param newQty New positive quantity.
     * @throws InvalidBookDataException if newQty <= 0.
     */
    void changeQuantity(int newQty);

    /**
     * @brief Checks whether this line item corresponds to a specific ISBN.
     * @param isbn ISBN string to compare.
     * @return True if book ISBN matches.
     */
    bool matchesIsbn(const std::string& isbn) const;

    Book getBook() const { return book; }
};

/**
 * @class Discount
 * @brief Represents a percentage discount rule with a minimum order threshold.
 */
class Discount {
private:
    std::string code;      ///< Promo code string.
    double percentage;     ///< Discount percentage in [0, 100].
    double minOrderAmount; ///< Minimum order total required for eligibility.
public:
    /**
     * @brief Constructs a Discount and validates percentage bounds.
     * @param code Promo code.
     * @param pct Percentage in [0, 100].
     * @param minAmount Minimum order total threshold.
     * @throws InvalidDiscountException if pct < 0 or pct > 100.
     */
    Discount(std::string code = "NONE", double pct = 0.0, double minAmount = 0.0);

    /**
     * @brief Checks whether the discount applies to a given order total.
     * @param orderTotal Raw order subtotal.
     * @return True if orderTotal >= minOrderAmount and percentage > 0.
     */
    bool isApplicable(double orderTotal) const;

    /**
     * @brief Calculates monetary savings for a given order total.
     * @param orderTotal Raw order subtotal.
     * @return Discount amount saved (0.0 if not applicable).
     */
    double computeSavings(double orderTotal) const;

    std::string getCode() const { return code; }
};

/**
 * @class Promotion
 * @brief Represents a time- or quota-limited promotional campaign wrapping a Discount.
 */
class Promotion {
private:
    std::string campaignName;  ///< Marketing campaign title.
    Discount attachedDiscount; ///< Underlying Discount rule.
    bool active;               ///< Campaign active flag.
    int usageLimit;            ///< Remaining redemption quota.
public:
    /**
     * @brief Constructs a Promotion campaign.
     * @param name Campaign name.
     * @param disc Attached Discount.
     * @param active Initial active status.
     * @param limit Maximum redemptions allowed.
     */
    Promotion(std::string name = "Seasonal", Discount disc = Discount(), bool active = true, int limit = 100);

    /**
     * @brief Applies the promotion to an amount and decrements remaining quota if used.
     * @param amount Original monetary amount.
     * @return Final amount after promotional savings.
     */
    double applyPromotion(double amount);

    /**
     * @brief Deactivates the promotional campaign immediately.
     */
    void deactivateCampaign();

    /**
     * @brief Checks whether the promotion is active and has remaining redemptions.
     * @return True if usable.
     */
    bool hasRemainingQuota() const;
};

/**
 * @class Order
 * @brief Represents a customer purchase order containing items, discounts, and lifecycle status.
 */
class Order {
private:
    int orderId;                  ///< Unique numeric order ID.
    User customer;                ///< Customer who placed the order.
    std::vector<OrderItem> items; ///< List of ordered items.
    std::string status;           ///< Lifecycle state (Draft, Confirmed, Cancelled).
    double discountAmount;        ///< Monetary discount applied to the order.
public:
    /**
     * @brief Constructs a Draft Order for a customer.
     * @param id Unique order ID.
     * @param customer Purchasing User.
     */
    Order(int id = 1, const User& customer = User());

    int getOrderId() const { return orderId; }
    std::string getStatus() const { return status; }

    /**
     * @brief Adds a Book line item to the order after verifying stock availability.
     * @param book Book to add.
     * @param quantity Number of copies.
     * @throws OutOfStockException if book stock is insufficient.
     */
    void addItem(const Book& book, int quantity);

    /**
     * @brief Validates and applies a Discount to the order.
     * @param discount Discount rule to apply.
     * @throws InvalidDiscountException if the order does not meet discount criteria.
     */
    void applyDiscount(const Discount& discount);

    /**
     * @brief Calculates final order total after subtracting applied discounts.
     * @return Net order total.
     */
    double calculateTotal() const;

    /**
     * @brief Transitions the order status to "Confirmed" if items are present.
     * @throws OrderNotFoundException if the order contains no items.
     */
    void confirmOrder();

    /**
     * @brief Transitions the order status to "Cancelled".
     */
    void cancelOrder();
};

/**
 * @class Invoice
 * @brief Represents a billing invoice with tax calculation generated for an Order.
 */
class Invoice {
private:
    std::string invoiceNumber; ///< Unique invoice document number.
    Order order;               ///< Associated Order.
    double taxRate;            ///< Applicable tax rate (e.g., 0.20 for 20%).
    bool paid;                 ///< Settlement status flag.
public:
    /**
     * @brief Constructs an Invoice for an Order.
     * @param number Invoice number string.
     * @param order Associated Order.
     * @param tax Tax rate fraction.
     */
    Invoice(std::string number = "INV-001", const Order& order = Order(), double tax = 0.20);

    /**
     * @brief Calculates the tax amount due on the order total.
     * @return Tax amount.
     */
    double calculateTaxAmount() const;

    /**
     * @brief Calculates the grand total including taxes.
     * @return Order total plus tax.
     */
    double calculateGrandTotal() const;

    /**
     * @brief Marks the invoice as paid.
     */
    void markAsPaid();

    bool isPaid() const { return paid; }
};

/**
 * @class Payment
 * @brief Base class for payment transactions settling an Invoice.
 */
class Payment {
protected:
    std::string transactionId; ///< Payment gateway transaction ID.
    double amount;             ///< Payment amount.
    std::string currency;      ///< Three-letter currency code.
    bool completed;            ///< Completion status flag.
public:
    /**
     * @brief Constructs a Payment transaction.
     * @param txId Transaction identifier.
     * @param amount Payment amount.
     * @param currency Currency code.
     */
    Payment(std::string txId = "TX-000", double amount = 0.0, std::string currency = "USD");

    /**
     * @brief Virtual destructor for polymorphic payment methods.
     */
    virtual ~Payment() = default;

    /**
     * @brief Processes payment against an Invoice and marks it paid on success.
     * @param invoice Target Invoice to settle.
     * @return True if payment succeeded.
     * @throws InsufficientFundsException if payment amount is less than invoice grand total.
     */
    virtual bool processPayment(Invoice& invoice);

    /**
     * @brief Reverts the payment completion status for refunds.
     */
    void refundPayment();

    bool isCompleted() const { return completed; }
};

/**
 * @class CreditCardPayment
 * @brief Payment implementation using a credit card with available credit limit verification.
 */
class CreditCardPayment : public Payment {
private:
    std::string maskedCardNumber; ///< Masked PAN (e.g., ****-1234).
    std::string expiryDate;       ///< Card expiration date string.
    double availableCredit;       ///< Available credit limit on the card.
public:
    /**
     * @brief Constructs a CreditCardPayment.
     * @param txId Transaction ID.
     * @param amount Payment amount.
     * @param card Masked card number.
     * @param exp Expiration date.
     * @param credit Available credit limit.
     */
    CreditCardPayment(std::string txId, double amount, std::string card, std::string exp, double credit);

    /**
     * @brief Verifies card expiration against the current year.
     * @param currentYear Current calendar year.
     * @return True if card is not expired.
     */
    bool verifyCardExpiry(int currentYear) const;

    /**
     * @brief Charges the credit card limit and settles the Invoice.
     * @param invoice Invoice to pay.
     * @return True if charged successfully.
     * @throws InsufficientFundsException if availableCredit < invoice grand total.
     */
    bool processPayment(Invoice& invoice) override;
};

/**
 * @class CashPayment
 * @brief Point-of-sale cash payment tracking tendered cash and customer change.
 */
class CashPayment : public Payment {
private:
    double cashTendered;   ///< Cash handed over by customer.
    double changeReturned; ///< Change owed back to customer.
public:
    /**
     * @brief Constructs a CashPayment.
     * @param txId Transaction ID.
     * @param amount Required amount.
     * @param tendered Cash tendered by customer.
     */
    CashPayment(std::string txId, double amount, double tendered);

    /**
     * @brief Calculates change due when tendered cash exceeds amount.
     * @return Change amount.
     */
    double calculateChange() const;

    /**
     * @brief Validates tendered cash and settles the Invoice.
     * @param invoice Invoice to pay.
     * @return True if cash covers the grand total.
     * @throws InsufficientFundsException if cashTendered < invoice grand total.
     */
    bool processPayment(Invoice& invoice) override;
};

/**
 * @class PayPalPayment
 * @brief Electronic wallet payment requiring two-factor authentication verification.
 */
class PayPalPayment : public Payment {
private:
    std::string payerEmail; ///< PayPal account email.
    bool twoFactorVerified; ///< 2FA verification flag.
public:
    /**
     * @brief Constructs a PayPalPayment.
     * @param txId Transaction ID.
     * @param amount Payment amount.
     * @param email Payer PayPal email.
     * @param verified Whether 2FA passed.
     */
    PayPalPayment(std::string txId, double amount, std::string email, bool verified);

    /**
     * @brief Verifies 2FA completion and valid payer email format.
     * @return True if account security check passes.
     */
    bool verifyAccountSecurity() const;

    /**
     * @brief Executes PayPal payment after verifying account security.
     * @param invoice Invoice to settle.
     * @return True if settled.
     * @throws InvalidPaymentException if 2FA security verification fails.
     * @throws InsufficientFundsException if amount is insufficient.
     */
    bool processPayment(Invoice& invoice) override;
};

/**
 * @class TransactionManager
 * @brief Coordinates order confirmation, invoice settlement, and cumulative revenue tracking.
 */
class TransactionManager {
private:
    std::vector<Invoice> invoices; ///< Ledger of recorded invoices.
    double totalRevenue;           ///< Cumulative settled revenue.
public:
    TransactionManager();

    /**
     * @brief Appends an Invoice to the internal ledger.
     * @param invoice Invoice to record.
     */
    void registerInvoice(const Invoice& invoice);

    /**
     * @brief Processes payment, confirms the order, and updates total revenue.
     * @param order Order to confirm.
     * @param invoice Invoice to settle.
     * @param payment Payment method to execute.
     * @return True if settlement succeeded.
     */
    bool settleOrderPayment(Order& order, Invoice& invoice, Payment& payment);

    double getTotalRevenue() const { return totalRevenue; }

    /**
     * @brief Counts how many invoices in the ledger are marked as paid.
     * @return Number of paid invoices.
     */
    size_t countPaidInvoices() const;
};