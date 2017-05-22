func f(x) {
var z = x * x + 1;
return z - 8589934591 * (z / 8589934591);
};

var prolog = new Prolog;
prolog.load("floyd.pl");
"Initialised SWI-Prolog.\n".print();

prolog.unify("blah", []);

prolog.close();
