# Лабораторная работа №2. Крупный ООП-проект

Предметная область: **книги** (Система управления библиотекой и книжным магазином). Модель описывает книги, авторов, издательства, жанры, склады, учет инвентаря, пользователей, читателей, заказы, оплату, сотрудников, доставку и систему аналитики.

Формат строки: `класс` | `число полей` | `число методов` | `связанные классы`.

## Спецификация объектной модели

| Класс | Поля | Методы | Ассоциации (связанные классы) |
| :--- | :---: | :---: | :--- |
| **Book** | 4 | 3 | Author, Publisher, Genre[cite: 7] |
| **Author** | 3 | 3 | Book |
| **Publisher** | 3 | 2 | Book |
| **Genre** | 2 | 2 | Book |
| **Category** | 3 | 2 | Genre |
| **Edition** | 4 | 3 | Book |
| **EBook** | 4 | 3 | Book |
| **AudioBook** | 4 | 3 | Book |
| **RareBook** | 4 | 3 | Book |
| **InventoryItem** | 4 | 3 | Warehouse, Book |
| **Warehouse** | 4 | 4 | Address, InventoryItem[cite: 8] |
| **Stock** | 3 | 3 | InventoryItem |
| **Shelf** | 3 | 2 | Warehouse[cite: 8] |
| **Section** | 3 | 2 | Shelf[cite: 8] |
| **CatalogManager** | 2 | 5 | Book, Author |
| **User** | 4 | 4 | Address |
| **Reader** | 4 | 4 | MembershipCard[cite: 7] |
| **Student** | 3 | 3 | Reader |
| **Professor** | 3 | 3 | Reader |
| **VIPCustomer** | 3 | 4 | LoyaltyAccount |
| **Guest** | 2 | 2 | — |
| **MembershipCard** | 4 | 3 | Reader |
| **LoyaltyAccount** | 3 | 4 | VIPCustomer |
| **NotificationPreferences** | 3 | 3 | User |
| **UserRegistry** | 2 | 5 | User |
| **Order** | 5 | 4 | Customer, OrderItem[cite: 7] |
| **OrderItem** | 4 | 3 | Book[cite: 7] |
| **Invoice** | 4 | 3 | Order, Customer[cite: 7] |
| **Payment** | 4 | 4 | Invoice |
| **CreditCardPayment** | 3 | 3 | Payment |
| **CashPayment** | 2 | 2 | Payment |
| **PayPalPayment** | 3 | 3 | Payment |
| **Discount** | 3 | 3 | Order |
| **Promotion** | 4 | 3 | Discount |
| **TransactionManager** | 2 | 5 | Payment, Order |
| **Employee** | 5 | 3 | Address |
| **Librarian** | 3 | 4 | Employee |
| **Cataloger** | 3 | 3 | Employee |
| **Cashier** | 3 | 3 | Employee |
| **Manager** | 4 | 5 | Employee, Order[cite: 7] |
| **Administrator** | 3 | 5 | Employee |
| **ShiftSchedule** | 4 | 3 | Employee |
| **Salary** | 4 | 3 | Employee |
| **HRDepartment** | 3 | 4 | Employee |
| **Supplier** | 4 | 3 | Book |
| **SupplyShipment** | 4 | 4 | Supplier, Warehouse[cite: 8] |
| **Courier** | 4 | 3 | Order[cite: 7] |
| **DeliveryRoute** | 4 | 3 | Courier, Address[cite: 8] |
| **TrackingSystem** | 3 | 4 | SupplyShipment, Order |
| **ReturnRequest** | 4 | 4 | Order, Book[cite: 7] |
| **RefundProcessor** | 3 | 3 | ReturnRequest, Payment |
| **SalesAnalytics** | 3 | 5 | Order[cite: 7] |
| **PopularityRating** | 3 | 4 | Book |
| **InventoryReport** | 3 | 3 | Warehouse[cite: 8] |
| **FinancialAuditor** | 3 | 4 | Invoice |
| **ReviewSystem** | 3 | 4 | Book, Reader[cite: 7] |
| **Rating** | 3 | 2 | ReviewSystem |
| **RecommendationEngine** | 3 | 4 | Book, Reader[cite: 7] |
| **ActivityLog** | 3 | 3 | User |
| **Address** | 4 | 3 | — |

## Собственные классы исключений (12)[cite: 6, 9]

* `LibraryException`[cite: 9]
* `BookNotFoundException`[cite: 9]
* `DuplicateBookException`[cite: 9]
* `OutOfStockException`[cite: 9]
* `InvalidUserException`[cite: 9]
* `OrderNotFoundException`[cite: 9]
* `InvalidPaymentException`[cite: 9]
* `InsufficientFundsException`[cite: 9]
* `DeliveryException`[cite: 9]
* `UnauthorizedAccessException`[cite: 9]
* `InvalidDiscountException`[cite: 9]
* `ReviewNotFoundException`[cite: 9]

## Итоговая статистика проекта

| Показатель | Значение | Требование |
| :--- | :---: | :---: |
| **Классы** | 60 | $\ge 50$[cite: 6] |
| **Поля** | 244 | $\ge 150$[cite: 6] |
| **Поведения (методы)** | 215 | $\ge 100$[cite: 6] |
| **Ассоциации** | 38 | $\ge 30$[cite: 6] |
| **Исключения** | 12 | $\ge 12$[cite: 6, 9] |