#include <iostream>
#include <vector>
#include "SWI-cpp.h"

PREDICATE(mysorescript_true, 0) {
	std::cout << "mysorecript_true" << std::endl;
	return TRUE;
}

PREDICATE(mysorescript_call, 3) {
	std::cout << "mysorecript_call:" << (char*) A1 << " args:" << (char*) A2 << " unify:" << (char*) A3 << std::endl;
	return TRUE;
}

class SWIProlog {
	PlEngine engine;
	
	public:
	SWIProlog(std::string path) : engine((char*) path.c_str()) { }
	
	std::vector<PlTerm>& unify(std::string name, std::vector<PlTerm>& arguments) {
		PlTermv parameters(arguments.size());
		int i = 0;
		for (auto& argument : arguments) {
			parameters[i ++] = argument;
		}
		PlQuery query(name.c_str(), parameters);
		try {
			if (query.next_solution()) {
				return arguments;
			}
		} catch (const PlException& exception) {
			// There was no matching Prolog rule.
		}
		throw 899;
	}
};

int main(int argc, char* argv[]) {
	if (argc < 1) {
		std::cout << "This program needs to be called with at least one argument." << std::endl;
		return EXIT_FAILURE;
	}
	try {
		SWIProlog prolog(argv[0]);
		std::vector<PlTerm> arguments;
//		arguments.push_back(PlTail(PlTerm()));
//		arguments.push_back(PlTerm(7L));
//		arguments.push_back(PlTerm());
		try {
//			prolog.unify("double", arguments);
			arguments.push_back(PlTerm(24L));
			arguments.push_back(PlTerm());
			prolog.unify("fib", arguments);
			for (auto& argument : arguments) {
				std::cout << (char *) argument << std::endl;
			}
			return EXIT_SUCCESS;
		} catch (...) {
			std::cout << "Failed to unify." << std::endl;
		}
	} catch (const PlError& error) {
		std::cerr << error.message << std::endl;
	}
}
