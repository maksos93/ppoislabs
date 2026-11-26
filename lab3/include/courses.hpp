/**
 * @file courses.hpp
 * @brief Courses, Instructors, Tracks, Lessons, Assignments, and Catalog definitions.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * @class ContactAddress
 * @brief Represents a physical or billing address for instructors, organizations, and classrooms.
 */
class ContactAddress {
private:
    std::string street;     ///< Street and building number.
    std::string city;       ///< City name.
    std::string postalCode; ///< Postal or ZIP code.
    std::string country;    ///< Country name.
public:
    /**
     * @brief Constructs a ContactAddress instance.
     * @param street Street line.
     * @param city City name.
     * @param postal Postal code.
     * @param country Country name.
     */
    ContactAddress(std::string street = "", std::string city = "", std::string postal = "", std::string country = "");

    /**
     * @brief Formats the full address into a single string.
     * @return Formatted full address.
     */
    std::string formatFullAddress() const;

    /**
     * @brief Checks whether postal code length is valid (4..10 characters).
     * @return True if valid.
     */
    bool validatePostalCode() const;

    /**
     * @brief Checks whether another address is in the same city.
     * @param other Other ContactAddress to compare.
     * @return True if both share a non-empty city name.
     */
    bool isSameCity(const ContactAddress& other) const;

    std::string getCity() const { return city; }
};

/**
 * @class SkillTag
 * @brief Represents a competency or technology tag associated with courses and tracks.
 */
class SkillTag {
private:
    std::string name;        ///< Skill name (e.g., C++, Algorithms).
    std::string description; ///< Skill scope description.
public:
    /**
     * @brief Constructs a SkillTag.
     * @param name Skill title.
     * @param desc Skill description.
     */
    SkillTag(std::string name = "General", std::string desc = "");

    /**
     * @brief Checks if name or description contains a keyword.
     * @param keyword Search substring.
     * @return True if keyword matches.
     */
    bool matchesKeyword(const std::string& keyword) const;

    /**
     * @brief Formats the skill tag with a leading '#'.
     * @return Formatted tag string.
     */
    std::string generateTag() const;

    std::string getName() const { return name; }
};

/**
 * @class Track
 * @brief Represents a learning track anchored to a primary SkillTag.
 */
class Track {
private:
    int id;               ///< Unique track ID.
    std::string title;    ///< Track display title.
    SkillTag primarySkill;///< Primary SkillTag for this track.
public:
    /**
     * @brief Constructs a Track.
     * @param id Track ID.
     * @param title Track title.
     * @param skill Primary SkillTag.
     */
    Track(int id = 0, std::string title = "Foundations", SkillTag skill = SkillTag());

    /**
     * @brief Assigns a new primary SkillTag to the track.
     * @param skill New SkillTag.
     */
    void assignSkill(const SkillTag& skill);

    /**
     * @brief Builds a path string combining primary skill and track title.
     * @return Path string "Skill > Track".
     */
    std::string buildTrackPath() const;

    std::string getTitle() const { return title; }
};

/**
 * @class Instructor
 * @brief Represents a course instructor or mentor with specialization and rating.
 */
class Instructor {
private:
    int id;                  ///< Unique instructor ID.
    std::string fullName;    ///< Instructor full name.
    std::string specialization;///< Main subject specialization.
public:
    /**
     * @brief Constructs an Instructor.
     * @param id Instructor ID.
     * @param name Full name.
     * @param spec Specialization area.
     */
    Instructor(int id = 0, std::string name = "TBA", std::string spec = "Computer Science");

    /**
     * @brief Formats an academic citation line for the instructor.
     * @return Formatted citation string.
     */
    std::string formatProfile() const;

    /**
     * @brief Checks whether instructor specializes in the given topic.
     * @param topic Topic name to check.
     * @return True if specialization matches topic.
     */
    bool specializesIn(const std::string& topic) const;

    /**
     * @brief Updates the instructor's specialization.
     * @param newSpec New specialization string.
     */
    void updateSpecialization(const std::string& newSpec);

    std::string getFullName() const { return fullName; }
};

/**
 * @class Organization
 * @brief Represents an educational partner or university providing accredited courses.
 */
class Organization {
private:
    std::string name;             ///< Organization name.
    std::string accreditationCode;///< Accreditation code.
    ContactAddress headOffice;    ///< Headquarters address.
public:
    /**
     * @brief Constructs an Organization.
     * @param name Organization name.
     * @param code Accreditation code.
     * @param addr Headquarters address.
     */
    Organization(std::string name = "OpenAcademy", std::string code = "ACC-01", ContactAddress addr = ContactAddress());

    /**
     * @brief Verifies whether the organization has a valid accreditation code.
     * @return True if accredited.
     */
    bool verifyAccreditation() const;

