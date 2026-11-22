/**
 * @file book.cpp
 * @brief Implementation of Catalog, Books, Publishing, and Inventory classes.
 */

#include "book.hpp"
#include "exceptions.hpp"
#include <utility>

Address::Address(std::string street, std::string city, std::string postal, std::string country)
    : street(std::move(street)), city(std::move(city)), postalCode(std::move(postal)), country(std::move(country)) {}

std::string Address::formatFullAddress() const {
    return street + ", " + city + " " + postalCode + ", " + country;
}

bool Address::validatePostalCode() const {
    return postalCode.size() >= 4 && postalCode.size() <= 10;
}

bool Address::isSameCity(const Address& other) const {
    return !city.empty() && city == other.city;
}

Genre::Genre(std::string name, std::string desc)
    : name(std::move(name)), description(std::move(desc)) {}

bool Genre::matchesKeyword(const std::string& keyword) const {
    return name.find(keyword) != std::string::npos || description.find(keyword) != std::string::npos;
}

std::string Genre::generateTag() const {
    return "#" + name;
}

Category::Category(int id, std::string title, Genre genre)
    : id(id), title(std::move(title)), primaryGenre(std::move(genre)) {}

void Category::assignGenre(const Genre& genre) {
    primaryGenre = genre;
}

std::string Category::buildBreadcrumb() const {
    return primaryGenre.getName() + " > " + title;
}

Author::Author(int id, std::string name, std::string country)
    : id(id), fullName(std::move(name)), country(std::move(country)) {}

std::string Author::formatCitation() const {
    return fullName + " (" + country + ")";
}

bool Author::isFromCountry(const std::string& targetCountry) const {
    return country == targetCountry;
}

void Author::updateBiographyCountry(const std::string& newCountry) {
    country = newCountry;
}

Publisher::Publisher(std::string name, std::string code, Address addr)
    : name(std::move(name)), registrationCode(std::move(code)), officeAddress(std::move(addr)) {}

bool Publisher::verifyLicense() const {
    return !registrationCode.empty() && registrationCode != "000";
}

std::string Publisher::generateImprintHeader() const {
    return name + " [" + registrationCode + "] - " + officeAddress.getCity();
}

Book::Book(const std::string& isbn, const std::string& title, const Author& author,
           const Publisher& publisher, const Genre& genre, double price, int stock)
    : isbn(isbn), title(title), author(author), publisher(publisher),
      genre(genre), price(price), stockQuantity(stock) {
    if (price < 0.0) {
        throw InvalidBookDataException("Book price cannot be negative.");
    }
    if (stock < 0) {
        throw InvalidBookDataException("Stock quantity cannot be negative.");
    }
}

void Book::updatePrice(double newPrice) {
    if (newPrice < 0.0) {
        throw InvalidBookDataException("Book price cannot be negative.");
    }
    price = newPrice;
}

void Book::restock(int amount) {
    if (amount <= 0) {
        throw InvalidBookDataException("Restock amount must be positive.");
    }
    stockQuantity += amount;
}

void Book::reserveCopies(int amount) {
    if (amount <= 0) {
        throw InvalidBookDataException("Reservation amount must be positive.");
    }
    if (stockQuantity < amount) {
        throw OutOfStockException(title);
    }
    stockQuantity -= amount;
}

double Book::calculateDiscountedPrice(double percent) const {
    if (percent < 0.0 || percent > 100.0) {
        throw InvalidDiscountException(percent);
    }
    return price * (1.0 - percent / 100.0);
}

std::string Book::describeFormat() const {
    return "Standard Print Book: " + title;
}

Edition::Edition(const Book& base, int editionNum, int year, std::string cover)
    : Book(base), editionNumber(editionNum), releaseYear(year), coverType(std::move(cover)) {}

bool Edition::isFirstEdition() const {
    return editionNumber == 1;
}

int Edition::calculateAge(int currentYear) const {
    return currentYear >= releaseYear ? (currentYear - releaseYear) : 0;
}

std::string Edition::describeFormat() const {
    return "Edition #" + std::to_string(editionNumber) + " (" + coverType + ")";
}

EBook::EBook(const Book& base, std::string format, double sizeMb, bool drm)
    : Book(base), fileFormat(std::move(format)), fileSizeMb(sizeMb), drmProtected(drm) {}

double EBook::estimateDownloadSeconds(double speedMbps) const {
    if (speedMbps <= 0.0) return 0.0;
    return (fileSizeMb * 8.0) / speedMbps;
}

void EBook::toggleDrmProtection(bool enabled) {
    drmProtected = enabled;
}

std::string EBook::describeFormat() const {
    return "EBook [" + fileFormat + ", " + std::to_string(fileSizeMb) + " MB]";
}

AudioBook::AudioBook(const Book& base, std::string narrator, int minutes, int bitrate)
    : Book(base), narrator(std::move(narrator)), durationMinutes(minutes), bitrateKbps(bitrate) {}

