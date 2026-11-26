# Лабораторная работа №3. Крупный ООП-проект

Предметная область: **Платформа онлайн обучения**. Модель описывает курсы и их форматы, треки и навыки, преподавателей и организации, модули и уроки, задания, учебные аудитории и каталоги, обучающихся и подписки, зачисления, купоны и биллинг, сотрудников, расписание, контент-партнеров, живые занятия, возвраты, аналитику, отзывы и аудит.

Формат строки: `Класс | число полей | число методов | связанные классы`.

## Спецификация объектной модели (60 доменных классов)

| Класс | Поля | Методы | Ассоциации (связанные классы) |
| :--- | :---: | :---: | :--- |
| **ContactAddress** | 4 | 3 | — |
| **SkillTag** | 2 | 2 | — |
| **Track** | 3 | 2 | SkillTag |
| **Instructor** | 3 | 3 | — |
| **Organization** | 3 | 2 | ContactAddress |
| **Course** | 7 | 5 | Instructor, Organization, SkillTag |
| **VideoCourse** | 3 | 3 | Course |
| **InteractiveCourse** | 3 | 3 | Course |
| **BootcampCourse** | 3 | 3 | Course |
| **CapstoneCourse** | 3 | 3 | Course |
| **Lesson** | 4 | 3 | Course |
| **StudyModule** | 3 | 3 | Lesson |
| **Assignment** | 3 | 3 | — |
| **Syllabus** | 3 | 2 | StudyModule, SkillTag |
| **Classroom** | 4 | 4 | ContactAddress, Syllabus, StudyModule, Lesson |
| **CourseCatalog** | 2 | 4 | Course, Instructor |
| **AlertSettings** | 3 | 3 | — |
| **Learner** | 5 | 4 | ContactAddress, AlertSettings |
| **StudyPass** | 4 | 4 | — |
| **StudentLearner** | 3 | 5 | Learner, StudyPass, Course |
| **UniversityStudent** | 3 | 3 | StudentLearner |
| **CorporateLearner** | 3 | 3 | StudentLearner |
| **RewardWallet** | 3 | 3 | — |
| **ProSubscriber** | 3 | 4 | Learner, RewardWallet |
| **Visitor** | 3 | 2 | — |
| **LearnerRegistry** | 2 | 4 | Learner, ContactAddress |
| **EnrollmentItem** | 3 | 3 | Course |
| **Coupon** | 3 | 2 | — |
| **PromoCampaign** | 4 | 3 | Coupon |
| **Enrollment** | 5 | 5 | Learner, EnrollmentItem, Course, Coupon |
| **Receipt** | 4 | 3 | Enrollment |
| **BillingTransaction** | 4 | 2 | Receipt |
| **CardBilling** | 3 | 2 | BillingTransaction, Receipt |
| **WalletBilling** | 2 | 2 | BillingTransaction, Receipt |
| **BankTransferBilling** | 2 | 2 | BillingTransaction, Receipt |
| **BillingManager** | 2 | 3 | Receipt, Enrollment, BillingTransaction |
| **StaffMember** | 5 | 3 | ContactAddress |
| **Mentor** | 2 | 2 | StaffMember, StudentLearner, Course |
| **CurriculumEditor** | 2 | 2 | StaffMember, CourseCatalog, Course |
| **BillingSpecialist** | 2 | 2 | StaffMember |
| **CourseDirector** | 2 | 2 | StaffMember, Enrollment, Coupon |
| **PlatformAdmin** | 2 | 2 | StaffMember, StudyPass |
| **SessionSchedule** | 4 | 2 | — |
| **PayrollRecord** | 4 | 2 | — |
| **HRManager** | 2 | 3 | StaffMember, SessionSchedule |
| **ContentPartner** | 4 | 2 | — |
| **ContentBatch** | 5 | 2 | ContentPartner, Classroom, Lesson |
| **LiveSession** | 4 | 2 | Enrollment |
| **StudySchedule** | 4 | 2 | LiveSession, ContactAddress |
| **ProgressTracker** | 3 | 2 | — |
| **RefundRequest** | 5 | 2 | Enrollment, Course |
| **RefundHandler** | 2 | 1 | RefundRequest, BillingTransaction, Course |
| **EnrollmentAnalytics** | 2 | 2 | Enrollment |
| **CourseEngagement** | 3 | 3 | — |
| **ClassroomAudit** | 3 | 2 | Classroom |
| **FinancialVerifier** | 2 | 2 | Receipt, Enrollment |
| **CourseReview** | 5 | 2 | — |
| **RatingSummary** | 2 | 3 | CourseReview |
| **RecommendationService** | 2 | 2 | CourseCatalog, SkillTag, RatingSummary, Course |
| **AuditTrail** | 2 | 2 | Learner |

## Собственные классы исключений (13)

* `PlatformException` (базовое исключение предметной области)
* `CourseNotFoundException`
* `DuplicateCourseException`
* `SeatLimitExceededException`
* `InvalidCourseDataException`
* `InvalidAccountException`
* `EnrollmentNotFoundException`
* `InvalidBillingException`
* `InsufficientBalanceException`
* `LiveSessionException`
* `AccessDeniedException`
* `InvalidCouponException`
* `SubmissionNotFoundException`

## Структура проекта

```text
edtech_lab2_project/
├── .github/
│   └── workflows/
│       └── ci.yml
├── include/
│   ├── exceptions.hpp
│   ├── courses.hpp
│   ├── learners.hpp
│   ├── enrollments.hpp
│   └── operations.hpp
├── src/
│   ├── courses.cpp
│   ├── learners.cpp
│   ├── enrollments.cpp
│   ├── operations.cpp
│   └── main.cpp
├── tests/
│   └── unit_tests.cpp
├── Makefile
├── linter_check.py
├── Doxyfile
└── README.md
```

## Итоговая статистика проекта

| Показатель | Значение | Требование |
| :--- | :---: | :---: |
| **Доменные классы** | 60 | $\ge 50$ |
| **Поля (суммарно по классам)** | 190 | $\ge 150$ |
| **Содержательные поведения (методы)** | 164 | $\ge 100$ |
| **Ассоциации между классами** | 64 | $\ge 30$ |
| **Собственные исключения** | 13 | $\ge 12$ |
