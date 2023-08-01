#!/bin/bash

cppcheck src test --suppress=missingInclude --suppress=missingIncludeSystem --enable=all --inline-suppr --quiet -DCADICAL -DCRYPTOMINISAT -DKISSAT --std='c++17' && \
find src test -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run -Werror
