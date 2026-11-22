/**
 * @file book.hpp
 * @brief Catalog, Books, Publishing, and Inventory management definitions.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * @class Address
 * @brief Represents a physical postal address for users, publishers, warehouses, and delivery routes.
 */
class Address {
private:
    std::string street;     ///< Street name and building number.
    std::string city;       ///< City or locality name.
    std::string postalCode; ///< Postal or ZIP code.
    std::string country;    ///< Country name.
public:
    /**
     * @brief Constructs an Address object.
     * @param street Street line.
     * @param city City name.
     * @param postal Postal code.
     * @param country Country name.
     */
    Address(std::string street = "", std::string city = "", std::string postal = "", std::string country = "");

    /**
     * @brief Formats all address components into a single human-readable string.
     * @return Formatted full postal address.
     */
    std::string formatFullAddress() const;

    /**
     * @brief Validates whether the postal code length falls within standard bounds (4..10 chars).
     * @return True if postal code is valid, false otherwise.
     */
    bool validatePostalCode() const;

    /**
     * @brief Checks whether another address is located in the same city.
     * @param other Another Address instance to compare against.
     * @return True if both addresses share a non-empty city name.
     */
    bool isSameCity(const Address& other) const;

    /**
     * @brief Returns the city name.
     * @return City string.
     */
    std::string getCity() const { return city; }
};

/**
 * @class Genre
 * @brief Represents a literary genre or subject classification.
 */
class Genre {
private:
    std::string name;        ///< Genre title (e.g., Programming, Fiction).
    std::string description; ///< Brief description of the genre scope.
public:
    /**
     * @brief Constructs a Genre instance.
     * @param name Genre name.
     * @param desc Genre description.
     */
    Genre(std::string name = "General", std::string desc = "");

    /**
     * @brief Checks if the genre name or description contains a given keyword.
     * @param keyword Substring to search for.
     * @return True if keyword matches name or description.
     */
    bool matchesKeyword(const std::string& keyword) const;

    /**
     * @brief Generates a hashtag identifier for catalog filtering.
     * @return Tag string prefixed with '#'.
     */
    std::string generateTag() const;

    /**
     * @brief Returns the genre name.
     * @return Genre name string.
     */
    std::string getName() const { return name; }
};

/**
 * @class Category
 * @brief Represents a catalog category linked to a primary literary Genre.
 */
class Category {
private:
    int id;                 ///< Unique numeric category identifier.
    std::string title;      ///< Category display title.
    Genre primaryGenre;     ///< Associated primary genre.
public:
    /**
     * @brief Constructs a Category object.
     * @param id Category identifier.
     * @param title Category name.
     * @param genre Primary Genre associated with this category.
     */
    Category(int id = 0, std::string title = "Uncategorized", Genre genre = Genre());

    /**
     * @brief Reassigns the primary genre of the category.
     * @param genre New Genre to associate.
     */
    void assignGenre(const Genre& genre);

    /**
     * @brief Builds a navigation breadcrumb combining genre and category title.
     * @return Breadcrumb string in format "Genre > Category".
     */
    std::string buildBreadcrumb() const;

    /**
     * @brief Returns the category title.
     * @return Category title string.
     */
    std::string getTitle() const { return title; }
};

/**
 * @class Author
 * @brief Represents a book author with biographical metadata.
 */
class Author {
private:
    int id;               ///< Unique author identifier.
    std::string fullName; ///< Author's full name.
    std::string country;  ///< Author's country of origin.
public:
    /**
     * @brief Constructs an Author object.
     * @param id Unique numeric ID.
     * @param name Full name of the author.
     * @param country Country of origin.
     */
    Author(int id = 0, std::string name = "Unknown", std::string country = "");

    /**
     * @brief Formats a bibliographic citation string for the author.
     * @return Citation formatted as "Full Name (Country)".
     */
    std::string formatCitation() const;

    /**
     * @brief Checks whether the author originates from a specific country.
     * @param targetCountry Country name to verify.
     * @return True if the author's country matches targetCountry.
     */
    bool isFromCountry(const std::string& targetCountry) const;

