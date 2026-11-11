/**
 * @file dictionary.cpp
 * @brief Implementation of the Dictionary class methods.
 */

#include "dictionary.hpp"
#include <stdexcept>

Dictionary::Dictionary() : root(nullptr) {}

Dictionary::~Dictionary() {
    clearTree(root);
}

void Dictionary::clearTree(Node* node) {
    if (!node) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

Dictionary::Node* Dictionary::cloneTree(const Node* node) const {
    if (!node) {
        return nullptr;
    }
    Node* newNode = new Node(node->engWord, node->rusTranslation);
    newNode->left = cloneTree(node->left);
    newNode->right = cloneTree(node->right);
    return newNode;
}

Dictionary::Dictionary(const Dictionary& other) : root(nullptr) {
    root = cloneTree(other.root);
}

Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (this != &other) {
        clearTree(root);
        root = cloneTree(other.root);
    }
    return *this;
}

Dictionary::Node* Dictionary::insertNode(Node* node, const std::string& eng, 
                                           const std::string& rus) {
    if (!node) {
        return new Node(eng, rus);
    }
    if (eng < node->engWord) {
        node->left = insertNode(node->left, eng, rus);
    } else if (eng > node->engWord) {
        node->right = insertNode(node->right, eng, rus);
    } else {
        node->rusTranslation = rus;
    }
    return node;
}

Dictionary& Dictionary::operator+=(const std::pair<std::string, std::string>& pair) {
    root = insertNode(root, pair.first, pair.second);
    return *this;
}

Dictionary& Dictionary::operator+=(const std::pair<const char*, const char*>& pair) {
    root = insertNode(root, std::string(pair.first), std::string(pair.second));
    return *this;
}

Dictionary::Node* Dictionary::findMinNode(Node* node) const {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

Dictionary::Node* Dictionary::removeNode(Node* node, const std::string& eng) {
    if (!node) {
        return nullptr;
    }
    if (eng < node->engWord) {
        node->left = removeNode(node->left, eng);
    } else if (eng > node->engWord) {
        node->right = removeNode(node->right, eng);
    } else {
        if (!node->left) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        if (!node->right) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        Node* temp = findMinNode(node->right);
        node->engWord = temp->engWord;
        node->rusTranslation = temp->rusTranslation;
        node->right = removeNode(node->right, temp->engWord);
    }
    return node;
}

Dictionary& Dictionary::operator-=(const std::string& engWord) {
    root = removeNode(root, engWord);
    return *this;
}

Dictionary::Node* Dictionary::findNode(Node* node, const std::string& eng) const {
    if (!node || node->engWord == eng) {
        return node;
    }
    if (eng < node->engWord) {
        return findNode(node->left, eng);
    }
    return findNode(node->right, eng);
}

std::string& Dictionary::operator[](const std::string& engWord) {
    Node* found = findNode(root, engWord);
    if (found) {
        return found->rusTranslation;
    }
    root = insertNode(root, engWord, "");
    return findNode(root, engWord)->rusTranslation;
}

const std::string& Dictionary::operator[](const std::string& engWord) const {
    Node* found = findNode(root, engWord);
    static const std::string kNotFoundMsg = "WORD_NOT_FOUND";
    if (found) {
        return found->rusTranslation;
    }
    return kNotFoundMsg;
}

bool Dictionary::compareTrees(const Node* lhs, const Node* rhs) const {
    if (!lhs && !rhs) {
        return true;
    }
    if (!lhs || !rhs) {
        return false;
    }
    return (lhs->engWord == rhs->engWord) && 
           (lhs->rusTranslation == rhs->rusTranslation) && 
           compareTrees(lhs->left, rhs->left) && 
           compareTrees(lhs->right, rhs->right);
}

bool Dictionary::operator==(const Dictionary& other) const {
    return size() == other.size() && compareTrees(root, other.root);
}

bool Dictionary::operator!=(const Dictionary& other) const {
    return !(*this == other);
}

size_t Dictionary::countNodes(Node* node) const {
    if (!node) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

size_t Dictionary::size() const {
    return countNodes(root);
}

void Dictionary::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open dictionary file: " + filename);
    }
    std::string eng, rus;
    while (file >> eng >> rus) {
        *this += {eng, rus};
    }
    file.close();
}