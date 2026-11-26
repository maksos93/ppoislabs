/**
 * @file operations.hpp
 * @brief Staff, HR, Partners, Live Sessions, Refunds, Analytics, Reviews, and Audit definitions.
 */

#pragma once

#include <string>
#include <vector>
#include "courses.hpp"
#include "learners.hpp"
#include "enrollments.hpp"

/**
 * @class StaffMember
 * @brief Base class for platform employees with role, salary, and address.
 */
class StaffMember {
protected:
    int staffId;            ///< Unique staff ID.
    std::string name;       ///< Full name.
    std::string role;       ///< Role title.
    double monthlySalary;   ///< Monthly salary.
    ContactAddress address; ///< Home address.
public:
    /**
     * @brief Constructs a StaffMember.
     * @param id Staff ID.
     * @param name Full name.
     * @param role Role title.
     * @param salary Monthly salary.
     * @param addr ContactAddress.
     */
    StaffMember(int id = 1, std::string name = "Staff", std::string role = "Support",
                double salary = 2500.0, ContactAddress addr = ContactAddress());

    virtual ~StaffMember() = default;

    int getStaffId() const { return staffId; }
    std::string getName() const { return name; }
    std::string getRole() const { return role; }

    /**
     * @brief Promotes staff member to a new role with a salary raise.
     * @param newRole New role.
     * @param raiseAmount Salary increase (if > 0).
     */
    void promote(const std::string& newRole, double raiseAmount);

    /**
     * @brief Computes annual gross compensation.
     * @return Annual salary.
     */
    double calculateAnnualSalary() const;

    /**
     * @brief Checks whether staff member is authorized for requiredRole.
     * @param requiredRole Required role name.
     * @return True if authorized.
     */
    bool hasPermission(const std::string& requiredRole) const;
};

/**
 * @class Mentor
 * @brief StaffMember who guides learners through courses and reviews progress.
 */
class Mentor : public StaffMember {
private:
    std::string trackArea;   ///< Assigned learning track area.
    int sessionsConducted; ///< Count of mentoring actions.
public:
    /**
     * @brief Constructs a Mentor.
     * @param base Base StaffMember.
     * @param area Track area.
     */
    Mentor(const StaffMember& base, std::string area = "Programming");

    /**
     * @brief Enrolls a StudentLearner into a Course and increments counter.
     * @param student Target StudentLearner.
     * @param course Target Course.
     */
    void assignCourseToLearner(StudentLearner& student, Course& course);

    /**
     * @brief Drops a Course for a StudentLearner and increments counter.
     * @param student Target StudentLearner.
     * @param course Target Course.
     */
    void unenrollLearnerFromCourse(StudentLearner& student, Course& course);

    int getSessionsConducted() const { return sessionsConducted; }
};

/**
 * @class CurriculumEditor
 * @brief StaffMember responsible for publishing courses into the catalog and indexing codes.
 */
class CurriculumEditor : public StaffMember {
private:
    std::string standardCode; ///< Curriculum standard (e.g., ACM, ECTS).
    int publishedCount;       ///< Number of courses published.
public:
    /**
     * @brief Constructs a CurriculumEditor.
     * @param base Base StaffMember.
     * @param standard Standard code.
     */
    CurriculumEditor(const StaffMember& base, std::string standard = "ECTS");

    /**
     * @brief Adds a Course to CourseCatalog and increments publishedCount.
     * @param catalog Target CourseCatalog.
     * @param course Course to publish.
     */
    void publishCourse(CourseCatalog& catalog, const Course& course);

    /**
     * @brief Generates a formatted catalog index code for a Course.
     * @param course Target Course.
     * @return Formatted index code.
     */
    std::string generateIndexCode(const Course& course) const;
};

/**
 * @class BillingSpecialist
 * @brief StaffMember managing cash/invoice settlements and shift reconciliation.
 */
