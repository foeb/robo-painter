![Seed: 1149, Depth: 5](exp-1149_5.png "Seed: 1149, Depth: 5")

`robo-painter` is a simple program to procedurally generate random, grayscale images with two parts:

1. a picture-generation language (written in C) 
2. a library for genetic programming in Lua.

Currently, only part 1 is complete.

The bulk of the code is contained in `liblang.c` which holds the procedures and data structures related to the language. Perhaps the most intersting feature of the language is that any string of `LANG_WORD`s (as defined in `liblang.h`) is a valid expression in the language, which allows them to be easily manipulated, generated, or otherwise mutated.

### BUILDING

The following has been tested on Ubuntu 18.04 LTS:

1. `sudo apt-get install git make gcc lua5.1 liblua5.1-dev`
2. `git submodule init && git submodule update`
3. `sudo apt-get install libgd-dev` to get the dependencies, followed by `sudo apt-get remove libgd-dev` (unfortunately, installing `libgd-dev` doesn't appear sufficient to get `lua-gd` to work on its own)
4. Download the latest release of `libgd` from https://libgd.github.io/pages/downloads.html and install it with the usual `./configure && make && sudo make install` dance
5. Finally, `cd lib/lua-gd && make`

To run, type `make run` for a fixed demo or `lua5.1 robo.lua` for a random seed, and check `images/`.

### LICENSE
This project is released into the public domain with the help of CC0.
See `COPYING` for more information.
