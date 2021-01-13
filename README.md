# Tiler

Tiler is a CLI tool for automated solving of [polyomino](https://en.wikipedia.org/wiki/Polyomino)
tiling problems. It features multiple solver backends.

## Usage

The CLI takes input (either from command-line or from a file) which defines a board (region
consisting of unit squares) and polyomino tiles and tries to find a perfect tiling of the board with
the tiles.

The shape of board as well as of each tile can be provided in any of these formats:

- name, eg. `3L` (see the `list` command for the list of all named shapes)
- dimensions, eg. `2x4` (applicable only for rectangles)
- [perimeter - TODO]
- map, eg. this equivalent of `3x2`:
  ```
  xxx
  xxx
  ```
  (useful with the `-f `option)

You should provide a list of shapes, where the first one represents the board and the others the
tiles. Tile shape may be prefixed with `N:` where `N` is the available number of such tiles
(unlimited if not provided). See the `example` command for input examples.

Run `tiler -h` and `tiler solve -h` to see all options.

### Limitations

Only continuous shapes without holes or touching corners are allowed.

Solver may rotate the tiles. There is a CLI flag to allow reflections (flipping the tiles over).

## Build Instructions

Building Tiler requires these dependencies:

- [CMake](https://cmake.org/) build tool (version 3.12 or later)
- [Conan](https://conan.io/) package manager
- a C/C++ compiler (a recent version of GCC is recommended)

When the dependencies are met, clone this repository and run `./scripts/release.sh`.

Your freshly compiled Tiler executable should be located at `build/bin/tiler`. You can configure
build options using eg. `ccmake` before running `cmake --build` (see
[release.sh](./scripts/release.sh)).

This setup is tested on Linux with GCC 10.2.0, but it should work on other platforms, too (maybe
with some minor tweaks).

## Development Setup

In addition to CMake and Conan (see [Build Instructions](#build-instructions)), you should install:

- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) code formatter (version 10 or later)
- [Cppcheck](http://cppcheck.sourceforge.net/) static code analysis tool
- [GDB](https://www.gnu.org/software/gdb/) debugger (optional)

Regarding the actual development, using [VS Code](https://code.visualstudio.com/) with the [C/C++
extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) is recommended -
this way, you can use `Setup`, `Lint`, `Build`, `Run` and `Test` tasks, the built-in debugging UI
and utilize proposed [VS Code settings](./.vscode/example.settings.json). If you intend to use other
editor/IDE, all commands to run can be found at [tasks.json](./.vscode/tasks.json).

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
