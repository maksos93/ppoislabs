/**
 * @file staff.hpp
 * @brief Employees, HR, Supply Chain, Delivery, Analytics, and Review definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "book.hpp"
#include "users.hpp"
#include "orders.hpp"

/**
 * @class Employee
 * @brief Base class representing a staff member with role, salary, and home address.
 */
class Employee {
protected:
    int employeeId;      ///< Unique staff ID.
    std::string name;    ///< Employee full name.
    std::string role;    ///< Job title / role.
    double baseSalary;   ///< Monthly base salary.
    Address homeAddress; ///< Residential address.
public:
    /**
     * @brief Constructs an Employee record.
     * @param id Employee ID.
     * @param name Full name.
     * @param role Job role.
     * @param salary Monthly salary.
     * @param addr Home Address.
     */
    Employee(int id = 1, std::string name = "Staff", std::string role = "General",
             double salary = 2000.0, Address addr = Address());

    /**
     * @brief Virtual destructor for staff hierarchy.
     */
    virtual ~Employee() = default;

    int getEmployeeId() const { return employeeId; }
    std::string getName() const { return name; }
    std::string getRole() const { return role; }

    /**
     * @brief Updates the employee's role and applies a salary raise.
     * @param newRole New job role.
     * @param raiseAmount Monthly raise increment (applied if > 0).
     */
    void promote(const std::string& newRole, double raiseAmount);

    /**
     * @brief Calculates annual gross compensation (baseSalary * 12).
     * @return Annual salary.
     */
    double calculateAnnualCompensation() const;

    /**
     * @brief Checks whether the employee is authorized for a required role.
     * @param requiredRole Role needed for the action.
     * @return True if employee role matches or is "Administrator".
     */
    bool authorizeAction(const std::string& requiredRole) const;
};

/**
 * @class Librarian
 * @brief Library desk staff responsible for issuing and accepting returned books.
 */
class Librarian : public Employee {
private:
    std::string deskZone; ///< Assigned reading room or service desk zone.
    int booksProcessed;   ///< Counter of circulation transactions handled.
public:
    /**
     * @brief Constructs a Librarian from a base Employee.
     * @param base Base Employee profile.
     * @param zone Service desk zone.
     */
    Librarian(const Employee& base, std::string zone = "Main Hall");

    /**
     * @brief Issues a book to a Reader and increments processed counter.
     * @param reader Borrowing Reader.
     * @param book Book being lent.
     */
    void issueBookToReader(Reader& reader, Book& book);

    /**
     * @brief Processes a book return from a Reader and increments processed counter.
     * @param reader Returning Reader.
     * @param book Book being returned.
     */
    void acceptReturnedBook(Reader& reader, Book& book);

    int getBooksProcessed() const { return booksProcessed; }
};

/**
 * @class Cataloger
 * @brief Specialist responsible for indexing new books and assigning call numbers.
 */
class Cataloger : public Employee {
private:
    std::string classificationStandard; ///< Library classification system (e.g., UDC, Dewey).
    int recordsCreated;                 ///< Number of catalog records created.
public:
    /**
     * @brief Constructs a Cataloger from a base Employee.
     * @param base Base Employee profile.
     * @param standard Classification standard code.
     */
    Cataloger(const Employee& base, std::string standard = "UDC");

    /**
     * @brief Registers a new Book into the CatalogManager.
     * @param catalog Target CatalogManager.
     * @param book Book to index.
     */
    void indexNewBook(CatalogManager& catalog, const Book& book);

    /**
     * @brief Generates a standardized library call number for a Book.
     * @param book Book to classify.
     * @return Formatted call number string.
     */
    std::string generateCallNumber(const Book& book) const;
};

/**
 * @class Cashier
 * @brief Bookstore point-of-sale operator managing register drawer balance.
 */
class Cashier : public Employee {
private:
    int registerNumber;   ///< Assigned POS terminal number.
    double drawerBalance; ///< Current cash amount in register drawer.
public:
    /**
     * @brief Constructs a Cashier from a base Employee.
     * @param base Base Employee profile.
     * @param regNum Register number.
     * @param openingFloat Initial cash float in drawer.
     */
    Cashier(const Employee& base, int regNum = 1, double openingFloat = 200.0);