    /**
     * @brief Updates the author's recorded country.
     * @param newCountry Updated country name.
     */
    void updateBiographyCountry(const std::string& newCountry);

    /**
     * @brief Returns the author's full name.
     * @return Full name string.
     */
    std::string getFullName() const { return fullName; }
};

/**
 * @class Publisher
 * @brief Represents a publishing house with licensing code and office address.
 */
class Publisher {
private:
    std::string name;             ///< Official publisher name.
    std::string registrationCode; ///< Business registration / ISBN prefix code.
    Address officeAddress;        ///< Headquarters address.
public:
    /**
     * @brief Constructs a Publisher instance.
     * @param name Publisher name.
     * @param code Registration code.
     * @param addr Office Address.
     */
    Publisher(std::string name = "Independent", std::string code = "000", Address addr = Address());

    /**
     * @brief Verifies whether the publisher holds a valid non-default registration code.
     * @return True if registration code is valid.
     */
    bool verifyLicense() const;

    /**
     * @brief Generates a formatted imprint header for book title pages.
     * @return Imprint string containing publisher name, code, and city.
     */
    std::string generateImprintHeader() const;

    /**
     * @brief Returns the publisher name.
     * @return Publisher name string.
     */
    std::string getName() const { return name; }
};

/**
 * @class Book
 * @brief Core domain entity representing a book in the library and bookstore catalog.
 */
class Book {
protected:
    std::string isbn;        ///< International Standard Book Number.
    std::string title;       ///< Book title.
    Author author;           ///< Author of the book.
    Publisher publisher;     ///< Publishing house.
    Genre genre;             ///< Literary genre.
    double price;            ///< Retail unit price.
    int stockQuantity;       ///< Available copies in stock.
public:
    /**
     * @brief Constructs a Book and validates price and stock non-negativity.
     * @param isbn Unique ISBN string.
     * @param title Book title.
     * @param author Book Author object.
     * @param publisher Book Publisher object.
     * @param genre Book Genre object.
     * @param price Retail price (must be >= 0).
     * @param stock Initial stock count (must be >= 0).
     * @throws InvalidBookDataException if price or stock is negative.
     */
    Book(const std::string& isbn = "", const std::string& title = "",
         const Author& author = Author(), const Publisher& publisher = Publisher(),
         const Genre& genre = Genre(), double price = 0.0, int stock = 0);

    /**
     * @brief Virtual destructor for polymorphic book subclasses.
     */
    virtual ~Book() = default;

    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    Author getAuthor() const { return author; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }

    /**
     * @brief Updates the retail price of the book.
     * @param newPrice New retail price (must be >= 0).
     * @throws InvalidBookDataException if newPrice is negative.
     */
    void updatePrice(double newPrice);

    /**
     * @brief Increases available stock by a positive number of copies.
     * @param amount Number of copies to add (must be > 0).
     * @throws InvalidBookDataException if amount <= 0.
     */
    void restock(int amount);

    /**
     * @brief Reserves and deducts copies from stock for borrowing or purchase.
     * @param amount Number of copies to reserve (must be > 0).
     * @throws InvalidBookDataException if amount <= 0.
     * @throws OutOfStockException if stockQuantity < amount.
     */
    void reserveCopies(int amount);

    /**
     * @brief Calculates the book price after applying a percentage discount.
     * @param percent Discount percentage in range [0, 100].
     * @return Discounted price.
     * @throws InvalidDiscountException if percent is outside [0, 100].
     */
    double calculateDiscountedPrice(double percent) const;

    /**
     * @brief Returns a human-readable description of the book format.
     * @return Format description string.
     */
    virtual std::string describeFormat() const;
};

/**
 * @class Edition
 * @brief Represents a specific print edition of a Book.
 */