    /**
     * @brief Generates a header badge with organization name, code, and city.
     * @return Formatted badge string.
     */
    std::string generateHeaderBadge() const;

    std::string getName() const { return name; }
};

/**
 * @class Course
 * @brief Core domain class representing an online course with pricing and seat capacity.
 */
class Course {
protected:
    std::string code;          ///< Unique course code (e.g., CS101).
    std::string title;         ///< Course title.
    Instructor leadInstructor; ///< Primary course Instructor.
    Organization provider;     ///< Publishing Organization.
    SkillTag mainSkill;        ///< Primary SkillTag taught.
    double tuitionFee;         ///< Course price in currency units.
    int availableSeats;        ///< Remaining enrollment seats.
public:
    /**
     * @brief Constructs a Course and validates tuition and seat non-negativity.
     * @param code Unique course code.
     * @param title Course title.
     * @param instructor Lead Instructor.
     * @param provider Providing Organization.
     * @param skill Main SkillTag.
     * @param fee Tuition fee (must be >= 0).
     * @param seats Available seats (must be >= 0).
     * @throws InvalidCourseDataException if fee or seats is negative.
     */
    Course(const std::string& code = "", const std::string& title = "",
           const Instructor& instructor = Instructor(), const Organization& provider = Organization(),
           const SkillTag& skill = SkillTag(), double fee = 0.0, int seats = 0);

    /**
     * @brief Virtual destructor for course hierarchy.
     */
    virtual ~Course() = default;

    std::string getCode() const { return code; }
    std::string getTitle() const { return title; }
    Instructor getLeadInstructor() const { return leadInstructor; }
    double getTuitionFee() const { return tuitionFee; }
    int getAvailableSeats() const { return availableSeats; }

    /**
     * @brief Updates course tuition fee.
     * @param newFee New fee (must be >= 0).
     * @throws InvalidCourseDataException if newFee < 0.
     */
    void updateTuitionFee(double newFee);

    /**
     * @brief Adds new enrollment seats to the course.
     * @param extraSeats Number of seats to add (must be > 0).
     * @throws InvalidCourseDataException if extraSeats <= 0.
     */
    void expandSeats(int extraSeats);

    /**
     * @brief Reserves seats for enrolled learners.
     * @param count Number of seats to reserve (must be > 0).
     * @throws InvalidCourseDataException if count <= 0.
     * @throws SeatLimitExceededException if availableSeats < count.
     */
    void reserveSeats(int count);

    /**
     * @brief Calculates discounted tuition fee for a percentage coupon.
     * @param percent Discount percentage in [0, 100].
     * @return Discounted tuition price.
     * @throws InvalidCouponException if percent is outside [0, 100].
     */
    double calculateDiscountedTuition(double percent) const;

    /**
     * @brief Describes the course delivery format.
     * @return Format description string.
     */
    virtual std::string describeDeliveryFormat() const;
};

/**
 * @class VideoCourse
 * @brief Self-paced course built around recorded video lectures and captions.
 */
class VideoCourse : public Course {
private:
    int totalVideos;         ///< Number of lecture videos.
    int totalMinutes;        ///< Total video runtime in minutes.
    bool hasSubtitles;       ///< Whether closed captions are provided.
public:
    /**
     * @brief Constructs a VideoCourse from a base Course.
     * @param base Base Course.
     * @param videos Video count.
     * @param minutes Total runtime in minutes.
     * @param subtitles Subtitles availability flag.
     */
    VideoCourse(const Course& base, int videos = 20, int minutes = 480, bool subtitles = true);

    /**
     * @brief Estimates completion hours at a given playback speed.
     * @param speed Playback speed multiplier (e.g., 1.0, 1.25).
     * @return Estimated hours to watch all videos.
     */
    double estimateWatchHours(double speed) const;

    /**
     * @brief Updates closed-caption availability.
     * @param enabled New subtitles flag.
     */
    void setSubtitlesEnabled(bool enabled);

    std::string describeDeliveryFormat() const override;
};

/**
 * @class InteractiveCourse
 * @brief Hands-on course with browser sandboxes and automated coding checks.
 */
class InteractiveCourse : public Course {
private:
    std::string runtimeEnv;  ///< Sandbox runtime environment (e.g., Python3, C++17).
    int labExercisesCount;   ///< Number of interactive coding labs.
    bool autoGraded;         ///< Whether exercises are auto-graded.
public:
    /**
     * @brief Constructs an InteractiveCourse from a base Course.
     * @param base Base Course.
     * @param env Runtime environment name.
     * @param labs Number of lab exercises.
     * @param autoGrade Auto-grading flag.
     */
    InteractiveCourse(const Course& base, std::string env = "C++17", int labs = 12, bool autoGrade = true);

