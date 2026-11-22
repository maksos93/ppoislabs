# Лабораторная работа №2. Крупный ООП-проект

Предметная область: **книги** (Система управления библиотекой и книжным магазином). Модель охватывает книги, авторов, издательства, жанры, склады, учет инвентаря, пользователей, читателей, заказы, оплату, сотрудников, логистику, возвраты, отзывы и аналитику.

Формат строки: `Класс | число полей | число методов | связанные классы`.

## Спецификация объектной модели (60 доменных классов)

| Класс | Поля | Методы | Ассоциации (связанные классы) |
| :--- | :---: | :---: | :--- |
| **Address** | 4 | 3 | — |
| **Genre** | 2 | 2 | — |
| **Category** | 3 | 2 | Genre |
| **Author** | 3 | 3 | — |
| **Publisher** | 3 | 2 | Address |
| **Book** | 7 | 5 | Author, Publisher, Genre |
| **Edition** | 3 | 3 | Book |
| **EBook** | 3 | 3 | Book |
| **AudioBook** | 3 | 3 | Book |
| **RareBook** | 3 | 3 | Book |
| **InventoryItem** | 4 | 3 | Book |
| **Stock** | 3 | 3 | InventoryItem |
| **Shelf** | 3 | 3 | — |
| **Section** | 3 | 2 | Shelf, Genre |
| **Warehouse** | 4 | 4 | Address, Section, Stock, InventoryItem |
| **CatalogManager** | 2 | 4 | Book, Author |
| **NotificationPreferences** | 3 | 3 | — |
| **User** | 5 | 4 | Address, NotificationPreferences |
| **MembershipCard** | 4 | 4 | — |
| **Reader** | 3 | 5 | User, MembershipCard, Book |
| **Student** | 3 | 3 | Reader |
| **Professor** | 3 | 3 | Reader |
| **LoyaltyAccount** | 3 | 3 | — |
| **VIPCustomer** | 3 | 4 | User, LoyaltyAccount |
| **Guest** | 3 | 2 | — |
| **UserRegistry** | 2 | 4 | User, Address |
| **OrderItem** | 3 | 3 | Book |
| **Discount** | 3 | 2 | — |
| **Promotion** | 4 | 3 | Discount |
| **Order** | 5 | 5 | User, OrderItem, Book, Discount |
| **Invoice** | 4 | 3 | Order |
| **Payment** | 4 | 2 | Invoice |
| **CreditCardPayment** | 3 | 2 | Payment, Invoice |
| **CashPayment** | 2 | 2 | Payment, Invoice |
| **PayPalPayment** | 2 | 2 | Payment, Invoice |
| **TransactionManager** | 2 | 3 | Invoice, Order, Payment |
| **Employee** | 5 | 3 | Address |
| **Librarian** | 2 | 2 | Employee, Reader, Book |
| **Cataloger** | 2 | 2 | Employee, CatalogManager, Book |
| **Cashier** | 2 | 2 | Employee |
| **Manager** | 2 | 2 | Employee, Order, Discount |
| **Administrator** | 2 | 2 | Employee, MembershipCard |
| **ShiftSchedule** | 4 | 2 | — |
| **Salary** | 4 | 2 | — |
| **HRDepartment** | 2 | 3 | Employee, ShiftSchedule |
| **Supplier** | 4 | 2 | — |
| **SupplyShipment** | 5 | 2 | Supplier, Warehouse, InventoryItem |
| **Courier** | 4 | 2 | Order |
| **DeliveryRoute** | 4 | 2 | Courier, Address |
| **TrackingSystem** | 3 | 2 | — |
| **ReturnRequest** | 5 | 2 | Order, Book |
| **RefundProcessor** | 2 | 1 | ReturnRequest, Payment, Book |
| **SalesAnalytics** | 2 | 2 | Order |
| **PopularityRating** | 3 | 3 | — |
| **InventoryReport** | 3 | 2 | Warehouse |
| **FinancialAuditor** | 2 | 2 | Invoice, Order |
| **ReviewSystem** | 5 | 2 | — |
| **Rating** | 2 | 3 | ReviewSystem |
| **RecommendationEngine** | 2 | 2 | CatalogManager, Genre, Rating, Book |
| **ActivityLog** | 2 | 2 | User |

## Собственные классы исключений (13)

* `LibraryException` (базовое исключение предметной области)
* `BookNotFoundException`
* `DuplicateBookException`
* `OutOfStockException`
* `InvalidBookDataException`
* `InvalidUserException`
* `OrderNotFoundException`
* `InvalidPaymentException`
* `InsufficientFundsException`
* `DeliveryException`
* `UnauthorizedAccessException`
* `InvalidDiscountException`
* `ReviewNotFoundException`

## Структура проекта

```text
lab2_project/
├── include/
│   ├── exceptions.hpp
│   ├── book.hpp
│   ├── users.hpp
│   ├── orders.hpp
│   └── staff.hpp
├── src/
│   ├── book.cpp
│   ├── users.cpp
│   ├── orders.cpp
│   ├── staff.cpp
│   └── main.cpp
├── tests/
│   └── unit_tests.cpp
├── Makefile
├── linter_check.py
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