class BillingSpecialist : public StaffMember {
private:
    int terminalId;      ///< Billing terminal ID.
    double cashCollected;///< Collected cash balance.
public:
    /**
     * @brief Constructs a BillingSpecialist.
     * @param base Base StaffMember.
     * @param termId Terminal ID.
     * @param openingCash Initial cash float.
     */
    BillingSpecialist(const StaffMember& base, int termId = 1, double openingCash = 100.0);

    /**
     * @brief Records a cash tuition payment.
     * @param amount Payment amount (must be >= 0).
     * @throws InvalidBillingException if amount < 0.
     */
    void recordCashTuition(double amount);

    /**
     * @brief Computes difference between counted cash and cashCollected.
     * @param countedCash Counted cash.
     * @return Variance amount.
     */
    double reconcileShift(double countedCash) const;
};

/**
 * @class CourseDirector
 * @brief Manager authorized to approve high-value enrollments and override coupons.
 */
class CourseDirector : public StaffMember {
private:
    std::string department;  ///< Department name.
    double approvalCeiling;  ///< Max enrollment amount director can approve.
public:
    /**
     * @brief Constructs a CourseDirector.
     * @param base Base StaffMember.
     * @param dept Department name.
     * @param ceiling Approval ceiling.
     */
    CourseDirector(const StaffMember& base, std::string dept = "Academics", double ceiling = 10000.0);

    /**
     * @brief Checks if an enrollment total is within approvalCeiling.
     * @param enrollment Target Enrollment.
     * @return True if within limit.
     */
    bool approveEnrollment(const Enrollment& enrollment) const;

    /**
     * @brief Overrides coupon discount on an Enrollment.
     * @param enrollment Target Enrollment.
     * @param coupon Coupon to apply.
     */
    void applyOverrideCoupon(Enrollment& enrollment, const Coupon& coupon) const;
};

/**
 * @class PlatformAdmin
 * @brief Administrator with security clearance to freeze or unfreeze a StudyPass.
 */
class PlatformAdmin : public StaffMember {
private:
    int clearanceLevel; ///< Security clearance level.
    bool mfaEnabled;    ///< MFA verification status.
public:
    /**
     * @brief Constructs a PlatformAdmin.
     * @param base Base StaffMember.
     * @param level Clearance level.
     * @param mfa MFA flag.
     */
    PlatformAdmin(const StaffMember& base, int level = 5, bool mfa = true);

    /**
     * @brief Freezes a learner's StudyPass if MFA is enabled.
     * @param pass Target StudyPass.
     * @throws AccessDeniedException if mfaEnabled is false.
     */
    void freezeStudyPass(StudyPass& pass) const;

    /**
     * @brief Unfreezes a learner's StudyPass if MFA is enabled.
     * @param pass Target StudyPass.
     * @throws AccessDeniedException if mfaEnabled is false.
     */
    void unfreezeStudyPass(StudyPass& pass) const;
};

/**
 * @class SessionSchedule
 * @brief Represents a scheduled work/mentoring shift for a staff member.
 */
class SessionSchedule {
private:
    int staffId;          ///< Staff member ID.
    std::string dayOfWeek;///< Day name.
    int startHour;        ///< Start hour (0..23).
    int endHour;          ///< End hour (0..23).
public:
    /**
     * @brief Constructs a SessionSchedule.
     * @param staffId Staff ID.
     * @param day Day of week.
     * @param start Start hour.
     * @param end End hour.
     */
    SessionSchedule(int staffId = 1, std::string day = "Monday", int start = 9, int end = 17);

    /**
     * @brief Calculates shift duration in hours.
     * @return Duration in hours.
     */
    int calculateHours() const;

    /**
     * @brief Checks whether this schedule overlaps with another on the same day.
     * @param other Other SessionSchedule.
     * @return True if overlapping.
     */
    bool overlapsWith(const SessionSchedule& other) const;
};

