# Tiler

Tiler is a CLI tool for automated solving of [polyomino](https://en.wikipedia.org/wiki/Polyomino)
tiling problems.

Currently, it features the following solver backends:

- **simple backtracking** (with a few tile selection heuristics)
- Donald Knuth's **Algorithm X** (Dancing Links)
- conversion to the **SAT problem** (several modifications)
- conversion to the **ILP problem**
- conversion to a **MiniZinc constraint satisfaction problem**

## Usage

The CLI takes input (either from the command line or from a file) which defines a board (a region
consisting of unit squares) and polyomino tiles and tries to find a perfect tiling of the board with
the tiles.

The shape of the board as well as of each tile can be provided in any of these formats:

- **name**, eg. `3L` (see the `shapes` command for the list of all named shapes)

- **dimensions**, eg. `2x4` (applicable only for rectangles)

- **map**, eg. this equivalent of `3x2`:

  ```txt
  xxx
  xxx
  ```

  (useful with the `-i` option)

- **perimeter**, eg. `DDRRULUL` - an equivalent of `3L` (crossings of the perimeter are not allowed,
  applicable only for shapes without holes)

You should provide a list of shapes, where the first one represents the board and the others the
tiles. Tile shape may be prefixed with `N:` where `N` is the available number of such tiles
(unlimited if not provided). See the `example` command for input examples.

Run `tiler -h` and `tiler solve -h` to see all options.

### Limitations

Only continuous shapes are allowed.

The solver may rotate the tiles. There is a CLI flag to allow reflections (flipping the tiles over).

## Examples

Here are some example outputs of the solver.

`tiler solve -o 01.svg 11x7 4T 3:3I`\
![Example 1](examples/01.svg)

`tiler solve -o 02.svg 10x9 1:5A 1:5F 1:5G 1:5J 1:5P 1:5S 1:5Y 1:5R 1:5N 1:5L 1:5Q 1:5Z 1:5I 1:5T 1:5U 1:5V 1:5W 1:5X`\
![Example 2](examples/02.svg)

## Build Instructions

Before building and running Tiler, always run `source scripts/load-env.sh` to load the environment
variables required by some dependencies.

### Dependencies

There are several optional open-source dependencies (in the `external` directory):

- [DLX1](https://www-cs-faculty.stanford.edu/~knuth/programs.html) exact cover finding algorithm by
  Donald Knuth
- [CaDiCaL](https://github.com/arminbiere/cadical) SAT solver
- [CryptoMiniSat](https://github.com/msoos/cryptominisat) SAT solver
- [Kissat](https://github.com/arminbiere/kissat) SAT solver
- [BreakID](https://github.com/meelgroup/breakid) CNF symmetry breaking library
- [SBVA](https://github.com/hgarrereyn/SBVA) CNF preprocessor
- [PBLib](https://github.com/master-keying/pblib) pseudo-boolean constraint encoding library
  (required when either CaDiCaL, CryptoMiniSat or Kissat is present)
- [MiniZinc](https://www.minizinc.org/) constraint modeling framework
- [Gecode](https://www.gecode.org/) constraint solver (required when MiniZinc is present)
- [Chuffed](https://github.com/chuffed/chuffed) constraint solver (optional when MiniZinc is
  present)

Run `./scripts/build-all-deps.sh` to clone and build them. You can select only some of them by
running a subset of `./scripts/build-*.sh` scripts and setting CMake options `-DDLX=OFF` /
`-DCADICAL=OFF` / `-DCRYPTOMINISAT=OFF` / `-DKISSAT=OFF` / `-DBREAKID=OFF` / `-DSBVA=OFF` /
`-DMINIZINC=OFF` / `-DCHUFFED=OFF`. Having the libraries installed system-wide should work as well
(at least for some of them).

Additionally, Tiler can be dynamically linked against the
[Gurobi](https://www.gurobi.com/products/gurobi-optimizer/) (v10.0.2) ILP solver. If you have an
appropriate license, follow the official Gurobi
[installation guide](https://www.gurobi.com/documentation/10.0/quickstart_linux/index.html).
Otherwise, set the CMake option `-DGUROBI=OFF`.

### Building Tiler

Building Tiler requires these dependencies:

- [CMake](https://cmake.org/) build tool (version 3.12 or later)
- [Conan](https://conan.io/) package manager
- a C/C++ compiler (a recent version of GCC is recommended, C++17 support is required)

Build the executable by running `./scripts/release-setup.sh` and `./scripts/release-build.sh`. You
can configure build options using eg. `ccmake` before running `release-build.sh` or by adding
appropriate `-D` flags to `release-setup.sh` (they will be passed on to CMake).

Your freshly compiled Tiler executable will be located at `release/bin/tiler`.

This setup is tested on Linux with GCC 10.2.0, but it should work on other platforms, too (maybe
with some minor tweaks).

## Development Setup

In addition to CMake and Conan (see [Build Instructions](#build-instructions)), you should install:

- [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) code formatter (version 10 or later)
- [Cppcheck](http://cppcheck.sourceforge.net/) static code analysis tool
- [GDB](https://www.gnu.org/software/gdb/) debugger (optional)
- [Python 3](https://www.python.org/), [Pandas](https://pandas.pydata.org/) and
  [Matplotlib](https://matplotlib.org/) (if you want to run the benchmark displaying script)

Regarding the actual development, using [VS Code](https://code.visualstudio.com/) with the [C/C++
extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) is recommended -
this way, you can use the IDE tasks, the built-in debugging UI and utilize the proposed [VS Code
settings](./.vscode/example.settings.json). If you intend to use a different editor/IDE, all
commands to run can be found at [tasks.json](./.vscode/tasks.json).

## Benchmarks

To run the benchmarking suite on all problems in the `problems` directory, run
`./scripts/release-benchmark.sh` (however, note that this will take several hours; you can edit the
`benchmark_filter` parameter in the script to select only some problems or solvers). You can use
`./benchmark/plot.py` to plot the measured data.

## Problems

The `problems` directory contains several tiling problems for testing and benchmarking. They are
organized by their tile sets, and their filenames try to describe the respective problems (e.g. they
usually contain board dimensions as a measure of the problem size). Each filename ends with either
`_s`, if it is solvable, or `_u`, if it is unsolvable. If the problem is intended to be solved with
reflections allowed, its filename contains a quote `'`.

The following books were used as sources of some problems:

> S. W. Golomb. *Polyominoes: Puzzles, patterns, problems, and packings*. Princeton University Press,
2nd edition, 1994.

```txt
many-unique/4r-5r-04x20'_s
many-unique/4r-5r-05x16'_s
many-unique/4r-5r-08x10'_s
many-unique/5r-03x20'_s
many-unique/5r-04x15'_s
many-unique/5r-05x12'_s
many-unique/5r-06x10'_s
many-unique/5r-diamond'_u
many-unique/5r-fcr'_s
many-unique/5r-H-08x09'_s
many-unique/5r-hskew-06x10'_u
many-unique/5r-plus'_u
many-unique/5r-sideways'_u
many-unique/5r-skew-03x20'_s
many-unique/5r-skew-04x15'_s
many-unique/5r-skew-05x12'_s
many-unique/5r-skew-06x10'_s
```

> G. E. Martin. *Polyominoes: a Guide to Puzzles and Problems in Tiling*. Mathematical Association of
America, 1991.

```txt
4I/10x10_u
4L/10x10_u
mixed/1-3I-07x07_s
mixed/4Os-4T-06x06_u
```

The remaining problems are either modifications of the listed ones, trivial, generally-known, or
genuine.

---

## License

Tiler is licensed under the [GNU GPL license v3](./LICENSE). This does not apply to files in the
`external` directory.

```txt
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
