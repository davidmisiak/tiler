#!/bin/bash

cppcheck src test --suppress=missingInclude --enable=all --inline-suppr --quiet -DCADICAL -DCRYPTOMINISAT && \
find src test -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run -Werror
