/**
 * @file operations.cpp
 * @brief Implementation of Staff, HR, Partners, Live Sessions, Refunds, Analytics, Reviews, and Audit.
 */

#include "operations.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <utility>

StaffMember::StaffMember(int id, std::string name, std::string role, double salary, ContactAddress addr)
    : staffId(id), name(std::move(name)), role(std::move(role)), monthlySalary(salary), address(std::move(addr)) {}

void StaffMember::promote(const std::string& newRole, double raiseAmount) {
    role = newRole;
    if (raiseAmount > 0.0) monthlySalary += raiseAmount;
}

double StaffMember::calculateAnnualSalary() const {
    return monthlySalary * 12.0;
}

bool StaffMember::hasPermission(const std::string& requiredRole) const {
    return role == requiredRole || role == "Administrator";
}

Mentor::Mentor(const StaffMember& base, std::string area)
    : StaffMember(base), trackArea(std::move(area)), sessionsConducted(0) {}

void Mentor::assignCourseToLearner(StudentLearner& student, Course& course) {
    student.enrollCourse(course);
    sessionsConducted++;
}

void Mentor::unenrollLearnerFromCourse(StudentLearner& student, Course& course) {
    student.dropCourse(course);
    sessionsConducted++;
}

CurriculumEditor::CurriculumEditor(const StaffMember& base, std::string standard)
    : StaffMember(base), standardCode(std::move(standard)), publishedCount(0) {}

void CurriculumEditor::publishCourse(CourseCatalog& catalog, const Course& course) {
    catalog.addCourse(course);
    publishedCount++;
}

std::string CurriculumEditor::generateIndexCode(const Course& course) const {
    return standardCode + "-" + course.getCode();
}

BillingSpecialist::BillingSpecialist(const StaffMember& base, int termId, double openingCash)
    : StaffMember(base), terminalId(termId), cashCollected(openingCash) {}

void BillingSpecialist::recordCashTuition(double amount) {
    if (amount < 0.0) throw InvalidBillingException("Cash amount cannot be negative.");
    cashCollected += amount;
}

double BillingSpecialist::reconcileShift(double countedCash) const {
    return countedCash - cashCollected;
}

CourseDirector::CourseDirector(const StaffMember& base, std::string dept, double ceiling)
    : StaffMember(base), department(std::move(dept)), approvalCeiling(ceiling) {}

bool CourseDirector::approveEnrollment(const Enrollment& enrollment) const {
    return enrollment.calculateNetTotal() <= approvalCeiling;
}

void CourseDirector::applyOverrideCoupon(Enrollment& enrollment, const Coupon& coupon) const {
    enrollment.applyCoupon(coupon);
}

PlatformAdmin::PlatformAdmin(const StaffMember& base, int level, bool mfa)
    : StaffMember(base), clearanceLevel(level), mfaEnabled(mfa) {}

void PlatformAdmin::freezeStudyPass(StudyPass& pass) const {
    if (!mfaEnabled) throw AccessDeniedException("PlatformAdmin");
    pass.setFrozen(true);
}

void PlatformAdmin::unfreezeStudyPass(StudyPass& pass) const {
    if (!mfaEnabled) throw AccessDeniedException("PlatformAdmin");
    pass.setFrozen(false);
}

SessionSchedule::SessionSchedule(int staffId, std::string day, int start, int end)
    : staffId(staffId), dayOfWeek(std::move(day)), startHour(start), endHour(end) {}

int SessionSchedule::calculateHours() const {
    return endHour >= startHour ? (endHour - startHour) : 0;
}

bool SessionSchedule::overlapsWith(const SessionSchedule& other) const {
    return dayOfWeek == other.dayOfWeek && startHour < other.endHour && other.startHour < endHour;
}

PayrollRecord::PayrollRecord(int staffId, double gross, double tax, double bonus)
    : staffId(staffId), grossPay(gross), taxWithheld(tax), bonusPay(bonus) {}

