#!/bin/bash

# Configure
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/Qt/6.10.2/macos

# Compile and run tests
cmake --build build && ctest --test-dir build --output-on-failure