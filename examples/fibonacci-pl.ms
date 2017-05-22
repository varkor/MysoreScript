var prolog = new Prolog;
prolog.load("fibonacci.pl");
"Initialised SWI-Prolog.\n".print();

func fib(n) {
	if (n < 2) {
		return n;
	}
	var result = prolog.unify("fib", [n - 1, null]);
	var fa = result.at(1);
	var fb = fib(n - 2);
	return fa + fb;
};

var f = prolog.unify("fib", [24, null]);
if (f) {
	f.print();
} else {
	"Failed to unify fib.\n".print();
}
prolog.close();
