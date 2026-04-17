# TI-84 Plus CE Physics Solver

Small CEdev C project for the TI-84 Plus CE / TI-84 Plus CE Python.

## Structure

- `src/main.c` - app entry point
- `src/menu.c` - category and formula menus
- `src/formula.c` - formula registry and solver logic
- `src/io_helpers.c` - screen and numeric input helpers
- `src/math_helpers.c` - validation helpers
- `include/*.h` - shared headers

## Build

From a CEdev environment:

```sh
make
```

## Notes

- `Mode` acts as `Back` on every screen.
- `Clear` exits from menus.
- Numeric input validates empty values, division by zero cases, and invalid square roots through solver checks.
- The formula registry is intentionally simple so more formulas can be added later by registering more `solve_option_t` entries.
