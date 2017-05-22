insertionSort(List, Sorted) :- insertionSort(List, [], Sorted).
insertionSort([], Acc, Acc).
insertionSort([H | T], Acc, Sorted) :- insert(H, Acc, NAcc), insertionSort(T, NAcc, Sorted).

insert(X, [Y | T], [Y | NT]) :- X > Y, insert(X, T, NT).
insert(X, [Y | T], [X, Y | T]) :- X =< Y.
insert(X, [], [X]).

drange(To, To, []).
drange(To, From, [To | Range]) :- is(NewTo, To - 1), drange(NewTo, From, Range).
