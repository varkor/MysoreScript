#pragma once

#include "Pegmatite/pegmatite.hh"

namespace MysoreScript {
	namespace Parser
	{
		using pegmatite::Rule;
		using pegmatite::operator""_E;
		using pegmatite::operator""_S;
		using pegmatite::ExprPtr;
		using pegmatite::BindAST;
		using pegmatite::any;
		using pegmatite::nl;
		using pegmatite::trace;

		/**
		 * Grammar for the MysoreScript language.
		 */
		struct MysoreScriptGrammar
		{
			/**
			 * Whitespace: spaces, tabs, newlines
			 */
			Rule whitespace  = ' '_E | '\t' | nl('\n');
			/**
			 * Comments, including tracking newlines inside comments via the
			 * whitespace rule.
			 */
			Rule comment     = ("/*"_E >>
							   (*(!ExprPtr("*/") >> (nl('\n') | any()))) >>
							   "*/") |
							   "//"_E >> *(!(ExprPtr("\n")) >> any()) >> nl('\n');
			/**
			 * Rule for treating both comments and whitespace as ignored tokens.
			 */
			Rule ignored     = *(comment | whitespace);
			/**
			 * Digits are things in the range 0-9.
			 */
			ExprPtr digit = '0'_E - '9';
			/**
			 * Numbers are one or more digits, optionally prefixed with its sign.
			 */
			Rule num    = -("+-"_S) >> +digit;
			/**
			 * Values are either numbers or expressions in brackets (highest precedence).
			 */
			Rule val    = num | '(' >> arith_expr >> ')';
			/**
			 * Value-esques are expressions that have higher precedence than any operation.
			 */
			Rule val_esque = val | call | newExpr | variable |
			string | array | null;
			/**
			 * Multiply operations are values or multiply, or divide operations,
			 * followed by a multiply symbol, followed by a value.  The sides can never
			 * be add or subtract operations, because they have lower precedence and so
			 * can only be parents of multiply or divide operations (or children via
			 * parenthetical expressions), not direct children.
			 */
			Rule mul_op = mul >> '*' >> val_esque;
			/**
			 * Divide operations follow the same syntax as multiply.
			 */
			Rule div_op = mul >> '/' >> val_esque;
			/**
			 * Multiply-precedence operations are either multiply or divide operations,
			 * or simple values (numbers of parenthetical expressions).
			 */
			Rule mul    = mul_op | div_op | val_esque;
			/**
			 * Add operations can have any expression on the left (including other add
			 * expressions), but only higher-precedence operations on the right.
			 */
			Rule add_op = arith_expr >> '+' >> arith_expr;
			/**
			 * Subtract operations follow the same structure as add.
			 */
			Rule sub_op = arith_expr >> '-' >> arith_expr;
			/**
			 * Not-equal comparison.
			 */
			Rule ne_cmp = arith_expr >> "!=" >> arith_expr;
			/**
			 * Equal comparison.
			 */
			Rule eq_cmp = arith_expr >> "==" >> arith_expr;
			/**
			 * Less-than comparison.
			 */
			Rule lt_cmp = arith_expr >> '<' >> arith_expr;
			/**
			 * Greater-than comparison.
			 */
			Rule gt_cmp = arith_expr >> '>' >> arith_expr;
			/**
			 * Less-than-or-equal comparison.
			 */
			Rule le_cmp = arith_expr >> "<=" >> arith_expr;
			/**
			 * Greater-than-or-equal comparison.
			 */
			Rule ge_cmp = arith_expr >> ">=" >> arith_expr;
			/**
			 * General rule for comparisons.  Matches one of the comparison types above.
			 */
			Rule cmp    = eq_cmp | ne_cmp | lt_cmp | gt_cmp | le_cmp | ge_cmp;
			/**
			 * Arithmetic expressions can be any of the numeric operations.
			 */
			Rule arith = add_op | sub_op | cmp | mul;
			/**
			 * Expressions can be any of the other types.
			 */
			Rule arith_expr = arith | expression;
			/**
			 * A character in a string.  Matches anything except the closing quote.
			 */
			ExprPtr character = term(("\\\""_E | !ExprPtr('"') >> (nl('\n') | any())));
			/**
			 * The body of a string.  An AST builder can use this to get just the
			 * characters in the string, without the enclosing quotes.
			 */
			Rule string_body      = *character;
			/**
			 * Strings are any collection of characters, enclosed in quotes.
			 */
			Rule string  = term('"' >> string_body >> '"');
			/**
			 * Letters - valid characters for the start of an identifier.
			 */
			ExprPtr letter    = (('a'_E - 'z') | ('A'_E - 'Z'));
			/**
			 * A list of reserved keywords to prevent any of them being used as
			 * identifiers and make attempting such a syntax error.
			 */
			Rule reserved = "func"_E | "null" | "new" | "var" | "return" | "else" | "if" | "while" | "class"; 
			/**
			 * Identifiers are a letter followed by zero or more alphanumeric
			 * characters.
			 */
			Rule identifier   = !reserved >> term(letter >> *(letter | digit));
			/**
			 * Argument list.  Zero or more comma-separated arguments, wrapped in
			 * brackets.
			 */
			Rule argList      = '('_E >> -(identifier >> *(',' >> identifier)) >> ')';
			/**
			 * A closure starts with the keyword 'func', followed by a function name, a
			 * list of arguments in brackets and a list of statements in braces.
			 */
			Rule closure      = "func"_E  >> identifier >> argList >>
								'{' >> statements >> '}';
			/**
			 * Variable references are single identifiers.
			 */
			Rule variable     = identifier;
			/**
			 * Assignments are variables, followed by a single equals sign, and then the
			 * expression to assign to the variable.
			 */
			Rule assignment   = variable >> '=' >> expression;
			/**
			 * The argument list for a call.  This is a separate rule to allow automatic
			 * AST construction to distinguish between the expressions that are part of
			 * the argument list from others in the call.
			 */
			Rule callArgList  = '('_E >> -(expression >> *(',' >> expression)) >> ')';
			/**
			 * A call is something that is callable, followed eventually by an argument
			 * list.  If the callable expression is followed by a dot and a method name,
			 * then this is a method invocation, otherwise the expression on the left is
			 * assumed to be a closure and is invoked directly.
			 */
			Rule call         = (instance >> '.'_E >> identifier >> callArgList) | (callable >> callArgList);
			/**
			 * Callable expression.  
			 */
			Rule callable     = closure | newExpr | arith | variable | ('(' >> expression >> ')') | call;
			/**
			 * An instance is an expression that can have methods called on it. This
			 * allows for a distinction between expressions that can be called, such
			 * as closures, and expressions that can have methods called on them,
			 * such as string or array literals. This is the same as an expression, but
			 * with the call at the end so that each of the other possibilities will be
			 * tried before hitting left recursion.
			 */
			Rule instance     = closure | newExpr | arith_expr | variable | string | array
								| call;
			/**
			 * All of the valid kinds of expression.  Note that the order places calls
			 * first, as greedy matching will try cause them to then be matched in the
			 * `callable` order (which places call last) and then parse the argument
			 * list, but would just successfully parse an expression and stop if call
			 * were not first here.  Note also that expression types that start with a
			 * keyword are placed before those that (may) begin with identifiers so that
			 * we can trivially disambiguate these cases.
			 */
			Rule expression   = call | closure | newExpr | arith_expr | variable |
								string | array | null;
			/**
			 * Elements can be any expression.
			 */
			Rule element = expression;
			/**
			 * Arrays are ordered lists of comma-separated elements, enclosed in square
			 * brackets.
			 */
			Rule array = '[' >> -(element >> *(',' >> element)) >> ']';
			/**
			 * The null value is the value of a variable before it has been defined.
			 * With no distinction between undefined variables, and null values,
			 * having a literal to represent it makes the behaviour of such values
			 * more intuitive.
			 */
			Rule null = "null"_E;
			/**
			 * A `new` expression: the keyword new followed by a class name.
			 */
			Rule newExpr      = "new"_E >> identifier;
			/**
			 * A variable declaration, optionally with an initialiser.
			 */
			Rule decl         = "var"_E >> identifier >> -('='_E >> expression) >> ';';
			/**
			 * A return statement.
			 */
			Rule ret          = "return"_E >> -expression >> ';';
			/**
			 * An else statement, that optionally follows an if statement.
			 */
			Rule elseStatement = "else"_E >> '{' >> statements >> '}';
			/**
			 * An if statement, with a condition in brackets followed by the body in
			 * braces.
			 */
			Rule ifStatement  = "if"_E >> '(' >> expression >> ')' >>
								'{' >> statements >> '}' >> -(elseStatement);
			/**
			 * A while loop, with the condition in brackets followed by the body in
			 * braces.
			 */
			Rule whileLoop    = "while"_E >> '(' >> expression >> ')' >>
								'{' >> statements >> '}';
			/**
			 * Classes are the keyword class, followed by the class name and optionally
			 * a superclass.  Instance variables must be declared first, then methods.
			 */
			Rule cls          = "class"_E >> identifier >> -(':'_E >> identifier) >> '{'
								 >> *decl >> *closure >> '}';
			/**
			 * All valid statement types.  Statements that are disambiguated by keywords
			 * are first.  All expressions are valid statements.
			 */
			Rule statement    = cls | ret | ifStatement | whileLoop | decl |
								((assignment | expression) >> ';');
			/**
			 * A list of statements: the top-level for programs in this grammar.
			 */
			Rule statements   = *(statement);
			/**
			 * Returns a singleton instance of this grammar.
			 */
			static MysoreScriptGrammar& get()
			{
				static MysoreScriptGrammar g;
				return g;
			}
			protected:
			/**
			 * Protected constructor.  This class is immutable, and so only the `get()`
			 * method should be used to return the singleton instance.
			 */
			MysoreScriptGrammar() {}
		};
	}
}
