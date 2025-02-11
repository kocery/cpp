#!/bin/bash

#cd build && cmake .. && make && ./test_fibonacci_heap
cd build && rm -rf * && \
cmake .. -DENABLE_UBASAN=true && make && ./test_fibonacci_heap && rm -rf * && \
#cmake .. -DENABLE_MSAN=true && make && ./test_fibonacci_heap && rm -rf * &&
cmake .. -DCHECK_COVERAGE=true && make && ./test_fibonacci_heap && cd .. && \
gcovr --exclude-unreachable-branches --gcov-executable="llvm-cov gcov" --html-details -o ./coverage_reports/report.html && \
clang-tidy -p build fibonacci_heap.cpp -checks=google-*,cppcoreguidelines-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory,modernize-*,performance-*,clang-analyzer-*,bugprone-*,misc-*,readability-*,-readability-identifier-length,-modernize-use-trailing-return-type,-misc-non-private-member-variables-in-classes