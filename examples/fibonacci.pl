fib(0, 0).
fib(1, 1).
fib(N, F) :- is(A, N + -1), is(B, N + -2), mysorescript_call(fib, [A], FA), fib(B, FB), is(F, FA + FB).
