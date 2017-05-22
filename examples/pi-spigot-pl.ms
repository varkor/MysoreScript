var prolog = new Prolog;
prolog.load("pi-spigot.pl");
"Initialised SWI-Prolog.\n".print();

func mod(x, y) {
		return x - y * (x / y); };
	var n = 20;
	var a = (prolog.unify("zeroes", [((n / 4) * 14 + 1), null])).at(1);
	var b = 0;
	var c = (n / 4) * 14;
	var d = 0;
	var e = 0;
	var f = 10000;
	var g = 0;
	var h = 0;
	while (c) {
		d = mod(d, f);
		e = d;
		b = c - 1;
		g = b * 2;
		while (g) {
			g = g - 1;
			var i;
			if (h) {
				i = a.at(b);
			} else {
				 i = f / 5; }
			d = (d * b) + (f * i);
			a.atPut(b, mod(d, g));
			d = d / g;
			b = b - 1;
			g = b * 2; }
		h = 1;
		c = c - 14; }

prolog.close();