    /**
     * @brief Estimates total practice hours assuming 35 minutes per lab.
     * @return Estimated lab hours.
     */
    double estimatePracticeHours() const;

    /**
     * @brief Adds additional lab exercises to the course.
     * @param additionalLabs Number of labs to add.
     */
    void addLabExercises(int additionalLabs);

    std::string describeDeliveryFormat() const override;
};

/**
 * @class BootcampCourse
 * @brief Intensive cohort-based program with live mentors and weekly sprints.
 */
class BootcampCourse : public Course {
private:
    int durationWeeks;     ///< Cohort length in weeks.
    int weeklyLiveHours;   ///< Weekly live mentoring hours.
    std::string cohortCode;///< Cohort identifier string.
public:
    /**
     * @brief Constructs a BootcampCourse from a base Course.
     * @param base Base Course.
     * @param weeks Duration in weeks.
     * @param liveHours Weekly live hours.
     * @param cohort Cohort code.
     */
    BootcampCourse(const Course& base, int weeks = 8, int liveHours = 6, std::string cohort = "COHORT-2026");

    /**
     * @brief Computes total scheduled live mentoring hours across the bootcamp.
     * @return Total live hours.
     */
    int calculateTotalLiveHours() const;

    /**
     * @brief Extends the bootcamp schedule by extra weeks.
     * @param extraWeeks Weeks to add.
     */
    void extendDuration(int extraWeeks);

    std::string describeDeliveryFormat() const override;
};

/**
 * @class CapstoneCourse
 * @brief Project-based course culminating in a graded portfolio defense.
 */
class CapstoneCourse : public Course {
private:
    std::string projectTopic;///< Capstone project domain topic.
    int defenseMinutes;      ///< Presentation and defense duration in minutes.
    double passingScore;     ///< Minimum score required to pass.
public:
    /**
     * @brief Constructs a CapstoneCourse from a base Course.
     * @param base Base Course.
     * @param topic Project topic.
     * @param defenseMin Defense duration in minutes.
     * @param passScore Passing score threshold.
     */
    CapstoneCourse(const Course& base, std::string topic = "EdTech Platform", int defenseMin = 30, double passScore = 75.0);

    /**
     * @brief Checks whether a candidate score passes the capstone requirement.
     * @param score Achieved project score.
     * @return True if score >= passingScore.
     */
    bool isPassingGrade(double score) const;

    /**
     * @brief Updates the passing score threshold.
     * @param newThreshold New passing score in [0, 100].
     * @throws InvalidCourseDataException if out of [0, 100].
     */
    void updatePassingThreshold(double newThreshold);

    std::string describeDeliveryFormat() const override;
};

/**
 * @class Lesson
 * @brief Represents an individual learning unit inside a module.
 */
class Lesson {
private:
    std::string lessonCode; ///< Unique lesson code.
    Course parentCourse;    ///< Course this lesson belongs to.
    int durationMinutes;    ///< Estimated study time in minutes.
    int xpReward;           ///< Experience points awarded on completion.
public:
    /**
     * @brief Constructs a Lesson.
     * @param code Lesson code.
     * @param course Parent Course.
     * @param minutes Duration in minutes.
     * @param xp XP reward.
     */
    Lesson(std::string code = "", Course course = Course(), int minutes = 25, int xp = 50);

    /**
     * @brief Checks whether the lesson is a short micro-learning unit (<= 15 min).
     * @return True if durationMinutes <= 15.
     */
    bool isMicroLesson() const;

    /**
     * @brief Adjusts lesson duration by a delta in minutes.
     * @param deltaMinutes Change in minutes.
     * @throws InvalidCourseDataException if resulting duration <= 0.
     */
    void adjustDuration(int deltaMinutes);

    /**
     * @brief Calculates XP earned including an optional streak bonus.
     * @param streakActive True if learner has an active study streak.
     * @return Total XP points.
     */
    int calculateTotalXp(bool streakActive) const;

    std::string getLessonCode() const { return lessonCode; }
};

/**
 * @class StudyModule
 * @brief Groups lessons into a structured curriculum section.
 */
class StudyModule {
private:
    std::string moduleTitle;     ///< Module title.
    std::vector<Lesson> lessons; ///< Lessons within the module.
    int totalXp;                 ///< Cumulative XP across lessons.
public:
    /**
     * @brief Constructs a StudyModule.
     * @param title Module title.
     */
    StudyModule(std::string title = "Module 1");

    /**
     * @brief Adds a Lesson to the module and updates totalXp.
     * @param lesson Lesson to add.
     */
    void addLesson(const Lesson& lesson);

    /**
     * @brief Counts micro-lessons in the module.
     * @return Count of short lessons.
     */
    int countMicroLessons() const;

