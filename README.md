# TI-84 Physics Solver

A CEdev-based physics formula solver for the TI-84 Plus CE / TI-84 Plus CE Python, written in C.

This project is designed as a practical calculator tool for physics study and test preparation. It combines a formula solver, a full formula reference, a symbol glossary, and basic derivative/integral support for common physics expressions.

## Features

- Built in C with the CEdev toolchain
- Targets TI-84 Plus CE and TI-84 Plus CE Python
- Clean calculator-friendly menu interface
- Formula Solver section for solving supported physics formulas
- All Formulas section for browsing lesson formulas
- Symbols section with symbol meanings and units
- Input validation for invalid values
- Back navigation on every screen
- Support for common kinematics, circular motion, dynamics, work, energy, and power formulas
- Basic derivative/integral tools for physics-style expressions and polynomial expressions

## Main Sections

### 1. Formula Solver
Lets the user:
- choose a formula category
- choose a formula
- choose which variable to find
- enter known values
- receive the computed result

### 2. All Formulas
Contains a browsable reference of important formulas from:
- Kinematics
- Circular Motion
- Dynamics
- Work / Energy / Power
- Physics Calculus

### 3. Symbols
Contains a glossary of symbols used in the formulas, including units such as:
- `F = force [N]`
- `m = mass [kg]`
- `v = velocity [m/s]`
- `a = acceleration [m/s^2]`
- `W = work [J]`
- `P = power [W]`

## Supported Formula Areas

### Kinematics
Includes formulas such as:
- `v = v0 + a*t`
- `s = s0 + v*t`
- `s = v0*t + 0.5*a*t^2`
- `v^2 = v0^2 + 2*a*s`

### Circular Motion
Includes formulas such as:
- `v = r*omega`
- `a_t = r*alpha`
- `a_c = v^2/r`
- `omega = 2*pi*f`
- `omega = 2*pi/T`
- `phi = omega*t`
- `omega = omega0 + alpha*t`
- `phi = omega0*t + 0.5*alpha*t^2`
- `phi = 2*pi*N`

### Dynamics
Includes formulas such as:
- `p = m*v`
- `F = m*a`
- `a = F/m`
- `I = F*t`

### Work / Energy / Power
Includes formulas such as:
- `Ek = 0.5*m*v^2`
- `W = F*d`
- `Pavg = W/t`
- `P = F*v`
- `delta Ep = m*g*h`
- `E = Ep + Ek`
- `W = delta Ek`
- `F = k*d`

### Physics Calculus
Includes practical derivative/integral-based physics relations such as:
- `v = dx/dt`
- `a = dv/dt`
- `F = dp/dt`
- `omega = dphi/dt`
- `alpha = domega/dt`
- `P = dW/dt`
- `dx = v*dt`
- `dv = a*dt`
- `dphi = omega*dt`
- `domega = alpha*dt`
- `I = integral(F dt)`
- `W = integral(F dr)`

It also includes a polynomial expression tool for inputs like:
- `10t`
- `3t^2 + 2t - 1`
- `x^3 - x`

with support for:
- derivative formula
- derivative at a value
- integral formula
- definite integral

## Project Structure

```text
ti84_physics_solver/
├── Makefile
├── include/
│   ├── expr_calc.h
│   ├── formula.h
│   ├── io_helpers.h
│   ├── math_helpers.h
│   ├── menu.h
│   ├── reference.h
│   └── symbols.h
└── src/
    ├── expr_calc.c
    ├── formula.c
    ├── io_helpers.c
    ├── main.c
    ├── math_helpers.c
    ├── menu.c
    ├── reference.c
    └── symbols.c
```

## Build Instructions
Make sure CEdev is installed and available in your terminal.

Example:

export PATH="$HOME/Desktop/CEdev/bin:$PATH"
Then build:

cd /path/to/ti84_physics_solver
make clean
make
The output file will be generated in:

bin/PHYSOLVE.8xp
Running on Calculator
Build the project with CEdev
Transfer PHYSOLVE.8xp to the calculator using TI Connect CE
On newer TI-84 Plus CE Python OS versions, launch the app through arTIfiCE
Open the app and navigate through the menus
Notes
This project is focused on practical physics formulas, not full symbolic algebra
The derivative/integral expression tool is intended for polynomial-style expressions
The app is designed to be easy to expand with new formulas and categories later
Future Improvements
Possible future additions:

more vector-based solvers
more symbolic derivative/integral support
additional physics categories
improved UI polish for long formula lists
more advanced unit-aware input/output