/**
 * @class PayrollRecord
 * @brief Represents staff payroll computation with gross pay, tax, and bonus.
 */
class PayrollRecord {
private:
    int staffId;       ///< Staff ID.
    double grossPay;   ///< Gross pay.
    double taxWithheld;///< Tax deduction.
    double bonusPay;   ///< Bonus amount.
public:
    /**
     * @brief Constructs a PayrollRecord.
     * @param staffId Staff ID.
     * @param gross Gross pay.
     * @param tax Tax withheld.
     * @param bonus Bonus pay.
     */
    PayrollRecord(int staffId = 1, double gross = 2500.0, double tax = 350.0, double bonus = 0.0);

    /**
     * @brief Calculates net pay (grossPay - taxWithheld + bonusPay).
     * @return Net pay.
     */
    double computeNetPay() const;

    /**
     * @brief Adds a positive bonus to the payroll record.
     * @param amount Bonus amount.
     */
    void addBonus(double amount);
};

/**
 * @class HRManager
 * @brief Manages staff roster, shift schedules, and monthly payroll.
 */
class HRManager {
private:
    std::vector<StaffMember> team;        ///< Staff roster.
    std::vector<SessionSchedule> shifts;  ///< Assigned shifts.
public:
    HRManager() = default;

    /**
     * @brief Adds a StaffMember to the team.
     * @param member StaffMember to hire.
     */
    void hireStaff(const StaffMember& member);

    /**
     * @brief Assigns a SessionSchedule.
     * @param schedule SessionSchedule to add.
     */
    void scheduleShift(const SessionSchedule& schedule);

    /**
     * @brief Computes total monthly base payroll across team members.
     * @return Monthly payroll total.
     */
    double calculateMonthlyPayroll() const;

    size_t getTeamSize() const { return team.size(); }
};

/**
 * @class ContentPartner
 * @brief External content provider supplying course batches.
 */
class ContentPartner {
private:
    std::string partnerId;    ///< Partner ID.
    std::string partnerName;  ///< Partner name.
    double qualityScore;    ///< Quality rating in [0.0, 1.0].
    int onboardingDays;     ///< Onboarding lead time in days.
public:
    /**
     * @brief Constructs a ContentPartner.
     * @param id Partner ID.
     * @param name Partner name.
     * @param quality Quality rating [0.0..1.0].
     * @param days Onboarding lead time.
     */
    ContentPartner(std::string id = "PRT-1", std::string name = "EduStudio", double quality = 0.95, int days = 4);

    /**
     * @brief Checks whether partner meets preferred SLA (quality >= 0.90, days <= 5).
     * @return True if preferred.
     */
    bool isPreferredPartner() const;

    /**
     * @brief Updates qualityScore based on timely delivery.
     * @param deliveredOnTime True if delivered on time.
     */
    void updateQualityScore(bool deliveredOnTime);
};

/**
 * @class ContentBatch
 * @brief Tracks an incoming course content batch from a ContentPartner to a Classroom.
 */
class ContentBatch {
private:
    std::string batchId;      ///< Batch ID.
    ContentPartner partner;   ///< Source partner.
    Classroom targetRoom;     ///< Destination Classroom.
    int lessonCount;          ///< Number of lessons in batch.
    bool imported;            ///< Import status flag.
public:
    /**
     * @brief Constructs a ContentBatch.
     * @param id Batch ID.
     * @param partner ContentPartner.
     * @param room Target Classroom.
     * @param count Lesson count.
     */
    ContentBatch(std::string id, const ContentPartner& partner, const Classroom& room, int count);

    /**
     * @brief Validates and dispatches the batch for import.
     * @throws LiveSessionException if lessonCount <= 0.
     */
    void dispatchBatch();

    /**
     * @brief Completes batch import and schedules a Lesson into targetRoom.
     * @param lesson Lesson to schedule.
     */
    void completeImport(const Lesson& lesson);

