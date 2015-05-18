wasp
====

WASP is an ASP solver handling disjunctive logic programs under the stable model semantics. WASP implements techniques originally introduced for SAT solving combined with optimization methods that have been specifically designed for ASP computation, such as **source pointers** enhancing unfounded sets computation and efficient techniques for **stable model checking**.

WASP takes as input logic programs in the GRINGO format and SAT/MaxSAT formulas in DIMACS format.

Compilation
====
In order to compile WASP boost (http://www.boost.org/) and g++-4.6 (or more recent) are required.

If all libraries have been installed just type:
```
make
```
and pray!

Other compiling options are also available:
* `make BUILD=stats` enables the usage of statistics
* `make BUILD=trace` enables the possibility to trace the internal behavior of WASP
* `make BUILD=debug` compiles WASP for debugging

Usage
===
In order to use WASP you need the grounder GRINGO (http://potassco.sourceforge.net/).

If gringo has been downloaded just type:
```
./gringo filename | ./wasp
```