    /**
     * @brief Records a cash sale into the register drawer.
     * @param amount Sale amount (must be >= 0).
     * @throws InvalidPaymentException if amount < 0.
     */
    void recordCashSale(double amount);

    /**
     * @brief Computes cash variance at shift close (countedCash - drawerBalance).
     * @param countedCash Actual physical cash counted.
     * @return Over/short difference.
     */
    double closeShiftReconciliation(double countedCash) const;
};

/**
 * @class Manager
 * @brief Department manager authorized to approve high-value orders and override discounts.
 */
class Manager : public Employee {
private:
    std::string departmentName; ///< Managed department name.
    double approvalLimit;       ///< Maximum order amount the manager can approve.
public:
    /**
     * @brief Constructs a Manager from a base Employee.
     * @param base Base Employee profile.
     * @param dept Department name.
     * @param limit Financial approval ceiling.
     */
    Manager(const Employee& base, std::string dept = "Operations", double limit = 5000.0);

    /**
     * @brief Checks whether an order's total falls within the manager's approval limit.
     * @param order Order to evaluate.
     * @return True if order total <= approvalLimit.
     */
    bool approveHighValueOrder(const Order& order) const;

    /**
     * @brief Applies a manual discount override to an Order.
     * @param order Target Order.
     * @param discount Discount to apply.
     */
    void overrideOrderDiscount(Order& order, const Discount& discount) const;
};

/**
 * @class Administrator
 * @brief System administrator with security clearance to suspend or restore reader cards.
 */
class Administrator : public Employee {
private:
    int securityClearance; ///< Numeric clearance level.
    bool twoFactorActive;  ///< Whether admin 2FA session is active.
public:
    /**
     * @brief Constructs an Administrator from a base Employee.
     * @param base Base Employee profile.
     * @param clearance Security clearance level.
     * @param twoFactor 2FA active flag.
     */
    Administrator(const Employee& base, int clearance = 5, bool twoFactor = true);

    /**
     * @brief Suspends a reader's MembershipCard if 2FA is active.
     * @param card Target MembershipCard.
     * @throws UnauthorizedAccessException if twoFactorActive is false.
     */
    void suspendReaderCard(MembershipCard& card) const;

    /**
     * @brief Restores a suspended MembershipCard if 2FA is active.
     * @param card Target MembershipCard.
     * @throws UnauthorizedAccessException if twoFactorActive is false.
     */
    void restoreReaderCard(MembershipCard& card) const;
};

/**
 * @class ShiftSchedule
 * @brief Represents a work shift assignment for an employee on a given day.
 */
class ShiftSchedule {
private:
    int employeeId;       ///< Assigned employee ID.
    std::string shiftDay; ///< Day of the week.
    int startHour;        ///< Shift start hour (0..23).
    int endHour;          ///< Shift end hour (0..23).
public:
    /**
     * @brief Constructs a ShiftSchedule entry.
     * @param empId Employee ID.
     * @param day Day name.
     * @param start Start hour.
     * @param end End hour.
     */
    ShiftSchedule(int empId = 1, std::string day = "Monday", int start = 9, int end = 17);

    /**
     * @brief Calculates shift length in hours.
     * @return Duration in hours.
     */
    int calculateShiftDuration() const;

    /**
     * @brief Checks whether this shift overlaps in time with another shift on the same day.
     * @param other Another ShiftSchedule to compare.
     * @return True if schedules overlap.
     */
    bool overlapsWith(const ShiftSchedule& other) const;
};

/**
 * @class Salary
 * @brief Represents a payroll calculation record with gross pay, tax deductions, and bonuses.
 */
class Salary {
private:
    int employeeId;      ///< Employee ID.
    double grossPay;     ///< Gross pay before deductions.
    double taxDeduction; ///< Withheld tax amount.
    double bonus;        ///< Performance bonus amount.
public:
    /**
     * @brief Constructs a Salary payroll record.
     * @param empId Employee ID.
     * @param gross Gross pay.
     * @param tax Tax deduction.
     * @param bonus Bonus amount.
     */
    Salary(int empId = 1, double gross = 2000.0, double tax = 300.0, double bonus = 0.0);

