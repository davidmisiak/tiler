#!/bin/bash

cppcheck --suppress=missingInclude --enable=all --inline-suppr --quiet src test && \
find src test -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run -Werror
