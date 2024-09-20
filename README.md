### About
Chess engine with a Monte Carlo tree search written in C++ and an interface written in C# with WinForms.

### Usage
In "Engine.cpp", replace MAX_ITERATIONS with however many iterations you want to engine to perform in its MTCS. The greater this amount the longer the engine takes to search for moves.

### Known bugs

- Stalemate not always identified
- Putting enemy king in check does not always work

### Necessary features

- Piece promotion
- En passant
- 50 move limit

### Future improvements

- Engine can train itself
- Game analysis