    /**
     * @brief Computes net take-home pay (grossPay - taxDeduction + bonus).
     * @return Net pay amount.
     */
    double computeNetPay() const;

    /**
     * @brief Adds a positive performance bonus to the salary record.
     * @param amount Bonus increment.
     */
    void addPerformanceBonus(double amount);
};

/**
 * @class HRDepartment
 * @brief Manages staff roster, shift scheduling, and monthly payroll aggregation.
 */
class HRDepartment {
private:
    std::vector<Employee> staff;          ///< Active employees list.
    std::vector<ShiftSchedule> schedules; ///< Assigned shift schedules.
public:
    HRDepartment() = default;

    /**
     * @brief Adds a newly hired Employee to the staff roster.
     * @param emp Employee to hire.
     */
    void hireEmployee(const Employee& emp);

    /**
     * @brief Records a work ShiftSchedule.
     * @param schedule ShiftSchedule to assign.
     */
    void assignShift(const ShiftSchedule& schedule);

    /**
     * @brief Calculates total monthly base payroll across all staff.
     * @return Monthly payroll expense.
     */
    double calculateMonthlyPayroll() const;

    size_t getHeadcount() const { return staff.size(); }
};

/**
 * @class Supplier
 * @brief Represents an external book wholesaler or publisher vendor.
 */
class Supplier {
private:
    std::string supplierId;  ///< Unique vendor ID.
    std::string companyName; ///< Vendor legal name.
    double reliabilityScore; ///< On-time delivery rating in [0.0, 1.0].
    int leadTimeDays;        ///< Average delivery lead time in days.
public:
    /**
     * @brief Constructs a Supplier profile.
     * @param id Supplier ID.
     * @param name Company name.
     * @param score Reliability score [0.0..1.0].
     * @param lead Lead time in days.
     */
    Supplier(std::string id = "SUP-1", std::string name = "BookWholesale", double score = 0.95, int lead = 3);

    /**
     * @brief Determines whether the supplier meets preferred vendor SLA criteria.
     * @return True if reliabilityScore >= 0.90 and leadTimeDays <= 5.
     */
    bool isPreferredSupplier() const;

    /**
     * @brief Adjusts the supplier's reliability score based on delivery punctuality.
     * @param deliveredOnTime True if shipment arrived on schedule.
     */
    void updateReliability(bool deliveredOnTime);
};

/**
 * @class SupplyShipment
 * @brief Tracks an inbound book shipment from a Supplier to a destination Warehouse.
 */
class SupplyShipment {
private:
    std::string shipmentId; ///< Unique shipment tracking ID.
    Supplier supplier;      ///< Originating Supplier.
    Warehouse destination;  ///< Destination Warehouse.
    int totalUnits;         ///< Total book units in shipment.
    bool received;          ///< Whether shipment has been received.
public:
    /**
     * @brief Constructs a SupplyShipment.
     * @param id Shipment ID.
     * @param sup Originating Supplier.
     * @param dest Destination Warehouse.
     * @param units Total units shipped.
     */
    SupplyShipment(std::string id, const Supplier& sup, const Warehouse& dest, int units);

    /**
     * @brief Validates and dispatches the shipment.
     * @throws DeliveryException if totalUnits <= 0.
     */
    void dispatchShipment();

    /**
     * @brief Completes shipment delivery and registers the inventory item at the warehouse.
     * @param item InventoryItem received.
     */
    void completeDelivery(const InventoryItem& item);

    bool isReceived() const { return received; }
};

/**
 * @class Courier
 * @brief Represents a delivery courier responsible for transporting customer orders.
 */
