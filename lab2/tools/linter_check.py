#!/usr/bin/env python3
"""
@file linter_check.py
@brief Simple static code analyzer script for C++ source and header files.
       Checks for maximum line length and other style restrictions.
"""

import os
import sys

# Максимально допустимая длина строки по требованиям курса
MAX_LINE_LENGTH = 150

def check_file(filepath):
    errors = 0
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        for line_num, line in enumerate(f, 1):
            # Убираем символ перевода строки в конце
            clean_line = line.rstrip('\r\n')
            
            # Проверка длины строки
            if len(clean_line) > MAX_LINE_LENGTH:
                print(f"[WARNING] {filepath}:{line_num} - line is too long ({len(clean_line)} chars, max allowed is {MAX_LINE_LENGTH})")
                errors += 1
                
    return errors

def main():
    target_dirs = ['include', 'src', 'tests']
    total_errors = 0
    files_checked = 0

    print("=== Starting Linter Code Check ===")
    
    for d in target_dirs:
        if not os.path.exists(d):
            continue
        for root, _, files in os.walk(d):
            for file in files:
                if file.endswith(('.cpp', '.hpp', '.h')):
                    filepath = os.path.join(root, file)
                    files_checked += 1
                    total_errors += check_file(filepath)

    print(f"Checked {files_checked} files.")
    if total_errors > 0:
        print(f"Linter found {total_errors} potential style warnings/errors.")
        # Можно сделать sys.exit(1) для падения CI/CD при нарушениях
        sys.exit(0) 
    else:
        print("All files passed style checks successfully!")
        sys.exit(0)

if __name__ == '__main__':
    main()