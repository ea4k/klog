#!/bin/bash
lcov -c -d ../src --output-file ./coverage.info
genhtml coverage.info --output-directory out