class Courier {
private:
    std::string courierId;   ///< Unique courier identifier.
    std::string fullName;    ///< Courier full name.
    std::string vehicleType; ///< Transport type (e.g., Van, Bike).
    bool available;          ///< Availability status flag.
public:
    /**
     * @brief Constructs a Courier profile.
     * @param id Courier ID.
     * @param name Courier name.
     * @param vehicle Vehicle type.
     */
    Courier(std::string id = "CR-1", std::string name = "Alex", std::string vehicle = "Van");

    /**
     * @brief Assigns an active Order to the courier and marks them busy.
     * @param order Order to deliver.
     * @throws DeliveryException if courier is busy or order is cancelled.
     */
    void assignOrder(const Order& order);

    /**
     * @brief Marks the dropoff complete and restores courier availability.
     */
    void completeDropoff();

    bool isAvailable() const { return available; }
};

/**
 * @class DeliveryRoute
 * @brief Represents a multi-stop delivery route assigned to a Courier.
 */
class DeliveryRoute {
private:
    std::string routeCode;      ///< Unique route code.
    Courier assignedCourier;    ///< Courier executing the route.
    std::vector<Address> stops; ///< Ordered list of delivery stop addresses.
    double estimatedKm;         ///< Estimated total route distance in kilometers.
public:
    /**
     * @brief Constructs a DeliveryRoute.
     * @param code Route identifier.
     * @param courier Assigned Courier.
     * @param km Estimated distance in km.
     */
    DeliveryRoute(std::string code, const Courier& courier, double km = 15.0);

    /**
     * @brief Appends a delivery stop Address to the route.
     * @param addr Stop address.
     */
    void addStop(const Address& addr);

    /**
     * @brief Estimates fuel cost for the route given a per-kilometer rate.
     * @param costPerKm Fuel cost per kilometer.
     * @return Total estimated fuel expense.
     */
    double estimateFuelCost(double costPerKm) const;
};

/**
 * @class TrackingSystem
 * @brief Tracks real-time checkpoint location and status notes for shipments and orders.
 */
class TrackingSystem {
private:
    std::string trackingNumber;  ///< Public tracking number.
    std::string currentLocation; ///< Latest recorded facility or city.
    std::string statusNote;      ///< Latest status message.
public:
    /**
     * @brief Constructs a TrackingSystem entry.
     * @param trackNum Tracking number string.
     */
    TrackingSystem(std::string trackNum = "TRK-001");

    /**
     * @brief Updates the current checkpoint location and status description.
     * @param location New checkpoint location.
     * @param note Status update note.
     */
    void updateCheckpoint(const std::string& location, const std::string& note);

    /**
     * @brief Formats a concise summary of current tracking status.
     * @return Formatted tracking line.
     */
    std::string formatTrackingSummary() const;
};

/**
 * @class ReturnRequest
 * @brief Represents a customer request to return a purchased book from an Order.
 */
class ReturnRequest {
private:
    int requestId;      ///< Unique RMA / return request ID.
    Order originalOrder;///< Original purchase Order.
    Book returnedBook;  ///< Book being returned.
    std::string reason; ///< Customer return reason.
    bool approved;      ///< Approval status flag.
public:
    /**
     * @brief Constructs a ReturnRequest.
     * @param id Return request ID.
     * @param order Original Order.
     * @param book Book to return.
     * @param reason Reason description.
     */
    ReturnRequest(int id, const Order& order, const Book& book, std::string reason);

    /**
     * @brief Approves the return request for refund processing.
     */
    void approveReturn();

    /**
     * @brief Rejects the return request and appends an explanation note.
     * @param note Rejection reason.
     */
    void rejectReturn(const std::string& note);

    bool isApproved() const { return approved; }
    Book getReturnedBook() const { return returnedBook; }
};

/**
 * @class RefundProcessor
 * @brief Executes financial refunds and restocks returned books upon approved ReturnRequest.
 */
class RefundProcessor {
private:
    double processedRefundsTotal; ///< Cumulative refunded monetary amount.
    int refundCount;              ///< Total number of processed refunds.
public:
    RefundProcessor();

