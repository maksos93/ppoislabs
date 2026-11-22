# Makefile for Lab 2 (Library and Bookstore Domain)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRCS = src/book.cpp src/users.cpp src/orders.cpp src/staff.cpp

all: library_app unit_tests

library_app: src/main.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) src/main.cpp $(SRCS) -o library_app

unit_tests: tests/unit_tests.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) tests/unit_tests.cpp $(SRCS) -o unit_tests

clean:
	rm -f library_app unit_tests
