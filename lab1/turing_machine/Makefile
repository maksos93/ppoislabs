# Makefile для Лабораторной работы №2 (Симулятор Машины Тьюринга)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

all: turing_app unit_tests

turing_app: src/main.cpp src/turing_machine.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/turing_machine.cpp -o turing_app

unit_tests: src/unit_tests.cpp src/turing_machine.cpp
	$(CXX) $(CXXFLAGS) src/unit_tests.cpp src/turing_machine.cpp -o unit_tests

clean:
	rm -f turing_app unit_tests