double PayrollRecord::computeNetPay() const {
    return grossPay - taxWithheld + bonusPay;
}

void PayrollRecord::addBonus(double amount) {
    if (amount > 0.0) bonusPay += amount;
}

void HRManager::hireStaff(const StaffMember& member) {
    team.push_back(member);
}

void HRManager::scheduleShift(const SessionSchedule& schedule) {
    shifts.push_back(schedule);
}

double HRManager::calculateMonthlyPayroll() const {
    double total = 0.0;
    for (const auto& m : team) {
        total += m.calculateAnnualSalary() / 12.0;
    }
    return total;
}

ContentPartner::ContentPartner(std::string id, std::string name, double quality, int days)
    : partnerId(std::move(id)), partnerName(std::move(name)), qualityScore(quality), onboardingDays(days) {}

bool ContentPartner::isPreferredPartner() const {
    return qualityScore >= 0.90 && onboardingDays <= 5;
}

void ContentPartner::updateQualityScore(bool deliveredOnTime) {
    qualityScore = deliveredOnTime
        ? std::min(1.0, qualityScore + 0.02)
        : std::max(0.0, qualityScore - 0.05);
}

ContentBatch::ContentBatch(std::string id, const ContentPartner& partner, const Classroom& room, int count)
    : batchId(std::move(id)), partner(partner), targetRoom(room), lessonCount(count), imported(false) {}

void ContentBatch::dispatchBatch() {
    if (lessonCount <= 0) throw LiveSessionException("Cannot dispatch empty content batch.");
}

void ContentBatch::completeImport(const Lesson& lesson) {
    targetRoom.scheduleLesson(lesson);
    imported = true;
}

LiveSession::LiveSession(std::string id, std::string topic, std::string format)
    : sessionId(std::move(id)), topic(std::move(topic)), formatType(std::move(format)), openForBooking(true) {}

void LiveSession::bookForEnrollment(const Enrollment& enrollment) {
    if (!openForBooking) throw LiveSessionException("Session is already booked.");
    if (enrollment.getStatus() == "Cancelled") throw LiveSessionException("Cannot book for cancelled enrollment.");
    openForBooking = false;
}

void LiveSession::releaseSession() {
    openForBooking = true;
}

StudySchedule::StudySchedule(std::string code, const LiveSession& session, double hours)
    : scheduleCode(std::move(code)), primarySession(session), estimatedHours(hours) {}

void StudySchedule::addHub(const ContactAddress& addr) {
    hubs.push_back(addr);
}

double StudySchedule::estimateMentorCost(double hourlyRate) const {
    return estimatedHours * hourlyRate;
}

ProgressTracker::ProgressTracker(std::string code)
    : trackerCode(std::move(code)), currentModule("Orientation"), statusNote("Started") {}

void ProgressTracker::updateMilestone(const std::string& module, const std::string& note) {
    currentModule = module;
    statusNote = note;
}

std::string ProgressTracker::formatSummary() const {
    return "[" + trackerCode + "] " + currentModule + ": " + statusNote;
}

RefundRequest::RefundRequest(int id, const Enrollment& enrollment, const Course& course, std::string reason)
    : requestId(id), sourceEnrollment(enrollment), targetCourse(course), reason(std::move(reason)), approved(false) {}

void RefundRequest::approve() {
    approved = true;
}

void RefundRequest::reject(const std::string& note) {
    approved = false;
    reason += " (Rejected: " + note + ")";
}

RefundHandler::RefundHandler() : totalRefunded(0.0), refundCount(0) {}

double RefundHandler::processRefund(RefundRequest& request, BillingTransaction& tx, Course& course) {
    if (!request.isApproved()) {
        throw AccessDeniedException("Refund request not approved.");
    }
    tx.reverseTransaction();
    course.expandSeats(1);
    double refunded = course.getTuitionFee();
    totalRefunded += refunded;
    refundCount++;
    return refunded;
}

