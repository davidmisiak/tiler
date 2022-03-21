# DLX1

The `dlx1.w` file contains Donald Knuth's original implementation of the Dancing Links algorithm.
This file is an exact copy of the `DLX1` file from
[www-cs-faculty.stanford.edu/~knuth/programs.html](https://www-cs-faculty.stanford.edu/~knuth/programs.html),
where Donald Knuth's code is hosted.

The file is in CWEB format. You can convert it to C and PDF files by running:

```sh
ctangle dlx1.w - dlx1.c
cweave dlx1.w - dlx1.tex
pdftex dlx1.tex dlx1.scn dlx1.idx
```

Note that the resulting files are also provided here as `dlx1.c` and `dlx1.pdf`.

Finally, in order to enable benchmarking the DLX1 algorithm in Tiler, we had to modify the input
parsing and output printing code -- so that the main function accepts and returns `std::vector`s
instead of working with `stdin` and `stdout`. The modified code (autoformatted and with some
unnecessary features removed) can be found in `dlx1-lib.cpp`. Use `scripts/build-dlx.sh` to build
it.