double AudioBook::calculateListeningHours(double playbackSpeed) const {
    if (playbackSpeed <= 0.0) return 0.0;
    return (durationMinutes / 60.0) / playbackSpeed;
}

double AudioBook::estimateStorageMegabytes() const {
    return (durationMinutes * 60.0 * bitrateKbps) / 8192.0;
}

std::string AudioBook::describeFormat() const {
    return "AudioBook narrated by " + narrator;
}

RareBook::RareBook(const Book& base, int year, std::string grade, double value)
    : Book(base), publicationYear(year), conditionGrade(std::move(grade)), appraisedValue(value) {}

double RareBook::computeInsurancePremium(double rate) const {
    return appraisedValue * rate;
}

void RareBook::reappraise(double newValue, const std::string& newGrade) {
    if (newValue < 0.0) {
        throw InvalidBookDataException("Appraised value cannot be negative.");
    }
    appraisedValue = newValue;
    conditionGrade = newGrade;
}

std::string RareBook::describeFormat() const {
    return "RareBook (" + std::to_string(publicationYear) + ", Grade: " + conditionGrade + ")";
}

InventoryItem::InventoryItem(std::string sku, Book book, int qty, int threshold)
    : sku(std::move(sku)), book(std::move(book)), quantityOnHand(qty), reorderThreshold(threshold) {}

bool InventoryItem::needsReorder() const {
    return quantityOnHand <= reorderThreshold;
}

void InventoryItem::adjustStock(int delta) {
    if (quantityOnHand + delta < 0) {
        throw OutOfStockException(book.getTitle());
    }
    quantityOnHand += delta;
}

double InventoryItem::calculateTotalValuation() const {
    return quantityOnHand * book.getPrice();
}

Stock::Stock(std::string batch) : batchCode(std::move(batch)), totalUnits(0) {}

void Stock::registerItem(const InventoryItem& item) {
    items.push_back(item);
    totalUnits += 1;
}

int Stock::countLowStockItems() const {
    int count = 0;
    for (const auto& item : items) {
        if (item.needsReorder()) count++;
    }
    return count;
}

double Stock::computeBatchValue() const {
    double sum = 0.0;
    for (const auto& item : items) {
        sum += item.calculateTotalValuation();
    }
    return sum;
}

Shelf::Shelf(std::string code, int capacity)
    : shelfCode(std::move(code)), capacity(capacity), currentLoad(0) {}

bool Shelf::allocateSpace(int units) {
    if (currentLoad + units > capacity) return false;
    currentLoad += units;
    return true;
}

void Shelf::releaseSpace(int units) {
    currentLoad = (units >= currentLoad) ? 0 : (currentLoad - units);
}

double Shelf::getOccupancyRate() const {
    return capacity > 0 ? static_cast<double>(currentLoad) / capacity : 0.0;
}

Section::Section(std::string name, Genre genre)
    : sectionName(std::move(name)), assignedGenre(std::move(genre)) {}

void Section::addShelf(const Shelf& shelf) {
    shelves.push_back(shelf);
}

double Section::calculateAverageOccupancy() const {
    if (shelves.empty()) return 0.0;
    double total = 0.0;
    for (const auto& s : shelves) {
        total += s.getOccupancyRate();
    }
    return total / shelves.size();
}

Warehouse::Warehouse(std::string id, Address addr)
    : warehouseId(std::move(id)), location(std::move(addr)), activeStock("MAIN-BATCH") {}

void Warehouse::addSection(const Section& section) {
    sections.push_back(section);
}

void Warehouse::receiveInventory(const InventoryItem& item) {
    activeStock.registerItem(item);
}

bool Warehouse::isLocatedInCity(const std::string& city) const {
    return location.getCity() == city;
}

double Warehouse::evaluateTotalAssetValue() const {
    return activeStock.computeBatchValue();
}

void CatalogManager::addBook(const Book& book) {
    if (books.find(book.getIsbn()) != books.end()) {
        throw DuplicateBookException(book.getIsbn());
    }
    books.insert({book.getIsbn(), book});
}

void CatalogManager::removeBook(const std::string& isbn) {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    books.erase(it);
}

Book& CatalogManager::getBook(const std::string& isbn) {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    return it->second;
}

const Book& CatalogManager::getBook(const std::string& isbn) const {
    auto it = books.find(isbn);
    if (it == books.end()) {
        throw BookNotFoundException(isbn);
    }
    return it->second;
}

std::vector<Book> CatalogManager::searchByAuthor(const std::string& authorName) const {
    std::vector<Book> result;
    for (const auto& pair : books) {
        if (pair.second.getAuthor().getFullName() == authorName) {
            result.push_back(pair.second);
        }
    }
    return result;
}

void CatalogManager::registerFeaturedAuthor(const Author& author) {
    featuredAuthors.push_back(author);
}
