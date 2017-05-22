loop(X, X, X).
loop(Tortoise, Hare, Meet) :-
mysorescript_call(f, [Tortoise], Tortoise_),
mysorescript_call(f, [Hare], Hare_),
mysorescript_call(f, [Hare_], Hare__),
loop(Tortoise_, Hare__, Meet).
loop2(X, X, X, 0).
loop2(Tortoise, Hare, Meet, MuPlus1) :-
mysorescript_call(f, [Tortoise], Tortoise_),
mysorescript_call(f, [Hare], Hare_),
loop2(Tortoise_, Hare_, Meet, Mu),
is(MuPlus1, +(Mu, 1)).
loop3(X, X, 1).
loop3(Tortoise, Hare, LambdaPlus1) :-
mysorescript_call(f, [Hare], Hare_),
loop3(Tortoise, Hare_, Lambda),
is(LambdaPlus1, +(Lambda, 1)).
floyd(X0, Length, Start) :- mysorescript_call(f, [X0], Tortoise),
mysorescript_call(f, [Tortoise], Hare),
loop(Tortoise, Hare, Meet),
loop2(X0, Meet, Meet2, Start),
mysorescript_call(f, [Meet2], Hare_),
loop3(Meet2, Hare_, Length).

blah :- floyd(3, Length, Start).