    /**
     * @brief Refunds payment and restocks the returned book if the ReturnRequest is approved.
     * @param request Approved ReturnRequest.
     * @param payment Payment transaction to refund.
     * @param inventoryBook Book record to restock.
     * @return Refunded monetary value.
     * @throws UnauthorizedAccessException if request is not approved.
     */
    double executeRefund(ReturnRequest& request, Payment& payment, Book& inventoryBook);

    double getProcessedRefundsTotal() const { return processedRefundsTotal; }
};

/**
 * @class SalesAnalytics
 * @brief Aggregates completed orders to compute sales volume and average order value (AOV).
 */
class SalesAnalytics {
private:
    std::vector<Order> completedOrders; ///< Recorded completed orders.
    double cumulativeSales;             ///< Total sales revenue across recorded orders.
public:
    SalesAnalytics();

    /**
     * @brief Records a completed Order into analytics metrics.
     * @param order Completed Order.
     */
    void recordOrder(const Order& order);

    /**
     * @brief Calculates the Average Order Value (AOV).
     * @return Mean order total (0.0 if no orders recorded).
     */
    double calculateAverageOrderValue() const;

    size_t getOrderVolume() const { return completedOrders.size(); }
};

/**
 * @class PopularityRating
 * @brief Tracks page views and purchase counts to compute a book popularity index.
 */
class PopularityRating {
private:
    std::string isbn;  ///< Target book ISBN.
    int viewCount;     ///< Number of catalog page views.
    int purchaseCount; ///< Number of copies purchased.
public:
    /**
     * @brief Constructs a PopularityRating tracker for an ISBN.
     * @param isbn Book ISBN.
     */
    PopularityRating(std::string isbn = "");

    /**
     * @brief Increments the catalog view counter.
     */
    void recordView();

    /**
     * @brief Adds purchased copies to the purchase counter.
     * @param qty Quantity purchased.
     */
    void recordPurchase(int qty);

    /**
     * @brief Computes a weighted popularity score from views and purchases.
     * @return Weighted popularity index.
     */
    double computePopularityIndex() const;
};

/**
 * @class InventoryReport
 * @brief Generates point-in-time asset valuation snapshots for a Warehouse.
 */
class InventoryReport {
private:
    std::string reportDate;   ///< Report generation date string.
    Warehouse warehouse;      ///< Target Warehouse audited.
    double valuationSnapshot; ///< Captured asset valuation.
public:
    /**
     * @brief Constructs an InventoryReport for a warehouse on a specific date.
     * @param date Report date string.
     * @param wh Target Warehouse.
     */
    InventoryReport(std::string date, const Warehouse& wh);

    /**
     * @brief Captures current total asset valuation from the warehouse.
     */
    void generateSnapshot();

    /**
     * @brief Formats a one-line summary of the inventory valuation snapshot.
     * @return Formatted report summary line.
     */
    std::string formatSummaryLine() const;
};

/**
 * @class FinancialAuditor
 * @brief Audits invoice accuracy against order totals and computes net profit margins.
 */
class FinancialAuditor {
private:
    std::string auditorName;     ///< Auditor identifier or system name.
    double discrepancyTolerance; ///< Allowed floating-point rounding tolerance.
public:
    /**
     * @brief Constructs a FinancialAuditor.
     * @param name Auditor name.
     * @param tolerance Maximum allowable rounding discrepancy.
     */
    FinancialAuditor(std::string name = "AuditBot", double tolerance = 0.01);

    /**
     * @brief Verifies that an Invoice grand total matches its Order total plus tax within tolerance.
     * @param invoice Invoice to audit.
     * @param order Underlying Order.
     * @return True if numbers reconcile within discrepancyTolerance.
     */
    bool verifyInvoiceMatchesOrder(const Invoice& invoice, const Order& order) const;

    /**
     * @brief Calculates net profit margin ratio ((revenue - expenses) / revenue).
     * @param revenue Total revenue.
     * @param expenses Total expenses.
     * @return Net margin ratio (0.0 if revenue <= 0).
     */
    double calculateNetMargin(double revenue, double expenses) const;
};

/**
 * @class ReviewSystem
 * @brief Represents an individual customer or reader review with star rating and text comment.
 */
