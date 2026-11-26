/**
 * @file main.cpp
 * @brief CLI application for Online Learning Platform (Lab 2).
 */

#include <iostream>
#include "courses.hpp"
#include "learners.hpp"
#include "enrollments.hpp"
#include "operations.hpp"
#include "exceptions.hpp"

namespace {
    const int kExitChoice = 0;
    const int kAddCourse = 1;
    const int kFindCourse = 2;
    const int kRemoveCourse = 3;
    const int kShowStats = 4;
    const int kSimulateEnrollment = 5;

    void printMenu() {
        std::cout << "\n=== ONLINE LEARNING PLATFORM (LAB 2) ===\n"
                  << "1. Add new course\n"
                  << "2. Find course by code\n"
                  << "3. Remove course\n"
                  << "4. Show total catalog size\n"
                  << "5. Simulate enrollment & billing workflow\n"
                  << "0. Exit\n"
                  << "Select option: ";
    }

    void handleAdd(CourseCatalog& catalog) {
        std::string code, title, instructorName, skillName;
        double fee = 0.0;
        int seats = 0;

        std::cout << "Enter Course Code: ";
        std::cin >> code;
        std::cout << "Enter Title: ";
        std::cin >> title;
        std::cout << "Enter Instructor: ";
        std::cin >> instructorName;
        std::cout << "Enter Skill: ";
        std::cin >> skillName;
        std::cout << "Enter Tuition Fee: ";
        std::cin >> fee;
        std::cout << "Enter Available Seats: ";
        std::cin >> seats;

        try {
            Instructor instr(1, instructorName, skillName);
            Organization org("EduHub Academy", "EDU-01", ContactAddress("10 Campus Way", "London", "EC1A 1BB", "UK"));
            SkillTag skill(skillName, skillName + " track");
            catalog.addCourse(Course(code, title, instr, org, skill, fee, seats));
            std::cout << "Course successfully added to catalog.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleSearch(const CourseCatalog& catalog) {
        std::string code;
        std::cout << "Enter course code to find: ";
        std::cin >> code;
        try {
            const Course& c = catalog.getCourse(code);
            std::cout << "Found: " << c.getTitle() << " by " << c.getLeadInstructor().getFullName()
                      << " | Fee: $" << c.getTuitionFee() << " | Seats: " << c.getAvailableSeats() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleRemove(CourseCatalog& catalog) {
        std::string code;
        std::cout << "Enter course code to remove: ";
        std::cin >> code;
        try {
            catalog.removeCourse(code);
            std::cout << "Course removed successfully.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void handleEnrollmentSimulation(CourseCatalog& catalog) {
        try {
            if (catalog.getCourseCount() == 0) {
                Instructor instr(10, "Alex Mercer", "OOP & C++");
                Organization org("CodeCraft Institute", "CC-99", ContactAddress("22 Tech Blvd", "London", "E1 6AN", "UK"));
                SkillTag skill("C++", "Modern C++ and OOP");
                catalog.addCourse(Course("CPP202", "ObjectOrientedCPP", instr, org, skill, 120.0, 25));
            }
            LearnerRegistry registry;
            ContactAddress addr("5 Learning St", "London", "SW1A 1AA", "UK");
            Learner learner = registry.registerLearner("Maria Ivanova", "maria@edtech.org", addr);

            const Course& target = catalog.getCourse("CPP202");
            Enrollment enrollment(201, learner);
            enrollment.addCourseItem(target, 1);
            enrollment.applyCoupon(Coupon("STUDENT15", 15.0, 50.0));

            Receipt receipt("RCP-2026-201", enrollment, 0.20);
            CardBilling billing("TX-7001", receipt.calculateGrandTotal(), "****-4242", "11/29", 1000.0);

            BillingManager manager;
            if (manager.settleEnrollment(enrollment, receipt, billing)) {
                std::cout << "Enrollment #" << enrollment.getEnrollmentId() << " settled! Grand total: $"
                          << receipt.calculateGrandTotal() << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Simulation error: " << e.what() << "\n";
        }
    }
}

int main() {
    CourseCatalog catalog;
    int choice = -1;

    do {
        printMenu();
        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case kAddCourse: handleAdd(catalog); break;
            case kFindCourse: handleSearch(catalog); break;
            case kRemoveCourse: handleRemove(catalog); break;
            case kShowStats: std::cout << "Total courses in catalog: " << catalog.getCourseCount() << "\n"; break;
            case kSimulateEnrollment: handleEnrollmentSimulation(catalog); break;
            case kExitChoice: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid option. Try again.\n"; break;
        }
    } while (choice != kExitChoice);

    return 0;
}
