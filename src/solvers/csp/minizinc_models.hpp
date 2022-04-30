#ifndef TILER_SOLVERS_CSP_MINIZINC_MODELS_HPP_
#define TILER_SOLVERS_CSP_MINIZINC_MODELS_HPP_

#include <string>

namespace minizinc_models {

// clang-format off
const std::string kMinizincModel = R"R(

include "geost.mzn";

int: w;
int: h;
int: nEmpty;
int: nVariants;
int: nTiles;

set of int: DIMS = 1..2;
set of int: XS = 0..w-1;
set of int: YS = 0..h-1;
set of int: CS = 0..max(w-1, h-1);
set of int: SQUARES = 1..w*h;
set of int: EMPTY = 1..nEmpty;
set of int: VARIANTS = 1..nVariants;
set of int: TILES = 1..nTiles;
set of int: E_VARIANTS = 1..nVariants+1;
set of int: E_TILES = 1..nTiles+nEmpty;

array[EMPTY, DIMS] of CS: empty;
array[VARIANTS] of set of SQUARES: variants;
array[TILES] of set of VARIANTS: tiles;

array[TILES, DIMS] of var CS: positions;
array[TILES] of var VARIANTS: kinds;

array[SQUARES, DIMS] of int: squareSizes = array2d(SQUARES, DIMS, [1 | _ in 1..2*w*h]);
array[SQUARES, DIMS] of int: squarePositions =
    array2d(SQUARES, DIMS, [if isX then x else y endif | y in YS, x in XS, isX in [true, false]]);

array[E_VARIANTS] of set of SQUARES: eVariants = variants ++ [{1}];
array[E_TILES, DIMS] of var CS: ePositions = array2d(E_TILES, DIMS, array1d(positions) ++ array1d(empty));
array[E_TILES] of var E_VARIANTS: eKinds = kinds ++ [nVariants+1 | _ in EMPTY];

constraint geost_bb(2, squareSizes, squarePositions, eVariants, ePositions, eKinds, [0, 0], [w, h]);

constraint forall (p in TILES) (kinds[p] in tiles[p]);

)R";
// clang-format on

}  // namespace minizinc_models

#endif  // TILER_SOLVERS_CSP_MINIZINC_MODELS_HPP_