class Edition : public Book {
private:
    int editionNumber;     ///< Sequential edition number (e.g., 1st, 2nd).
    int releaseYear;       ///< Publication year of this edition.
    std::string coverType; ///< Cover binding type (e.g., Hardcover, Paperback).
public:
    /**
     * @brief Constructs an Edition from a base Book.
     * @param base Base Book entity.
     * @param editionNum Edition number.
     * @param year Release year.
     * @param cover Cover binding description.
     */
    Edition(const Book& base, int editionNum = 1, int year = 2024, std::string cover = "Hardcover");

    /**
     * @brief Checks if this is a first edition print.
     * @return True if editionNumber == 1.
     */
    bool isFirstEdition() const;

    /**
     * @brief Calculates how many years have passed since this edition's release.
     * @param currentYear Reference calendar year.
     * @return Age in years (0 if currentYear < releaseYear).
     */
    int calculateAge(int currentYear) const;

    /**
     * @brief Describes the edition format and cover binding.
     * @return Formatted edition description.
     */
    std::string describeFormat() const override;
};

/**
 * @class EBook
 * @brief Represents a digital electronic book with file size and DRM properties.
 */
class EBook : public Book {
private:
    std::string fileFormat; ///< Digital format (e.g., EPUB, PDF).
    double fileSizeMb;      ///< File size in megabytes.
    bool drmProtected;      ///< Whether Digital Rights Management is active.
public:
    /**
     * @brief Constructs an EBook from a base Book.
     * @param base Base Book entity.
     * @param format File format string.
     * @param sizeMb Size in megabytes.
     * @param drm DRM protection flag.
     */
    EBook(const Book& base, std::string format = "EPUB", double sizeMb = 2.5, bool drm = true);

    /**
     * @brief Estimates download time in seconds for a given network bandwidth.
     * @param speedMbps Connection speed in megabits per second.
     * @return Estimated download duration in seconds.
     */
    double estimateDownloadSeconds(double speedMbps) const;

    /**
     * @brief Enables or disables DRM protection on the e-book file.
     * @param enabled Desired DRM state.
     */
    void toggleDrmProtection(bool enabled);

    /**
     * @brief Describes the electronic book format and file size.
     * @return Formatted EBook description.
     */
    std::string describeFormat() const override;
};

/**
 * @class AudioBook
 * @brief Represents an audio recording of a Book with narrator and duration metadata.
 */
class AudioBook : public Book {
private:
    std::string narrator; ///< Name of the voice narrator.
    int durationMinutes;  ///< Total audio length in minutes.
    int bitrateKbps;      ///< Audio encoding bitrate in kbps.
public:
    /**
     * @brief Constructs an AudioBook from a base Book.
     * @param base Base Book entity.
     * @param narrator Narrator name.
     * @param minutes Total playback duration in minutes.
     * @param bitrate Audio bitrate in kbps.
     */
    AudioBook(const Book& base, std::string narrator = "Narrator", int minutes = 300, int bitrate = 128);

    /**
     * @brief Calculates total listening time in hours at a given playback speed.
     * @param playbackSpeed Speed multiplier (e.g., 1.0, 1.5).
     * @return Listening time in hours.
     */
    double calculateListeningHours(double playbackSpeed) const;

    /**
     * @brief Estimates audio storage size in megabytes from duration and bitrate.
     * @return Estimated size in MB.
     */
    double estimateStorageMegabytes() const;

    /**
     * @brief Describes the audiobook and its narrator.
     * @return Formatted AudioBook description.
     */
    std::string describeFormat() const override;
};

/**
 * @class RareBook
 * @brief Represents an antiquarian or collectible book requiring appraisal and insurance.
 */
class RareBook : public Book {
private:
    int publicationYear;       ///< Historical year of publication.
    std::string conditionGrade;///< Collectible condition grade (e.g., Mint, Fine).
    double appraisedValue;     ///< Appraised monetary value.
public:
    /**
     * @brief Constructs a RareBook from a base Book.
     * @param base Base Book entity.
     * @param year Year of original publication.
     * @param grade Condition grade string.
     * @param value Appraised value in currency units.
     */
    RareBook(const Book& base, int year = 1900, std::string grade = "Fine", double value = 500.0);

