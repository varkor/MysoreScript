zeroes(0, []).
zeroes(N, [0|T]) :- N_ is N - 1, zeroes(N_, T).
mod(X, Y, Z) :- is(Z, mod(X, Y)).
