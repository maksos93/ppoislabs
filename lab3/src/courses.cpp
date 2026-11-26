/**
 * @file courses.cpp
 * @brief Implementation of Courses, Instructors, Tracks, Lessons, Assignments, and Catalog.
 */

#include "courses.hpp"
#include "exceptions.hpp"
#include <utility>

ContactAddress::ContactAddress(std::string street, std::string city, std::string postal, std::string country)
    : street(std::move(street)), city(std::move(city)), postalCode(std::move(postal)), country(std::move(country)) {}

std::string ContactAddress::formatFullAddress() const {
    return street + ", " + city + " " + postalCode + ", " + country;
}

bool ContactAddress::validatePostalCode() const {
    return postalCode.size() >= 4 && postalCode.size() <= 10;
}

bool ContactAddress::isSameCity(const ContactAddress& other) const {
    return !city.empty() && city == other.city;
}

SkillTag::SkillTag(std::string name, std::string desc)
    : name(std::move(name)), description(std::move(desc)) {}

bool SkillTag::matchesKeyword(const std::string& keyword) const {
    return name.find(keyword) != std::string::npos || description.find(keyword) != std::string::npos;
}

std::string SkillTag::generateTag() const {
    return "#" + name;
}

Track::Track(int id, std::string title, SkillTag skill)
    : id(id), title(std::move(title)), primarySkill(std::move(skill)) {}

void Track::assignSkill(const SkillTag& skill) {
    primarySkill = skill;
}

std::string Track::buildTrackPath() const {
    return primarySkill.getName() + " > " + title;
}

Instructor::Instructor(int id, std::string name, std::string spec)
    : id(id), fullName(std::move(name)), specialization(std::move(spec)) {}

std::string Instructor::formatProfile() const {
    return fullName + " [" + specialization + "]";
}

bool Instructor::specializesIn(const std::string& topic) const {
    return specialization == topic;
}

void Instructor::updateSpecialization(const std::string& newSpec) {
    specialization = newSpec;
}

Organization::Organization(std::string name, std::string code, ContactAddress addr)
    : name(std::move(name)), accreditationCode(std::move(code)), headOffice(std::move(addr)) {}

bool Organization::verifyAccreditation() const {
    return !accreditationCode.empty() && accreditationCode != "000";
}

std::string Organization::generateHeaderBadge() const {
    return name + " (" + accreditationCode + ") - " + headOffice.getCity();
}

Course::Course(const std::string& code, const std::string& title, const Instructor& instructor,
               const Organization& provider, const SkillTag& skill, double fee, int seats)
    : code(code), title(title), leadInstructor(instructor), provider(provider),
      mainSkill(skill), tuitionFee(fee), availableSeats(seats) {
    if (fee < 0.0) {
        throw InvalidCourseDataException("Tuition fee cannot be negative.");
    }
    if (seats < 0) {
        throw InvalidCourseDataException("Available seats cannot be negative.");
    }
}

void Course::updateTuitionFee(double newFee) {
    if (newFee < 0.0) {
        throw InvalidCourseDataException("Tuition fee cannot be negative.");
    }
    tuitionFee = newFee;
}

void Course::expandSeats(int extraSeats) {
    if (extraSeats <= 0) {
        throw InvalidCourseDataException("Extra seats must be positive.");
    }
    availableSeats += extraSeats;
}

void Course::reserveSeats(int count) {
    if (count <= 0) {
        throw InvalidCourseDataException("Seat reservation count must be positive.");
    }
    if (availableSeats < count) {
        throw SeatLimitExceededException(title);
    }
    availableSeats -= count;
}

double Course::calculateDiscountedTuition(double percent) const {
    if (percent < 0.0 || percent > 100.0) {
        throw InvalidCouponException(percent);
    }
    return tuitionFee * (1.0 - percent / 100.0);
}

std::string Course::describeDeliveryFormat() const {
    return "Standard Online Course: " + title;
}

VideoCourse::VideoCourse(const Course& base, int videos, int minutes, bool subtitles)
    : Course(base), totalVideos(videos), totalMinutes(minutes), hasSubtitles(subtitles) {}

double VideoCourse::estimateWatchHours(double speed) const {
    if (speed <= 0.0) return 0.0;
    return (totalMinutes / 60.0) / speed;
}

void VideoCourse::setSubtitlesEnabled(bool enabled) {
    hasSubtitles = enabled;
}

std::string VideoCourse::describeDeliveryFormat() const {
    return "VideoCourse (" + std::to_string(totalVideos) + " videos, " + std::to_string(totalMinutes) + " min)";
}

InteractiveCourse::InteractiveCourse(const Course& base, std::string env, int labs, bool autoGrade)
    : Course(base), runtimeEnv(std::move(env)), labExercisesCount(labs), autoGraded(autoGrade) {}

double InteractiveCourse::estimatePracticeHours() const {
    return (labExercisesCount * 35.0) / 60.0;
}

void InteractiveCourse::addLabExercises(int additionalLabs) {
    if (additionalLabs > 0) {
        labExercisesCount += additionalLabs;
    }
}

std::string InteractiveCourse::describeDeliveryFormat() const {
    return "InteractiveCourse [" + runtimeEnv + ", " + std::to_string(labExercisesCount) + " labs]";
}

BootcampCourse::BootcampCourse(const Course& base, int weeks, int liveHours, std::string cohort)
    : Course(base), durationWeeks(weeks), weeklyLiveHours(liveHours), cohortCode(std::move(cohort)) {}

