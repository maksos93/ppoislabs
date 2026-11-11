/**
 * @file main.cpp
 * @brief CLI application for interacting with the Dictionary.
 */

#include <iostream>
#include "dictionary.hpp"

namespace {
    const int kExitChoice = 0;
    const int kAddWord = 1;
    const int kSearchWord = 2;
    const int kRemoveWord = 3;
    const int kShowSize = 4;
    const int kLoadFile = 5;

    void printMenu() {
        std::cout << "\n=== ENGLISH-RUSSIAN DICTIONARY ===\n"
                  << "1. Add word (+=)\n"
                  << "2. Find or update translation ([])\n"
                  << "3. Remove word (-=)\n"
                  << "4. Show dictionary size\n"
                  << "5. Load dictionary from file\n"
                  << "0. Exit\n"
                  << "Select option: ";
    }

    void handleAdd(Dictionary& dict) {
        std::string eng, rus;
        std::cout << "Enter English word: ";
        std::cin >> eng;
        std::cout << "Enter Russian translation: ";
        std::cin >> rus;
        dict += {eng, rus};
        std::cout << "Word added successfully.\n";
    }

    void handleSearch(Dictionary& dict) {
        std::string eng;
        std::cout << "Enter English word: ";
        std::cin >> eng;
        std::cout << "Translation: " << dict[eng] << "\n";
        
        std::cout << "Update translation? (y/n): ";
        char answer;
        std::cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            std::string newRus;
            std::cout << "Enter new translation: ";
            std::cin >> newRus;
            dict[eng] = newRus;
            std::cout << "Translation updated.\n";
        }
    }

    void handleRemove(Dictionary& dict) {
        std::string eng;
        std::cout << "Enter English word to remove: ";
        std::cin >> eng;
        dict -= eng;
        std::cout << "Operation completed.\n";
    }

    void handleLoad(Dictionary& dict) {
        std::string filename;
        std::cout << "Enter file path: ";
        std::cin >> filename;
        try {
            dict.loadFromFile(filename);
            std::cout << "Dictionary loaded successfully.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

int main() {
    Dictionary dict;
    int choice = -1;

    do {
        printMenu();
        if (!(std::cin >> choice)) {
            break;
        }

        switch (choice) {
            case kAddWord: handleAdd(dict); break;
            case kSearchWord: handleSearch(dict); break;
            case kRemoveWord: handleRemove(dict); break;
            case kShowSize: std::cout << "Total words: " << dict.size() << "\n"; break;
            case kLoadFile: handleLoad(dict); break;
            case kExitChoice: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice. Try again.\n"; break;
        }
    } while (choice != kExitChoice);

    return 0;
}