    /**
     * @brief Computes total duration of all lessons in minutes.
     * @return Total minutes.
     */
    int computeTotalMinutes() const;
};

/**
 * @class Assignment
 * @brief Represents a graded homework task or quiz with a deadline and max points.
 */
class Assignment {
private:
    std::string assignmentId; ///< Unique assignment identifier.
    int maxPoints;            ///< Maximum achievable score.
    int currentScore;         ///< Scored points.
public:
    /**
     * @brief Constructs an Assignment.
     * @param id Assignment ID.
     * @param maxPts Maximum points.
     */
    Assignment(std::string id = "HW-1", int maxPts = 100);

    /**
     * @brief Records a score within [0, maxPoints].
     * @param pts Points to assign.
     * @return True if score was recorded.
     */
    bool submitScore(int pts);

    /**
     * @brief Resets recorded score back to zero.
     */
    void resetScore();

    /**
     * @brief Calculates completion ratio in [0.0, 1.0].
     * @return Score ratio.
     */
    double getCompletionRatio() const;
};

/**
 * @class Syllabus
 * @brief Organizes study modules and a primary skill tag for a course curriculum.
 */
class Syllabus {
private:
    std::string syllabusTitle;       ///< Syllabus title.
    std::vector<StudyModule> modules;///< Modules in sequence.
    SkillTag targetSkill;            ///< Target SkillTag.
public:
    /**
     * @brief Constructs a Syllabus.
     * @param title Syllabus title.
     * @param skill Target SkillTag.
     */
    Syllabus(std::string title = "Core Syllabus", SkillTag skill = SkillTag());

    /**
     * @brief Appends a StudyModule to the syllabus.
     * @param mod StudyModule to add.
     */
    void addModule(const StudyModule& mod);

    /**
     * @brief Computes average lesson minutes across all modules.
     * @return Average minutes per module.
     */
    double calculateAverageModuleMinutes() const;
};

/**
 * @class Classroom
 * @brief Represents a virtual or physical cohort space holding modules and a lesson roster.
 */
class Classroom {
private:
    std::string roomId;            ///< Unique classroom ID.
    ContactAddress campusLocation; ///< Location or regional hub address.
    std::vector<Syllabus> syllabi; ///< Assigned course syllabi.
    StudyModule activeModule;      ///< Currently active module.
public:
    /**
     * @brief Constructs a Classroom at a ContactAddress.
     * @param id Classroom ID.
     * @param addr Location address.
     */
    Classroom(std::string id = "ROOM-01", ContactAddress addr = ContactAddress());

    /**
     * @brief Adds a Syllabus to the classroom.
     * @param syl Syllabus to attach.
     */
    void addSyllabus(const Syllabus& syl);

    /**
     * @brief Registers a Lesson into the active module.
     * @param lesson Lesson to add.
     */
    void scheduleLesson(const Lesson& lesson);

    /**
     * @brief Checks whether the classroom hub is in the given city.
     * @param city City name.
     * @return True if located in city.
     */
    bool isInCity(const std::string& city) const;

    /**
     * @brief Evaluates total learning minutes in the active module.
     * @return Total minutes.
     */
    int evaluateActiveModuleMinutes() const;
};

/**
 * @class CourseCatalog
 * @brief Central catalog managing courses by code and featured instructors.
 */
class CourseCatalog {
private:
    std::map<std::string, Course> courses;      ///< Map from course code to Course.
    std::vector<Instructor> featuredInstructors;///< Featured instructors.
public:
    CourseCatalog() = default;

    /**
     * @brief Adds a new Course to the catalog.
     * @param course Course instance to insert.
     * @throws DuplicateCourseException if course code already exists.
     */
    void addCourse(const Course& course);

    /**
     * @brief Removes a Course from the catalog by code.
     * @param code Course code to remove.
     * @throws CourseNotFoundException if code is not found.
     */
    void removeCourse(const std::string& code);

    /**
     * @brief Returns mutable Course reference by code.
     * @param code Course code.
     * @return Reference to Course.
     * @throws CourseNotFoundException if not found.
     */
    Course& getCourse(const std::string& code);

    /**
     * @brief Returns const Course reference by code.
     * @param code Course code.
     * @return Const reference to Course.
     * @throws CourseNotFoundException if not found.
     */
    const Course& getCourse(const std::string& code) const;

    /**
     * @brief Finds all courses taught by a specific instructor name.
     * @param instructorName Instructor full name.
     * @return Vector of matching Course objects.
     */
    std::vector<Course> searchByInstructor(const std::string& instructorName) const;

    /**
     * @brief Registers a featured Instructor.
     * @param instructor Instructor to add.
     */
    void registerFeaturedInstructor(const Instructor& instructor);

    size_t getCourseCount() const { return courses.size(); }
};
