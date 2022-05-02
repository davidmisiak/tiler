include "geost.mzn";

int: w;
int: h;
int: nEmpty;
int: nRects;
int: nVariants;
int: nTiles;

set of int: DIMS = 1..2;
set of int: XS = 0..w-1;
set of int: YS = 0..h-1;
set of int: CS = 0..max(w-1, h-1);
set of int: RECTS = 1..nRects;
set of int: EMPTY = 1..nEmpty;
set of int: VARIANTS = 1..nVariants;
set of int: TILES = 1..nTiles;
set of int: E_VARIANTS = 1..nVariants+1;
set of int: E_TILES = 1..nTiles+nEmpty;

array[EMPTY, DIMS] of CS: empty;
array[VARIANTS] of set of RECTS: variants;
array[TILES] of set of VARIANTS: tiles;

array[TILES, DIMS] of var CS: positions;
array[TILES] of var VARIANTS: kinds;

array[RECTS, DIMS] of int: rectSizes;
array[RECTS, DIMS] of int: rectPositions;

array[E_VARIANTS] of set of RECTS: eVariants = variants ++ [{1}];
array[E_TILES, DIMS] of var CS: ePositions = array2d(E_TILES, DIMS, array1d(positions) ++ array1d(empty));
array[E_TILES] of var E_VARIANTS: eKinds = kinds ++ [nVariants+1 | _ in EMPTY];

constraint geost_bb(2, rectSizes, rectPositions, eVariants, ePositions, eKinds, [0, 0], [w, h]);

constraint forall (p in TILES) (kinds[p] in tiles[p]);
