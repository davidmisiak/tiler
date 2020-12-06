# Tiler

Tiler is a CLI tool for automated solving of polyomino tiling problems. It features multiple solver
backends.

## Build Instructions

Building Tiler requires these dependencies:

- [CMake](https://cmake.org/) build tool (version 3.12 or later)
- [Conan](https://conan.io/) package manager
- a C/C++ compiler (a recent version of GCC is recommended)

When the dependencies are met, clone this repository and run:

```bash
mkdir build
cd build
conan install ..
cmake ..
cmake --build .
```

Your freshly compiled Tiler executable should be located at `build/bin/tiler`. You can configure
build options using eg. `ccmake` before running `cmake --build`.

This setup is tested on Linux with GCC 10.2.0, but it should work on other platforms, too (maybe
with some minor tweaks).

## Development Setup

In addition to CMake and Conan (see [Build Instructions](#build-instructions)), you should install:

- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) code formatter (version 10 or later)
- [Cppcheck](http://cppcheck.sourceforge.net/) static code analysis tool

Regarding the actual development, using [VS Code](https://code.visualstudio.com/) with the [C/C++
extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) is recommended -
this way, you can use Setup, Lint, Build, Run and Test tasks (and utilize provided
`example.settings.json`). If you intend to use other editor/IDE, all commands to run can be found at
[tasks.json](./.vscode/tasks.json).

---

## License

Tiler is licensed under the [GNU GPL license v3](./LICENSE).

```
Tiler - tool for automated solving of polyomino tiling problems
Copyright (C) 2020  David Misiak

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```
