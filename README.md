Wasp
====

Wasp is an ASP solver handling disjunctive logic programs under the stable model semantics. Wasp implements techniques originally introduced for SAT solving combined with optimization methods that have been specifically designed for ASP computation, such as **source pointers** enhancing unfounded sets computation and efficient techniques for **stable model checking**.

Wasp takes as input logic programs in the GRINGO format and SAT/MaxSAT formulas in DIMACS format.

Compilation
====
In order to compile wasp boost (http://www.boost.org/) and g++-4.6 (or more recent) are required.

If all libraries have been installed just type:
```
make
```
and pray!

Other compiling options are also available:
* `make BUILD=stats` enables the usage of statistics
* `make BUILD=trace` enables the possibility to trace the internal behavior of wasp
* `make BUILD=debug` compiles wasp for debugging

Usage
===
In order to use wasp you need a grounder. You can use either GRINGO (http://potassco.sourceforge.net/) or I-DLV (https://github.com/DeMaCS-UNICAL/I-DLV/wiki).

If the grounder has been downloaded just type:
```
./gringo filename | ./wasp

or

./i-dlv filename | ./wasp
```

Team
===
* Mario Alviano, University of Calabria
* Carmine Dodaro, University of Calabria (main developer)
* Francesco Ricca, University of Calabria

For any doubt or suggestion feel free to contact us at wasp-asp@googlegroups.com.
