/**
 * @file staff.cpp
 * @brief Implementation of Staff, HR, Supply Chain, Delivery, Analytics, and Reviews.
 */

#include "staff.hpp"
#include "exceptions.hpp"
#include <utility>

Employee::Employee(int id, std::string name, std::string role, double salary, Address addr)
    : employeeId(id), name(std::move(name)), role(std::move(role)), baseSalary(salary), homeAddress(std::move(addr)) {}

void Employee::promote(const std::string& newRole, double raiseAmount) {
    role = newRole;
    if (raiseAmount > 0.0) baseSalary += raiseAmount;
}

double Employee::calculateAnnualCompensation() const {
    return baseSalary * 12.0;
}

bool Employee::authorizeAction(const std::string& requiredRole) const {
    return role == requiredRole || role == "Administrator";
}

Librarian::Librarian(const Employee& base, std::string zone)
    : Employee(base), deskZone(std::move(zone)), booksProcessed(0) {}

void Librarian::issueBookToReader(Reader& reader, Book& book) {
    reader.borrowBook(book);
    booksProcessed++;
}

void Librarian::acceptReturnedBook(Reader& reader, Book& book) {
    reader.returnBook(book);
    booksProcessed++;
}

Cataloger::Cataloger(const Employee& base, std::string standard)
    : Employee(base), classificationStandard(std::move(standard)), recordsCreated(0) {}

void Cataloger::indexNewBook(CatalogManager& catalog, const Book& book) {
    catalog.addBook(book);
    recordsCreated++;
}

std::string Cataloger::generateCallNumber(const Book& book) const {
    return classificationStandard + "-" + book.getIsbn();
}

Cashier::Cashier(const Employee& base, int regNum, double openingFloat)
    : Employee(base), registerNumber(regNum), drawerBalance(openingFloat) {}

void Cashier::recordCashSale(double amount) {
    if (amount < 0.0) throw InvalidPaymentException("Negative cash sale.");
    drawerBalance += amount;
}

double Cashier::closeShiftReconciliation(double countedCash) const {
    return countedCash - drawerBalance;
}

Manager::Manager(const Employee& base, std::string dept, double limit)
    : Employee(base), departmentName(std::move(dept)), approvalLimit(limit) {}

bool Manager::approveHighValueOrder(const Order& order) const {
    return order.calculateTotal() <= approvalLimit;
}

void Manager::overrideOrderDiscount(Order& order, const Discount& discount) const {
    order.applyDiscount(discount);
}

Administrator::Administrator(const Employee& base, int clearance, bool twoFactor)
    : Employee(base), securityClearance(clearance), twoFactorActive(twoFactor) {}

void Administrator::suspendReaderCard(MembershipCard& card) const {
    if (!twoFactorActive) throw UnauthorizedAccessException("Administrator");
    card.setSuspended(true);
}

void Administrator::restoreReaderCard(MembershipCard& card) const {
    if (!twoFactorActive) throw UnauthorizedAccessException("Administrator");
    card.setSuspended(false);
}

ShiftSchedule::ShiftSchedule(int empId, std::string day, int start, int end)
    : employeeId(empId), shiftDay(std::move(day)), startHour(start), endHour(end) {}

int ShiftSchedule::calculateShiftDuration() const {
    return endHour >= startHour ? (endHour - startHour) : 0;
}

bool ShiftSchedule::overlapsWith(const ShiftSchedule& other) const {
    return shiftDay == other.shiftDay && startHour < other.endHour && other.startHour < endHour;
}

Salary::Salary(int empId, double gross, double tax, double bonus)
    : employeeId(empId), grossPay(gross), taxDeduction(tax), bonus(bonus) {}

double Salary::computeNetPay() const {
    return grossPay - taxDeduction + bonus;
}

void Salary::addPerformanceBonus(double amount) {
    if (amount > 0.0) bonus += amount;
}

void HRDepartment::hireEmployee(const Employee& emp) {
    staff.push_back(emp);
}

void HRDepartment::assignShift(const ShiftSchedule& schedule) {
    schedules.push_back(schedule);
}

double HRDepartment::calculateMonthlyPayroll() const {
    double total = 0.0;
    for (const auto& e : staff) {
        total += e.calculateAnnualCompensation() / 12.0;
    }
    return total;
}

Supplier::Supplier(std::string id, std::string name, double score, int lead)
    : supplierId(std::move(id)), companyName(std::move(name)), reliabilityScore(score), leadTimeDays(lead) {}

bool Supplier::isPreferredSupplier() const {
    return reliabilityScore >= 0.90 && leadTimeDays <= 5;
}

void Supplier::updateReliability(bool deliveredOnTime) {
    reliabilityScore = deliveredOnTime
        ? std::min(1.0, reliabilityScore + 0.02)
        : std::max(0.0, reliabilityScore - 0.05);
}

SupplyShipment::SupplyShipment(std::string id, const Supplier& sup, const Warehouse& dest, int units)
    : shipmentId(std::move(id)), supplier(sup), destination(dest), totalUnits(units), received(false) {}

void SupplyShipment::dispatchShipment() {
    if (totalUnits <= 0) throw DeliveryException("Cannot dispatch empty shipment.");
}

void SupplyShipment::completeDelivery(const InventoryItem& item) {
    destination.receiveInventory(item);
    received = true;
}

Courier::Courier(std::string id, std::string name, std::string vehicle)
    : courierId(std::move(id)), fullName(std::move(name)), vehicleType(std::move(vehicle)), available(true) {}

void Courier::assignOrder(const Order& order) {
    if (!available) throw DeliveryException("Courier is busy.");
    if (order.getStatus() == "Cancelled") throw DeliveryException("Cannot deliver cancelled order.");
    available = false;
}

void Courier::completeDropoff() {
    available = true;
}

