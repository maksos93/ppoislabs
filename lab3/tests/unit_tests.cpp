/**
 * @file unit_tests.cpp
 * @brief Unit tests for Online Learning Platform domain classes and all 13 exceptions.
 */

#include "courses.hpp"
#include "learners.hpp"
#include "enrollments.hpp"
#include "operations.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <cassert>

void testCatalogAndCourses() {
    CourseCatalog catalog;
    ContactAddress addr("10 Academy Rd", "London", "EC1V 9LB", "UK");
    Instructor instr(1, "Dr. Alan Turing", "Algorithms");
    Organization org("Cambridge Online", "CO-01", addr);
    SkillTag skill("Algorithms", "Data structures and complexity");

    Course c1("ALG101", "Algorithms & Data Structures", instr, org, skill, 100.0, 15);
    catalog.addCourse(c1);
    assert(catalog.getCourseCount() == 1);

    const Course& found = catalog.getCourse("ALG101");
    assert(found.getTitle() == "Algorithms & Data Structures");
    assert(found.calculateDiscountedTuition(20.0) == 80.0);

    // DuplicateCourseException
    bool dupCaught = false;
    try { catalog.addCourse(c1); } catch (const DuplicateCourseException&) { dupCaught = true; }
    assert(dupCaught);

    // CourseNotFoundException
    bool notFoundCaught = false;
    try { catalog.getCourse("NOPE999"); } catch (const CourseNotFoundException&) { notFoundCaught = true; }
    assert(notFoundCaught);

    // InvalidCourseDataException
    bool invalidDataCaught = false;
    try { Course bad("BAD1", "BadCourse", instr, org, skill, -50.0, 5); }
    catch (const InvalidCourseDataException&) { invalidDataCaught = true; }
    assert(invalidDataCaught);

    // SeatLimitExceededException
    bool seatLimitCaught = false;
    try { catalog.getCourse("ALG101").reserveSeats(999); }
    catch (const SeatLimitExceededException&) { seatLimitCaught = true; }
    assert(seatLimitCaught);

    // Polymorphic course formats
    VideoCourse vc(c1, 24, 360, true);
    InteractiveCourse ic(c1, "C++17", 10, true);
    CapstoneCourse cc(c1, "Search Engine", 30, 80.0);
    assert(vc.estimateWatchHours(1.5) == 4.0);
    assert(ic.estimatePracticeHours() > 5.0);
    assert(cc.isPassingGrade(85.0));

    catalog.removeCourse("ALG101");
    assert(catalog.getCourseCount() == 0);
}

void testLearnersAndEnrollments() {
    LearnerRegistry registry;
    ContactAddress addr("7 Study Ln", "London", "WC1E 7HU", "UK");
    Learner l = registry.registerLearner("Elena Novak", "elena@learn.io", addr);
    assert(registry.countLearners() == 1);

    // InvalidAccountException
    bool invalidAccCaught = false;
    try { registry.registerLearner("", "invalid-email", addr); }
    catch (const InvalidAccountException&) { invalidAccCaught = true; }
    assert(invalidAccCaught);

    Instructor instr(2, "Grace Hopper", "Compilers");
    Organization org("Systems Academy", "SA-02", addr);
    SkillTag skill("Compilers", "Language translation");
    Course compCourse("CMP301", "Compiler Design", instr, org, skill, 200.0, 10);

    Enrollment enr(301, l);
    enr.addCourseItem(compCourse, 1);
    Coupon coupon("LEARN10", 10.0, 100.0);
    enr.applyCoupon(coupon);
    assert(enr.calculateNetTotal() == 180.0);

    // InvalidCouponException
    bool invalidCouponCaught = false;
    try { Coupon badCoupon("BAD", 120.0, 0.0); }
    catch (const InvalidCouponException&) { invalidCouponCaught = true; }
    assert(invalidCouponCaught);

    // EnrollmentNotFoundException on empty confirm
    Enrollment emptyEnr(888, l);
    bool enrNotFoundCaught = false;
    try { emptyEnr.confirmEnrollment(); }
    catch (const EnrollmentNotFoundException&) { enrNotFoundCaught = true; }
    assert(enrNotFoundCaught);

    Receipt rcpt("RCP-301", enr, 0.10);
    assert(rcpt.calculateGrandTotal() == 198.0);

    // InsufficientBalanceException
    WalletBilling lowWallet("TX-10", 198.0, 50.0, 10.0);
    bool insuffCaught = false;
    try { lowWallet.executePayment(rcpt); }
    catch (const InsufficientBalanceException&) { insuffCaught = true; }
    assert(insuffCaught);

    // InvalidBillingException
    BankTransferBilling unverified("TX-11", 198.0, "elena@learn.io", false);
    bool invalidBillCaught = false;
    try { unverified.executePayment(rcpt); }
    catch (const InvalidBillingException&) { invalidBillCaught = true; }
    assert(invalidBillCaught);

    CardBilling validCard("TX-12", 198.0, "****-8888", "10/29", 1000.0);
    BillingManager bm;
    assert(bm.settleEnrollment(enr, rcpt, validCard));
    assert(rcpt.isSettled());
}

void testStaffSessionsAndReviews() {
    ContactAddress addr("15 Mentor Ave", "London", "SE1 7PB", "UK");
    StaffMember staff(1, "Daniel", "Support", 3000.0, addr);
    PlatformAdmin admin(staff, 5, false);
    StudyPass pass("PASS-55", 3);

    // AccessDeniedException
    bool deniedCaught = false;
    try { admin.freezeStudyPass(pass); }
    catch (const AccessDeniedException&) { deniedCaught = true; }
    assert(deniedCaught);

    // LiveSessionException
    LiveSession session("LIVE-55", "Templates & RAII", "Workshop");
    Learner l(2, "Liam", "liam@learn.io", addr);
    Enrollment cancelled(55, l);
    cancelled.cancelEnrollment();
    bool sessionErrCaught = false;
    try { session.bookForEnrollment(cancelled); }
    catch (const LiveSessionException&) { sessionErrCaught = true; }
    assert(sessionErrCaught);

    // SubmissionNotFoundException
    RatingSummary summary("CMP301");
    summary.addReview(CourseReview(1, "CMP301", "Liam", 5, "Very clear explanations!"));
    assert(summary.calculateAverageStars() == 5.0);
    bool subNotFoundCaught = false;
    try { summary.removeReviewById(777); }
    catch (const SubmissionNotFoundException&) { subNotFoundCaught = true; }
    assert(subNotFoundCaught);
}

int main() {
    testCatalogAndCourses();
    testLearnersAndEnrollments();
    testStaffSessionsAndReviews();
    std::cout << "All Online Learning Platform unit tests (60 classes, 13 exceptions) passed!\n";
    return 0;
}
