# Makefile для Лабораторной работы №1 (Англо-русский словарь на BST)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

all: vocabulary unit_tests

vocabulary: src/main.cpp src/dictionary.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/dictionary.cpp -o vocabulary

unit_tests: src/unit_tests.cpp src/dictionary.cpp
	$(CXX) $(CXXFLAGS) src/unit_tests.cpp src/dictionary.cpp -o unit_tests

clean:
	rm -f vocabulary unit_tests