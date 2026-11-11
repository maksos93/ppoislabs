/**
 * @file dictionary.hpp
 * @brief Заголовочный файл класса Dictionary (Англо-русский словарь на BST).
 */

#pragma once

#include <string>
#include <utility>
#include <fstream>

/**
 * @class Dictionary
 * @brief Класс, реализующий англо-русский словарь на основе бинарного дерева поиска.
 */
class Dictionary {
private:
    /**
     * @struct Node
     * @brief Узел бинарного дерева поиска.
     */
    struct Node {
        std::string engWord;
        std::string rusTranslation;
        Node* left;
        Node* right;

        Node(const std::string& eng, const std::string& rus)
            : engWord(eng), rusTranslation(rus), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertNode(Node* node, const std::string& eng, const std::string& rus);
    Node* removeNode(Node* node, const std::string& eng);
    Node* findNode(Node* node, const std::string& eng) const;
    void clearTree(Node* node);
    Node* cloneTree(const Node* node) const;
    bool compareTrees(const Node* lhs, const Node* rhs) const;
    size_t countNodes(Node* node) const;
    Node* findMinNode(Node* node) const;

public:
    /** @brief Конструктор по умолчанию. Инициализирует пустое дерево. */
    Dictionary();

    /** @brief Деструктор. Освобождает выделенную память. */
    ~Dictionary();

    /** 
     * @brief Конструктор копирования.
     * @param other Ссылка на копируемый словарь.
     */
    Dictionary(const Dictionary& other);

    /** 
     * @brief Оператор присваивания копированием.
     * @param other Ссылка на копируемый словарь.
     * @return Ссылка на текущий объект.
     */
    Dictionary& operator=(const Dictionary& other);

    /** 
     * @brief Добавление слова через std::string.
     * @param pair Пара: английское слово и перевод.
     * @return Ссылка на текущий объект.
     */
    Dictionary& operator+=(const std::pair<std::string, std::string>& pair);

    /** 
     * @brief Добавление слова через C-строки (char*).
     * @param pair Пара C-строк: английское слово и перевод.
     * @return Ссылка на текущий объект.
     */
    Dictionary& operator+=(const std::pair<const char*, const char*>& pair);

    /** 
     * @brief Удаление слова из словаря.
     * @param engWord Английское слово для удаления.
     * @return Ссылка на текущий объект.
     */
    Dictionary& operator-=(const std::string& engWord);

    /** 
     * @brief Поиск или создание перевода для изменения.
     * @param engWord Английское слово.
     * @return Ссылка на перевод.
     */
    std::string& operator[](const std::string& engWord);

    /** 
     * @brief Константный поиск перевода (только для чтения).
     * @param engWord Английское слово.
     * @return Константная ссылка на перевод.
     */
    const std::string& operator[](const std::string& engWord) const;

    /** 
     * @brief Проверка словарей на равенство.
     * @param other Сравниваемый словарь.
     * @return true, если словари идентичны, иначе false.
     */
    bool operator==(const Dictionary& other) const;

    /** 
     * @brief Проверка словарей на неравенство.
     * @param other Сравниваемый словарь.
     * @return true, если словари отличаются.
     */
    bool operator!=(const Dictionary& other) const;

    /** 
     * @brief Получение количества слов в словаре.
     * @return Количество узлов в дереве.
     */
    size_t size() const;

    /** 
     * @brief Загрузка словаря из текстового файла.
     * @param filename Путь к файлу.
     * @throw std::runtime_error если файл не удалось открыть.
     */
    void loadFromFile(const std::string& filename);
};