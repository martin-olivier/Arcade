# Arcade
Arcade is a gaming platform, a program that lets the user choose a game to play and keeps a register of player scores.

## Subject

[![sub](https://img.shields.io/badge/Subject-pdf-red.svg)](./subject.pdf)

## Requirements

`Build`
- C++20 compiler
- cmake 3.17

`Libraries`
- SFML
- SDL
- SDL_image
- SDL_mixer
- SDL_ttf
- NCURSES

## Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Librairies
### Graphic
We include `3` graphic libs:   
- SFML
- SDL
- NCURSES   

### Game
We include `2` game libs:
- Nibbler
- Pacman  

## Usage
```bash
USAGE:
  ./arcade path_to_graphical_lib
  
DESCRIPTION:
  lib         path to the initial graphical library to load (./lib/arcade_lib_name.so)
  
COMMANDS:
  N           next graphic lib
  P           previous graphic lib
  O           next game lib
  I           previous game lib
  
  M           menu
  R           reset the game
  ESC         quit arcade
  
  RIGHT_ARROW move right
  LEFT_ARROW  move left
  UP_ARROW    move up
  DOWN_ARROW  move down
```

## Documentation

[![doc](https://img.shields.io/badge/Documentation-pdf-red.svg)](./doc/documentation.pdf)

## Result

Grade `A`

## Contributors

- [Benjamin Reigner](https://github.com/Breigner01)
- [Martin Olivier](https://github.com/martin-olivier)
- [Coline Seguret](https://github.com/Cleopha)