    bool isImported() const { return imported; }
};

/**
 * @class LiveSession
 * @brief Represents a live webinar or mentoring session assigned to a single enrollment.
 */
class LiveSession {
private:
    std::string sessionId;  ///< Session ID.
    std::string topic;      ///< Session topic.
    std::string formatType; ///< Format (e.g., Webinar, Workshop).
    bool openForBooking;    ///< Booking availability flag.
public:
    /**
     * @brief Constructs a LiveSession.
     * @param id Session ID.
     * @param topic Topic title.
     * @param format Format string.
     */
    LiveSession(std::string id = "LIVE-01", std::string topic = "OOP Design", std::string format = "Workshop");

    /**
     * @brief Books the session for an active Enrollment.
     * @param enrollment Target Enrollment.
     * @throws LiveSessionException if already booked or enrollment is cancelled.
     */
    void bookForEnrollment(const Enrollment& enrollment);

    /**
     * @brief Reopens the session for booking after completion.
     */
    void releaseSession();

    bool isOpenForBooking() const { return openForBooking; }
};

/**
 * @class StudySchedule
 * @brief Multi-stop or multi-module study route assigned to a LiveSession/track.
 */
class StudySchedule {
private:
    std::string scheduleCode;       ///< Schedule code.
    LiveSession primarySession;     ///< Associated LiveSession.
    std::vector<ContactAddress> hubs; ///< Study hubs or regional centers.
    double estimatedHours;          ///< Estimated total study hours.
public:
    /**
     * @brief Constructs a StudySchedule.
     * @param code Schedule code.
     * @param session LiveSession.
     * @param hours Estimated study hours.
     */
    StudySchedule(std::string code, const LiveSession& session, double hours = 20.0);

    /**
     * @brief Adds a study hub ContactAddress.
     * @param addr Hub address.
     */
    void addHub(const ContactAddress& addr);

    /**
     * @brief Estimates mentor cost given hourly rate.
     * @param hourlyRate Rate per hour.
     * @return Estimated cost.
     */
    double estimateMentorCost(double hourlyRate) const;
};

/**
 * @class ProgressTracker
 * @brief Tracks milestone checkpoints and current status for a learner's path.
 */
class ProgressTracker {
private:
    std::string trackerCode;   ///< Tracker code.
    std::string currentModule; ///< Current module name.
    std::string statusNote;    ///< Progress note.
public:
    /**
     * @brief Constructs a ProgressTracker.
     * @param code Tracker code.
     */
    ProgressTracker(std::string code = "PRG-001");

    /**
     * @brief Updates current module and status note.
     * @param module Module name.
     * @param note Status note.
     */
    void updateMilestone(const std::string& module, const std::string& note);

    /**
     * @brief Formats a summary string of current progress.
     * @return Formatted summary.
     */
    std::string formatSummary() const;
};

/**
 * @class RefundRequest
 * @brief Represents a learner's request for a tuition refund on a Course.
 */
class RefundRequest {
private:
    int requestId;             ///< Refund request ID.
    Enrollment sourceEnrollment; ///< Source Enrollment.
    Course targetCourse;       ///< Course being refunded.
    std::string reason;        ///< Reason for refund.
    bool approved;             ///< Approval flag.
public:
    /**
     * @brief Constructs a RefundRequest.
     * @param id Request ID.
     * @param enrollment Enrollment.
     * @param course Course.
     * @param reason Refund reason.
     */
    RefundRequest(int id, const Enrollment& enrollment, const Course& course, std::string reason);

    /**
     * @brief Approves the refund request.
     */
    void approve();

    /**
     * @brief Rejects the refund request with a note.
     * @param note Rejection note.
     */
    void reject(const std::string& note);

    bool isApproved() const { return approved; }
    Course getTargetCourse() const { return targetCourse; }
};

/**
 * @class RefundHandler
 * @brief Processes approved refunds, reverses transactions, and restores course seats.
 */