    /**
     * @brief Computes insurance premium based on appraised value and rate.
     * @param rate Insurance rate multiplier (e.g., 0.05 for 5%).
     * @return Calculated insurance premium.
     */
    double computeInsurancePremium(double rate) const;

    /**
     * @brief Updates the appraised valuation and physical condition grade.
     * @param newValue Updated appraised value (must be >= 0).
     * @param newGrade Updated condition grade.
     * @throws InvalidBookDataException if newValue is negative.
     */
    void reappraise(double newValue, const std::string& newGrade);

    /**
     * @brief Describes the rare book publication year and condition grade.
     * @return Formatted RareBook description.
     */
    std::string describeFormat() const override;
};

/**
 * @class InventoryItem
 * @brief Tracks warehouse SKU, stock quantity, and reorder threshold for a Book.
 */
class InventoryItem {
private:
    std::string sku;      ///< Stock Keeping Unit code.
    Book book;            ///< Associated Book item.
    int quantityOnHand;   ///< Physical units currently stored.
    int reorderThreshold; ///< Minimum quantity before restock is triggered.
public:
    /**
     * @brief Constructs an InventoryItem record.
     * @param sku Unique SKU identifier.
     * @param book Associated Book.
     * @param qty Initial quantity on hand.
     * @param threshold Low-stock threshold.
     */
    InventoryItem(std::string sku = "", Book book = Book(), int qty = 0, int threshold = 5);

    /**
     * @brief Determines whether current quantity has reached or fallen below reorderThreshold.
     * @return True if restocking is needed.
     */
    bool needsReorder() const;

    /**
     * @brief Adjusts on-hand stock by a positive or negative delta.
     * @param delta Quantity change.
     * @throws OutOfStockException if resulting quantity would be negative.
     */
    void adjustStock(int delta);

    /**
     * @brief Calculates total retail valuation of units on hand.
     * @return Total value (quantityOnHand * book price).
     */
    double calculateTotalValuation() const;

    std::string getSku() const { return sku; }
};

/**
 * @class Stock
 * @brief Aggregates a batch of InventoryItem entries for valuation and low-stock monitoring.
 */
class Stock {
private:
    std::string batchCode;            ///< Batch identifier.
    std::vector<InventoryItem> items; ///< Collection of inventory items in this batch.
    int totalUnits;                   ///< Number of registered item entries.
public:
    /**
     * @brief Constructs a Stock batch.
     * @param batch Batch identifier string.
     */
    Stock(std::string batch = "BATCH-01");

    /**
     * @brief Registers a new InventoryItem into the stock batch.
     * @param item InventoryItem to add.
     */
    void registerItem(const InventoryItem& item);

    /**
     * @brief Counts how many items in the batch require reordering.
     * @return Number of low-stock items.
     */
    int countLowStockItems() const;

    /**
     * @brief Computes total monetary valuation across all registered items.
     * @return Cumulative batch valuation.
     */
    double computeBatchValue() const;
};

/**
 * @class Shelf
 * @brief Represents a physical storage shelf with unit capacity tracking.
 */
class Shelf {
private:
    std::string shelfCode; ///< Shelf location code (e.g., A1).
    int capacity;          ///< Maximum book unit capacity.
    int currentLoad;       ///< Currently occupied units.
public:
    /**
     * @brief Constructs a Shelf with a code and maximum capacity.
     * @param code Shelf identifier.
     * @param capacity Maximum unit capacity.
     */
    Shelf(std::string code = "A1", int capacity = 100);

    /**
     * @brief Attempts to allocate space on the shelf for incoming units.
     * @param units Number of units to place.
     * @return True if space was allocated, false if capacity would be exceeded.
     */
    bool allocateSpace(int units);

    /**
     * @brief Frees occupied space on the shelf.
     * @param units Number of units removed.
     */
    void releaseSpace(int units);

    /**
     * @brief Calculates current occupancy ratio in [0.0, 1.0].
     * @return Occupancy rate.
     */
    double getOccupancyRate() const;
};

