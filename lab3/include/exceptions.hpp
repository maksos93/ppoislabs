/**
 * @file exceptions.hpp
 * @brief Custom exception classes for the Online Learning Platform domain.
 */

#pragma once

#include <stdexcept>
#include <string>

/**
 * @class PlatformException
 * @brief Base runtime exception for all errors in the online learning platform.
 */
class PlatformException : public std::runtime_error {
public:
    /**
     * @brief Constructs a PlatformException with an error message.
     * @param message Human-readable error description.
     */
    explicit PlatformException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @class CourseNotFoundException
 * @brief Thrown when a requested course cannot be found in the catalog.
 */
class CourseNotFoundException : public PlatformException {
public:
    /**
     * @brief Constructs CourseNotFoundException for a missing course code.
     * @param courseCode Course identifier that was not found.
     */
    explicit CourseNotFoundException(const std::string& courseCode)
        : PlatformException("Course not found: " + courseCode) {}
};

/**
 * @class DuplicateCourseException
 * @brief Thrown when registering a course with an already existing code.
 */
class DuplicateCourseException : public PlatformException {
public:
    /**
     * @brief Constructs DuplicateCourseException for a duplicate course code.
     * @param courseCode Duplicate course identifier.
     */
    explicit DuplicateCourseException(const std::string& courseCode)
        : PlatformException("Duplicate course code: " + courseCode) {}
};

/**
 * @class SeatLimitExceededException
 * @brief Thrown when course enrollment exceeds maximum seat capacity.
 */
class SeatLimitExceededException : public PlatformException {
public:
    /**
     * @brief Constructs SeatLimitExceededException for a full course.
     * @param courseTitle Title of the course with no remaining seats.
     */
    explicit SeatLimitExceededException(const std::string& courseTitle)
        : PlatformException("Seat limit exceeded for course: " + courseTitle) {}
};

/**
 * @class InvalidCourseDataException
 * @brief Thrown when course pricing, duration, credits, or grading parameters are invalid.
 */
class InvalidCourseDataException : public PlatformException {
public:
    /**
     * @brief Constructs InvalidCourseDataException with validation details.
     * @param details Description of the invalid course attribute.
     */
    explicit InvalidCourseDataException(const std::string& details)
        : PlatformException("Invalid course data: " + details) {}
};

/**
 * @class InvalidAccountException
 * @brief Thrown when user/student/instructor account data or lookup is invalid.
 */
class InvalidAccountException : public PlatformException {
public:
    /**
     * @brief Constructs InvalidAccountException with account error context.
     * @param accountInfo Details of the invalid account or query.
     */
    explicit InvalidAccountException(const std::string& accountInfo)
        : PlatformException("Invalid account data: " + accountInfo) {}
};

/**
 * @class EnrollmentNotFoundException
 * @brief Thrown when an enrollment record is missing or empty.
 */
class EnrollmentNotFoundException : public PlatformException {
public:
    /**
     * @brief Constructs EnrollmentNotFoundException for a specific enrollment ID.
     * @param enrollmentId Numeric enrollment identifier.
     */
    explicit EnrollmentNotFoundException(int enrollmentId)
        : PlatformException("Enrollment not found with ID: " + std::to_string(enrollmentId)) {}
};

/**
 * @class InvalidBillingException
 * @brief Thrown when tuition payment or billing verification fails.
 */
class InvalidBillingException : public PlatformException {
public:
    /**
     * @brief Constructs InvalidBillingException with billing error details.
     * @param details Reason why billing failed.
     */
    explicit InvalidBillingException(const std::string& details)
        : PlatformException("Invalid billing operation: " + details) {}
};

/**
 * @class InsufficientBalanceException
 * @brief Thrown when wallet balance, credit limit, or reward points are insufficient.
 */
class InsufficientBalanceException : public PlatformException {
public:
    /**
     * @brief Constructs InsufficientBalanceException comparing required and available amounts.
     * @param required Required amount.
     * @param available Available amount.
     */
    explicit InsufficientBalanceException(double required, double available)
        : PlatformException("Insufficient balance. Required: " + std::to_string(required) +
                           ", Available: " + std::to_string(available)) {}
};

/**
 * @class LiveSessionException
 * @brief Thrown when scheduling or joining a live class or webinar fails.
 */
class LiveSessionException : public PlatformException {
public:
    /**
     * @brief Constructs LiveSessionException with session scheduling error details.
     * @param reason Explanation of the live session failure.
     */
    explicit LiveSessionException(const std::string& reason)
        : PlatformException("Live session error: " + reason) {}
};

/**
 * @class AccessDeniedException
 * @brief Thrown when an account, subscription, or staff role lacks permission.
 */
class AccessDeniedException : public PlatformException {
public:
    /**
     * @brief Constructs AccessDeniedException for a role or permission scope.
     * @param scope Role or resource that denied access.
     */
    explicit AccessDeniedException(const std::string& scope)
        : PlatformException("Access denied for: " + scope) {}
};

/**
 * @class InvalidCouponException
 * @brief Thrown when a discount coupon percentage or eligibility rule is invalid.
 */
class InvalidCouponException : public PlatformException {
public:
    /**
     * @brief Constructs InvalidCouponException for an invalid coupon value.
     * @param value Value that triggered the coupon error.
     */
    explicit InvalidCouponException(double value)
        : PlatformException("Invalid coupon value: " + std::to_string(value)) {}
};

/**
 * @class SubmissionNotFoundException
 * @brief Thrown when a homework or quiz submission cannot be found by ID.
 */
class SubmissionNotFoundException : public PlatformException {
public:
    /**
     * @brief Constructs SubmissionNotFoundException for a missing submission ID.
     * @param submissionId Numeric submission identifier.
     */
    explicit SubmissionNotFoundException(int submissionId)
        : PlatformException("Submission not found with ID: " + std::to_string(submissionId)) {}
};
