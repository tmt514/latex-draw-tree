# latex-draw-tree
transform into latex codes for tree

## Usage

```bash
make run FILE=binary.in
```

You can change `binary.in` into any other input file under the `inputs/` folder.

## Input format

```
ID_1 k_children child_ID_1 child_ID_2 ... child_ID_k [style]
ID_2 x_children child_ID_1 child_ID_2 ... child_ID_x [style]
...
END_OF_VERTICES
```