DeliveryRoute::DeliveryRoute(std::string code, const Courier& courier, double km)
    : routeCode(std::move(code)), assignedCourier(courier), estimatedKm(km) {}

void DeliveryRoute::addStop(const Address& addr) {
    stops.push_back(addr);
}

double DeliveryRoute::estimateFuelCost(double costPerKm) const {
    return estimatedKm * costPerKm;
}

TrackingSystem::TrackingSystem(std::string trackNum)
    : trackingNumber(std::move(trackNum)), currentLocation("Warehouse"), statusNote("Label Created") {}

void TrackingSystem::updateCheckpoint(const std::string& location, const std::string& note) {
    currentLocation = location;
    statusNote = note;
}

std::string TrackingSystem::formatTrackingSummary() const {
    return "[" + trackingNumber + "] " + currentLocation + ": " + statusNote;
}

ReturnRequest::ReturnRequest(int id, const Order& order, const Book& book, std::string reason)
    : requestId(id), originalOrder(order), returnedBook(book), reason(std::move(reason)), approved(false) {}

void ReturnRequest::approveReturn() {
    approved = true;
}

void ReturnRequest::rejectReturn(const std::string& note) {
    approved = false;
    reason += " (Rejected: " + note + ")";
}

RefundProcessor::RefundProcessor() : processedRefundsTotal(0.0), refundCount(0) {}

double RefundProcessor::executeRefund(ReturnRequest& request, Payment& payment, Book& inventoryBook) {
    if (!request.isApproved()) {
        throw UnauthorizedAccessException("Return request not approved.");
    }
    payment.refundPayment();
    inventoryBook.restock(1);
    double refunded = inventoryBook.getPrice();
    processedRefundsTotal += refunded;
    refundCount++;
    return refunded;
}

SalesAnalytics::SalesAnalytics() : cumulativeSales(0.0) {}

void SalesAnalytics::recordOrder(const Order& order) {
    completedOrders.push_back(order);
    cumulativeSales += order.calculateTotal();
}

double SalesAnalytics::calculateAverageOrderValue() const {
    return completedOrders.empty() ? 0.0 : (cumulativeSales / completedOrders.size());
}

PopularityRating::PopularityRating(std::string isbn)
    : isbn(std::move(isbn)), viewCount(0), purchaseCount(0) {}

void PopularityRating::recordView() {
    viewCount++;
}

void PopularityRating::recordPurchase(int qty) {
    if (qty > 0) purchaseCount += qty;
}

double PopularityRating::computePopularityIndex() const {
    return viewCount * 0.2 + purchaseCount * 2.5;
}

InventoryReport::InventoryReport(std::string date, const Warehouse& wh)
    : reportDate(std::move(date)), warehouse(wh), valuationSnapshot(0.0) {}

void InventoryReport::generateSnapshot() {
    valuationSnapshot = warehouse.evaluateTotalAssetValue();
}

std::string InventoryReport::formatSummaryLine() const {
    return "Report [" + reportDate + "] Valuation: $" + std::to_string(valuationSnapshot);
}

FinancialAuditor::FinancialAuditor(std::string name, double tolerance)
    : auditorName(std::move(name)), discrepancyTolerance(tolerance) {}

bool FinancialAuditor::verifyInvoiceMatchesOrder(const Invoice& invoice, const Order& order) const {
    double expected = order.calculateTotal() + invoice.calculateTaxAmount();
    double diff = invoice.calculateGrandTotal() - expected;
    return diff >= -discrepancyTolerance && diff <= discrepancyTolerance;
}

double FinancialAuditor::calculateNetMargin(double revenue, double expenses) const {
    return revenue > 0.0 ? ((revenue - expenses) / revenue) : 0.0;
}

ReviewSystem::ReviewSystem(int id, std::string isbn, std::string author, int stars, std::string comment)
    : reviewId(id), bookIsbn(std::move(isbn)), authorName(std::move(author)), stars(stars), comment(std::move(comment)) {}

void ReviewSystem::updateRating(int newStars, const std::string& newComment) {
    stars = newStars;
    comment = newComment;
}

bool ReviewSystem::isPositiveReview() const {
    return stars >= 4;
}

Rating::Rating(std::string isbn) : bookIsbn(std::move(isbn)) {}

void Rating::addReview(const ReviewSystem& review) {
    reviews.push_back(review);
}

void Rating::removeReviewById(int reviewId) {
    for (auto it = reviews.begin(); it != reviews.end(); ++it) {
        if (it->getReviewId() == reviewId) {
            reviews.erase(it);
            return;
        }
    }
    throw ReviewNotFoundException(reviewId);
}

double Rating::calculateAverageStars() const {
    if (reviews.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& r : reviews) sum += r.getStars();
    return sum / reviews.size();
}

RecommendationEngine::RecommendationEngine(std::string strategy, double threshold)
    : strategyName(std::move(strategy)), minScoreThreshold(threshold) {}

std::vector<Book> RecommendationEngine::recommendByGenre(const CatalogManager& catalog, const Genre& preferred) const {
    return catalog.searchByAuthor(preferred.getName());
}

bool RecommendationEngine::shouldRecommendRating(const Rating& rating) const {
    return rating.calculateAverageStars() >= minScoreThreshold;
}

ActivityLog::ActivityLog(int capacity) : maxCapacity(capacity) {}

void ActivityLog::logUserAction(const User& user, const std::string& action) {
    if (static_cast<int>(entries.size()) >= maxCapacity) {
        entries.erase(entries.begin());
    }
    entries.push_back("User#" + std::to_string(user.getUserId()) + " (" + user.getFullName() + "): " + action);
}

std::string ActivityLog::getLatestEntry() const {
    return entries.empty() ? "No activity" : entries.back();
}