class RefundHandler {
private:
    double totalRefunded; ///< Cumulative refunded amount.
    int refundCount;    ///< Count of processed refunds.
public:
    RefundHandler();

    /**
     * @brief Executes refund if RefundRequest is approved.
     * @param request Approved RefundRequest.
     * @param tx BillingTransaction to reverse.
     * @param course Course to restore seats on.
     * @return Refunded tuition fee.
     * @throws AccessDeniedException if request is not approved.
     */
    double processRefund(RefundRequest& request, BillingTransaction& tx, Course& course);

    double getTotalRefunded() const { return totalRefunded; }
};

/**
 * @class EnrollmentAnalytics
 * @brief Aggregates confirmed enrollments to compute volume and average enrollment value.
 */
class EnrollmentAnalytics {
private:
    std::vector<Enrollment> enrollments; ///< Recorded enrollments.
    double cumulativeRevenue;          ///< Total revenue across enrollments.
public:
    EnrollmentAnalytics();

    /**
     * @brief Records an Enrollment into analytics.
     * @param enrollment Target Enrollment.
     */
    void recordEnrollment(const Enrollment& enrollment);

    /**
     * @brief Calculates average enrollment revenue.
     * @return Average value (0.0 if empty).
     */
    double calculateAverageValue() const;

    size_t getEnrollmentCount() const { return enrollments.size(); }
};

/**
 * @class CourseEngagement
 * @brief Tracks lesson views and completed enrollments to compute an engagement score.
 */
class CourseEngagement {
private:
    std::string courseCode; ///< Course code.
    int viewCount;          ///< Lesson/page views.
    int completionCount;    ///< Completed learners count.
public:
    /**
     * @brief Constructs a CourseEngagement tracker.
     * @param code Course code.
     */
    CourseEngagement(std::string code = "");

    /**
     * @brief Increments view counter.
     */
    void recordView();

    /**
     * @brief Adds completed learners to completionCount.
     * @param count Count to add.
     */
    void recordCompletion(int count);

    /**
     * @brief Computes weighted engagement score.
     * @return Engagement score.
     */
    double computeEngagementScore() const;
};

/**
 * @class ClassroomAudit
 * @brief Captures a curriculum/module duration snapshot for a Classroom.
 */
class ClassroomAudit {
private:
    std::string auditDate;  ///< Audit date string.
    Classroom classroom;    ///< Audited Classroom.
    int minutesSnapshot;    ///< Captured minutes snapshot.
public:
    /**
     * @brief Constructs a ClassroomAudit.
     * @param date Audit date.
     * @param room Target Classroom.
     */
    ClassroomAudit(std::string date, const Classroom& room);

    /**
     * @brief Captures current active module minutes from the classroom.
     */
    void captureSnapshot();

    /**
     * @brief Formats a summary line for the audit.
     * @return Formatted line.
     */
    std::string formatSummaryLine() const;
};

/**
 * @class FinancialVerifier
 * @brief Audits receipts against enrollments and calculates net profit margins.
 */
class FinancialVerifier {
private:
    std::string verifierName; ///< Verifier name.
    double tolerance;       ///< Allowed rounding tolerance.
public:
    /**
     * @brief Constructs a FinancialVerifier.
     * @param name Verifier name.
     * @param tol Tolerance value.
     */
    FinancialVerifier(std::string name = "AuditEngine", double tol = 0.01);

    /**
     * @brief Verifies whether Receipt grand total matches Enrollment net total + tax.
     * @param receipt Receipt to verify.
     * @param enrollment Associated Enrollment.
     * @return True if within tolerance.
     */
    bool verifyReceipt(const Receipt& receipt, const Enrollment& enrollment) const;

    /**
     * @brief Calculates net profit margin ((revenue - costs) / revenue).
     * @param revenue Gross revenue.
     * @param costs Total costs.
     * @return Net margin ratio.
     */
    double calculateNetMargin(double revenue, double costs) const;
};

