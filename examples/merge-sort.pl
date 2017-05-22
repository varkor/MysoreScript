drange(To, To, []).
drange(To, From, [To | Range]) :- NewTo is To - 1, drange(NewTo, From, Range).
