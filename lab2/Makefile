# Makefile for Lab 2 (Books Domain)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

all: library_app unit_tests

library_app: src/main.cpp src/book.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/book.cpp -o library_app

unit_tests: tests/unit_tests.cpp src/book.cpp
	$(CXX) $(CXXFLAGS) tests/unit_tests.cpp src/book.cpp -o unit_tests

clean:
	rm -f library_app unit_tests