/**
 * @class CourseReview
 * @brief Represents an individual learner review with star rating and feedback comment.
 */
class CourseReview {
private:
    int reviewId;           ///< Unique review ID.
    std::string courseCode; ///< Reviewed course code.
    std::string authorName; ///< Reviewer name.
    int stars;              ///< Star rating (1..5).
    std::string comment;    ///< Feedback comment.
public:
    /**
     * @brief Constructs a CourseReview.
     * @param id Review ID.
     * @param code Course code.
     * @param author Author name.
     * @param stars Star score.
     * @param comment Comment text.
     */
    CourseReview(int id = 1, std::string code = "", std::string author = "", int stars = 5, std::string comment = "");

    /**
     * @brief Updates star rating and comment.
     * @param newStars New stars.
     * @param newComment New comment.
     */
    void updateFeedback(int newStars, const std::string& newComment);

    /**
     * @brief Checks whether review is positive (stars >= 4).
     * @return True if positive.
     */
    bool isPositive() const;

    int getStars() const { return stars; }
    int getReviewId() const { return reviewId; }
};

/**
 * @class RatingSummary
 * @brief Aggregates CourseReview items for a course and computes average stars.
 */
class RatingSummary {
private:
    std::string courseCode;          ///< Course code.
    std::vector<CourseReview> reviews; ///< Reviews list.
public:
    /**
     * @brief Constructs a RatingSummary.
     * @param code Course code.
     */
    RatingSummary(std::string code = "");

    /**
     * @brief Adds a CourseReview.
     * @param review CourseReview to add.
     */
    void addReview(const CourseReview& review);

    /**
     * @brief Removes a review by ID.
     * @param reviewId Target review ID.
     * @throws SubmissionNotFoundException if reviewId is not found.
     */
    void removeReviewById(int reviewId);

    /**
     * @brief Computes average star rating across reviews.
     * @return Average stars (0.0 if empty).
     */
    double calculateAverageStars() const;
};

/**
 * @class RecommendationService
 * @brief Recommends courses by instructor/skill and evaluates rating thresholds.
 */
class RecommendationService {
private:
    std::string strategy;     ///< Strategy name.
    double minRatingThreshold;///< Minimum average stars to recommend.
public:
    /**
     * @brief Constructs a RecommendationService.
     * @param strat Strategy name.
     * @param threshold Min star threshold.
     */
    RecommendationService(std::string strat = "SkillMatch", double threshold = 4.0);

    /**
     * @brief Searches catalog for courses matching a given SkillTag name.
     * @param catalog Source CourseCatalog.
     * @param skill Preferred SkillTag.
     * @return Vector of matching courses.
     */
    std::vector<Course> recommendBySkill(const CourseCatalog& catalog, const SkillTag& skill) const;

    /**
     * @brief Checks whether a RatingSummary meets minRatingThreshold.
     * @param summary RatingSummary to check.
     * @return True if average >= minRatingThreshold.
     */
    bool shouldRecommend(const RatingSummary& summary) const;
};

/**
 * @class AuditTrail
 * @brief Bounded activity log recording learner actions across the platform.
 */
class AuditTrail {
private:
    std::vector<std::string> records; ///< Log entries.
    int maxEntries;                   ///< Max retained entries.
public:
    /**
     * @brief Constructs an AuditTrail with capacity limit.
     * @param maxCapacity Max entries.
     */
    AuditTrail(int maxCapacity = 500);

    /**
     * @brief Logs a learner action, evicting oldest entry if at capacity.
     * @param learner Acting Learner.
     * @param action Action description.
     */
    void logAction(const Learner& learner, const std::string& action);

    /**
     * @brief Returns the most recent log entry.
     * @return Latest entry or "No activity".
     */
    std::string getLatestRecord() const;

    size_t countRecords() const { return records.size(); }
};