int BootcampCourse::calculateTotalLiveHours() const {
    return durationWeeks * weeklyLiveHours;
}

void BootcampCourse::extendDuration(int extraWeeks) {
    if (extraWeeks > 0) {
        durationWeeks += extraWeeks;
    }
}

std::string BootcampCourse::describeDeliveryFormat() const {
    return "BootcampCourse [" + cohortCode + ", " + std::to_string(durationWeeks) + " weeks]";
}

CapstoneCourse::CapstoneCourse(const Course& base, std::string topic, int defenseMin, double passScore)
    : Course(base), projectTopic(std::move(topic)), defenseMinutes(defenseMin), passingScore(passScore) {}

bool CapstoneCourse::isPassingGrade(double score) const {
    return score >= passingScore;
}

void CapstoneCourse::updatePassingThreshold(double newThreshold) {
    if (newThreshold < 0.0 || newThreshold > 100.0) {
        throw InvalidCourseDataException("Passing threshold must be between 0 and 100.");
    }
    passingScore = newThreshold;
}

std::string CapstoneCourse::describeDeliveryFormat() const {
    return "CapstoneCourse: " + projectTopic + " (" + std::to_string(defenseMinutes) + " min defense)";
}

Lesson::Lesson(std::string code, Course course, int minutes, int xp)
    : lessonCode(std::move(code)), parentCourse(std::move(course)), durationMinutes(minutes), xpReward(xp) {}

bool Lesson::isMicroLesson() const {
    return durationMinutes <= 15;
}

void Lesson::adjustDuration(int deltaMinutes) {
    if (durationMinutes + deltaMinutes <= 0) {
        throw InvalidCourseDataException("Lesson duration must remain positive.");
    }
    durationMinutes += deltaMinutes;
}

int Lesson::calculateTotalXp(bool streakActive) const {
    return streakActive ? (xpReward + 25) : xpReward;
}

StudyModule::StudyModule(std::string title) : moduleTitle(std::move(title)), totalXp(0) {}

void StudyModule::addLesson(const Lesson& lesson) {
    lessons.push_back(lesson);
    totalXp += lesson.calculateTotalXp(false);
}

int StudyModule::countMicroLessons() const {
    int count = 0;
    for (const auto& l : lessons) {
        if (l.isMicroLesson()) count++;
    }
    return count;
}

int StudyModule::computeTotalMinutes() const {
    int sum = 0;
    for (const auto& l : lessons) {
        sum += l.calculateTotalXp(false);
    }
    return sum;
}

Assignment::Assignment(std::string id, int maxPts)
    : assignmentId(std::move(id)), maxPoints(maxPts), currentScore(0) {}

bool Assignment::submitScore(int pts) {
    if (pts < 0 || pts > maxPoints) return false;
    currentScore = pts;
    return true;
}

void Assignment::resetScore() {
    currentScore = 0;
}

double Assignment::getCompletionRatio() const {
    return maxPoints > 0 ? static_cast<double>(currentScore) / maxPoints : 0.0;
}

Syllabus::Syllabus(std::string title, SkillTag skill)
    : syllabusTitle(std::move(title)), targetSkill(std::move(skill)) {}

void Syllabus::addModule(const StudyModule& mod) {
    modules.push_back(mod);
}

double Syllabus::calculateAverageModuleMinutes() const {
    if (modules.empty()) return 0.0;
    double total = 0.0;
    for (const auto& m : modules) {
        total += m.computeTotalMinutes();
    }
    return total / modules.size();
}

Classroom::Classroom(std::string id, ContactAddress addr)
    : roomId(std::move(id)), campusLocation(std::move(addr)), activeModule("Active Module") {}

void Classroom::addSyllabus(const Syllabus& syl) {
    syllabi.push_back(syl);
}

void Classroom::scheduleLesson(const Lesson& lesson) {
    activeModule.addLesson(lesson);
}

bool Classroom::isInCity(const std::string& city) const {
    return campusLocation.getCity() == city;
}

int Classroom::evaluateActiveModuleMinutes() const {
    return activeModule.computeTotalMinutes();
}

void CourseCatalog::addCourse(const Course& course) {
    if (courses.find(course.getCode()) != courses.end()) {
        throw DuplicateCourseException(course.getCode());
    }
    courses.insert({course.getCode(), course});
}

void CourseCatalog::removeCourse(const std::string& code) {
    auto it = courses.find(code);
    if (it == courses.end()) {
        throw CourseNotFoundException(code);
    }
    courses.erase(it);
}

Course& CourseCatalog::getCourse(const std::string& code) {
    auto it = courses.find(code);
    if (it == courses.end()) {
        throw CourseNotFoundException(code);
    }
    return it->second;
}

const Course& CourseCatalog::getCourse(const std::string& code) const {
    auto it = courses.find(code);
    if (it == courses.end()) {
        throw CourseNotFoundException(code);
    }
    return it->second;
}

std::vector<Course> CourseCatalog::searchByInstructor(const std::string& instructorName) const {
    std::vector<Course> result;
    for (const auto& pair : courses) {
        if (pair.second.getLeadInstructor().getFullName() == instructorName) {
            result.push_back(pair.second);
        }
    }
    return result;
}

void CourseCatalog::registerFeaturedInstructor(const Instructor& instructor) {
    featuredInstructors.push_back(instructor);
}
