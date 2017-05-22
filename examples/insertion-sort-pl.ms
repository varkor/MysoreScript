var prolog = new Prolog;
prolog.load("insertion-sort.pl");
"Initialised SWI-Prolog.\n".print();

func insertionSort(array) {
	var i = 1;
	var j;
	var x;
	var break;
	var l = array.length();
	while (i < l) {
		x = array.at(i);
		j = i - 1;
		break = 0;
		while (break == 0) {
			break = 1;
			if (j >= 0) {
				if (array.at(j) > x) {
					array.atPut(j + 1, array.at(j));
					j = j - 1;
					break = 0;
				}
			}
		}
		array.atPut(j + 1, x);
		i = i + 1;
	}
};

func drange(from, to) {
	var array = [];
	var i = from;
	var j = 0;
	while (i > to) {
		array.atPut(j, i);
		i = i - 1;
		j = j + 1;
	}
	return array;
};

var range_mss = drange(800, 0);
((prolog.unify("insertionSort", [range_mss, null])).at(1));
var range_el = (prolog.unify("drange", [800, 0, null])).at(2);
insertionSort(range_el);

prolog.close();