class ReviewSystem {
private:
    int reviewId;        ///< Unique review ID.
    std::string bookIsbn;///< Reviewed book ISBN.
    std::string authorName; ///< Reviewer name.
    int stars;           ///< Star rating (1..5).
    std::string comment; ///< Textual review body.
public:
    /**
     * @brief Constructs a ReviewSystem entry.
     * @param id Unique review ID.
     * @param isbn Book ISBN.
     * @param author Reviewer name.
     * @param stars Star score.
     * @param comment Review text.
     */
    ReviewSystem(int id = 1, std::string isbn = "", std::string author = "", int stars = 5, std::string comment = "");

    /**
     * @brief Updates the star rating and comment text of the review.
     * @param newStars Updated star count.
     * @param newComment Updated comment text.
     */
    void updateRating(int newStars, const std::string& newComment);

    /**
     * @brief Checks whether the review is considered positive (4 or more stars).
     * @return True if stars >= 4.
     */
    bool isPositiveReview() const;

    int getStars() const { return stars; }
    int getReviewId() const { return reviewId; }
};

/**
 * @class Rating
 * @brief Aggregates all ReviewSystem entries for a book and calculates its average rating.
 */
class Rating {
private:
    std::string bookIsbn;              ///< Book ISBN being rated.
    std::vector<ReviewSystem> reviews; ///< Collection of reviews for the book.
public:
    /**
     * @brief Constructs a Rating aggregator for a book ISBN.
     * @param isbn Book ISBN.
     */
    Rating(std::string isbn = "");

    /**
     * @brief Appends a new review to the rating collection.
     * @param review ReviewSystem instance to add.
     */
    void addReview(const ReviewSystem& review);

    /**
     * @brief Removes a review by its unique reviewId.
     * @param reviewId ID of the review to remove.
     * @throws ReviewNotFoundException if reviewId does not exist.
     */
    void removeReviewById(int reviewId);

    /**
     * @brief Calculates the arithmetic mean of star ratings across all reviews.
     * @return Average star score (0.0 if no reviews exist).
     */
    double calculateAverageStars() const;
};

/**
 * @class RecommendationEngine
 * @brief Provides book recommendations based on genre preferences and minimum rating thresholds.
 */
class RecommendationEngine {
private:
    std::string strategyName;   ///< Recommendation algorithm name.
    double minScoreThreshold;   ///< Minimum average star score to recommend a title.
public:
    /**
     * @brief Constructs a RecommendationEngine.
     * @param strategy Algorithm strategy label.
     * @param threshold Minimum star rating threshold.
     */
    RecommendationEngine(std::string strategy = "GenreMatch", double threshold = 4.0);

    /**
     * @brief Queries the catalog for recommended books matching a preferred Genre.
     * @param catalog Source CatalogManager.
     * @param preferred Preferred Genre.
     * @return Vector of recommended Book objects.
     */
    std::vector<Book> recommendByGenre(const CatalogManager& catalog, const Genre& preferred) const;

    /**
     * @brief Evaluates whether a book's Rating meets the minimum recommendation threshold.
     * @param rating Aggregated Rating object.
     * @return True if average stars >= minScoreThreshold.
     */
    bool shouldRecommendRating(const Rating& rating) const;
};

/**
 * @class ActivityLog
 * @brief Bounded audit log recording user actions across the library and bookstore system.
 */
class ActivityLog {
private:
    std::vector<std::string> entries; ///< Chronological log entries.
    int maxCapacity;                  ///< Maximum retained entries before oldest is evicted.
public:
    /**
     * @brief Constructs an ActivityLog with a maximum entry capacity.
     * @param capacity Maximum number of entries to retain.
     */
    ActivityLog(int capacity = 500);

    /**
     * @brief Records a formatted action entry for a User, evicting oldest if at capacity.
     * @param user Acting User.
     * @param action Description of the action performed.
     */
    void logUserAction(const User& user, const std::string& action);

    /**
     * @brief Retrieves the most recently logged activity entry.
     * @return Latest entry string, or "No activity" if empty.
     */
    std::string getLatestEntry() const;

    size_t countEntries() const { return entries.size(); }
};