EnrollmentAnalytics::EnrollmentAnalytics() : cumulativeRevenue(0.0) {}

void EnrollmentAnalytics::recordEnrollment(const Enrollment& enrollment) {
    enrollments.push_back(enrollment);
    cumulativeRevenue += enrollment.calculateNetTotal();
}

double EnrollmentAnalytics::calculateAverageValue() const {
    return enrollments.empty() ? 0.0 : (cumulativeRevenue / enrollments.size());
}

CourseEngagement::CourseEngagement(std::string code)
    : courseCode(std::move(code)), viewCount(0), completionCount(0) {}

void CourseEngagement::recordView() {
    viewCount++;
}

void CourseEngagement::recordCompletion(int count) {
    if (count > 0) completionCount += count;
}

double CourseEngagement::computeEngagementScore() const {
    return viewCount * 0.2 + completionCount * 3.0;
}

ClassroomAudit::ClassroomAudit(std::string date, const Classroom& room)
    : auditDate(std::move(date)), classroom(room), minutesSnapshot(0) {}

void ClassroomAudit::captureSnapshot() {
    minutesSnapshot = classroom.evaluateActiveModuleMinutes();
}

std::string ClassroomAudit::formatSummaryLine() const {
    return "Audit [" + auditDate + "] Active XP/Minutes: " + std::to_string(minutesSnapshot);
}

FinancialVerifier::FinancialVerifier(std::string name, double tol)
    : verifierName(std::move(name)), tolerance(tol) {}

bool FinancialVerifier::verifyReceipt(const Receipt& receipt, const Enrollment& enrollment) const {
    double expected = enrollment.calculateNetTotal() + receipt.calculateTax();
    double diff = receipt.calculateGrandTotal() - expected;
    return diff >= -tolerance && diff <= tolerance;
}

double FinancialVerifier::calculateNetMargin(double revenue, double costs) const {
    return revenue > 0.0 ? ((revenue - costs) / revenue) : 0.0;
}

CourseReview::CourseReview(int id, std::string code, std::string author, int stars, std::string comment)
    : reviewId(id), courseCode(std::move(code)), authorName(std::move(author)), stars(stars), comment(std::move(comment)) {}

void CourseReview::updateFeedback(int newStars, const std::string& newComment) {
    stars = newStars;
    comment = newComment;
}

bool CourseReview::isPositive() const {
    return stars >= 4;
}

RatingSummary::RatingSummary(std::string code) : courseCode(std::move(code)) {}

void RatingSummary::addReview(const CourseReview& review) {
    reviews.push_back(review);
}

void RatingSummary::removeReviewById(int reviewId) {
    for (auto it = reviews.begin(); it != reviews.end(); ++it) {
        if (it->getReviewId() == reviewId) {
            reviews.erase(it);
            return;
        }
    }
    throw SubmissionNotFoundException(reviewId);
}

double RatingSummary::calculateAverageStars() const {
    if (reviews.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& r : reviews) sum += r.getStars();
    return sum / reviews.size();
}

RecommendationService::RecommendationService(std::string strat, double threshold)
    : strategy(std::move(strat)), minRatingThreshold(threshold) {}

std::vector<Course> RecommendationService::recommendBySkill(const CourseCatalog& catalog, const SkillTag& skill) const {
    return catalog.searchByInstructor(skill.getName());
}

bool RecommendationService::shouldRecommend(const RatingSummary& summary) const {
    return summary.calculateAverageStars() >= minRatingThreshold;
}

AuditTrail::AuditTrail(int maxCapacity) : maxEntries(maxCapacity) {}

void AuditTrail::logAction(const Learner& learner, const std::string& action) {
    if (static_cast<int>(records.size()) >= maxEntries) {
        records.erase(records.begin());
    }
    records.push_back("Learner#" + std::to_string(learner.getLearnerId()) + " (" + learner.getFullName() + "): " + action);
}

std::string AuditTrail::getLatestRecord() const {
    return records.empty() ? "No activity" : records.back();
}
