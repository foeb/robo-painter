![Seed: 1149, Depth: 5](exp-1149_5.png "Seed: 1149, Depth: 5")

This is an (incomplete) system for generating
terrain heightmaps using techniques from genetic programming. That is,
it uses fitness functions as a guide to select abstract syntax trees
of mathematical expressions to combine into new, hopefully better,
expressions until time runs out or we find something acceptable.

### BUILDING

On Ubuntu 18.04 LTS:

1. `sudo apt-get install git make gcc lua5.1 liblua5.1-dev`
2. `git submodule init && git submodule update`
3. build `lib/lua-gd` (see https://github.com/ittner/lua-gd/issues/9 for help)

To run, type `make run` and check `images/`.

### LICENSE
This project is released into the public domain with the help of CC0.
See `COPYING` for more information.
