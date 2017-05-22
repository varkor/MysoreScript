var prolog = new Prolog;
prolog.load("merge-sort.pl");
"Initialised SWI-Prolog.\n".print();

func merge(values, helper, low, middle, high) {
	var i = low;
	while (i <= high) {
		helper.atPut(i, values.at(i));
		i = i + 1;
	}

	var i = low;
	var j = middle + 1;
	var k = low;
	while ((i <= middle) && (j <= high)) {
		if (helper.at(i) <= helper.at(j)) {
			values.atPut(k, helper.at(i));
			i = i + 1;
		} else {
			values.atPut(k, helper.at(j));
			j = j + 1;
		}
		k = k + 1;
	}
	
	while (i <= middle) {
		values.atPut(k, helper.at(i));
		k = k + 1;
		i = i + 1;
	}
};

func merge_range(values, helper, low, high) {
	if (low < high) {
		var middle = low + (high - low) / 2;
		merge_range(values, helper, low, middle);
		merge_range(values, helper, middle + 1, high);
		merge(values, helper, low, middle, high);
	}
};

func mergesort(values) {
	var helper = [];
	merge_range(values, helper, 0, values.length() - 1);
	return values;
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

var range = (prolog.unify("drange", [4000, 0, null])).at(2);
mergesort(range);

prolog.close();