/**
 * @class Section
 * @brief Groups multiple Shelf units under a specific literary Genre inside a warehouse or hall.
 */
class Section {
private:
    std::string sectionName;   ///< Section title.
    std::vector<Shelf> shelves;///< Shelves belonging to this section.
    Genre assignedGenre;       ///< Genre assigned to this section.
public:
    /**
     * @brief Constructs a Section with a name and assigned Genre.
     * @param name Section name.
     * @param genre Assigned Genre.
     */
    Section(std::string name = "Main", Genre genre = Genre());

    /**
     * @brief Adds a Shelf to the section.
     * @param shelf Shelf instance to append.
     */
    void addShelf(const Shelf& shelf);

    /**
     * @brief Computes the average occupancy rate across all shelves in the section.
     * @return Average occupancy ratio (0.0 if no shelves exist).
     */
    double calculateAverageOccupancy() const;
};

/**
 * @class Warehouse
 * @brief Represents a physical storage facility holding sections and active stock batches.
 */
class Warehouse {
private:
    std::string warehouseId;       ///< Unique warehouse identifier.
    Address location;              ///< Physical warehouse address.
    std::vector<Section> sections; ///< Storage sections inside the warehouse.
    Stock activeStock;             ///< Active inventory stock batch.
public:
    /**
     * @brief Constructs a Warehouse at a given Address.
     * @param id Warehouse code.
     * @param addr Physical Address.
     */
    Warehouse(std::string id = "WH-01", Address addr = Address());

    /**
     * @brief Adds a storage Section to the warehouse.
     * @param section Section to add.
     */
    void addSection(const Section& section);

    /**
     * @brief Registers incoming inventory into the warehouse's active stock.
     * @param item Incoming InventoryItem.
     */
    void receiveInventory(const InventoryItem& item);

    /**
     * @brief Checks whether the warehouse is situated in the specified city.
     * @param city City name to check.
     * @return True if located in city.
     */
    bool isLocatedInCity(const std::string& city) const;

    /**
     * @brief Evaluates total monetary value of all stock stored in the warehouse.
     * @return Total asset valuation.
     */
    double evaluateTotalAssetValue() const;
};

/**
 * @class CatalogManager
 * @brief Central registry for managing books by ISBN and featured authors.
 */
class CatalogManager {
private:
    std::map<std::string, Book> books;    ///< Map of ISBN to Book records.
    std::vector<Author> featuredAuthors;  ///< Highlighted authors in the catalog.
public:
    CatalogManager() = default;

    /**
     * @brief Adds a new Book to the catalog.
     * @param book Book instance to insert.
     * @throws DuplicateBookException if a book with the same ISBN already exists.
     */
    void addBook(const Book& book);

    /**
     * @brief Removes a Book from the catalog by ISBN.
     * @param isbn ISBN of the book to remove.
     * @throws BookNotFoundException if the ISBN is not present.
     */
    void removeBook(const std::string& isbn);

    /**
     * @brief Retrieves a mutable reference to a Book by ISBN.
     * @param isbn ISBN to look up.
     * @return Reference to the matching Book.
     * @throws BookNotFoundException if the ISBN is not found.
     */
    Book& getBook(const std::string& isbn);

    /**
     * @brief Retrieves a const reference to a Book by ISBN.
     * @param isbn ISBN to look up.
     * @return Const reference to the matching Book.
     * @throws BookNotFoundException if the ISBN is not found.
     */
    const Book& getBook(const std::string& isbn) const;

    /**
     * @brief Searches the catalog for all books written by a given author.
     * @param authorName Exact author full name to match.
     * @return Vector of matching Book objects.
     */
    std::vector<Book> searchByAuthor(const std::string& authorName) const;

    /**
     * @brief Adds an Author to the featured authors list.
     * @param author Author to feature.
     */
    void registerFeaturedAuthor(const Author& author);

    /**
     * @brief Returns total number of distinct books in the catalog.
     * @return Number of catalog entries.
     */
    size_t getBookCount() const { return books.size(); }
};

using Catalog = CatalogManager;