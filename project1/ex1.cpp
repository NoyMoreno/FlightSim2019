#include "ex1.h"

// Section: Binary Operator
BinaryOperator::BinaryOperator(Expression *l, Expression *r) : right(r), left(l) {}
BinaryOperator::~BinaryOperator() { 
	delete left;
	delete right;
}
// Sub-Section: Constructor+Destructors
Plus::Plus(Expression *l, Expression *r) : BinaryOperator(l, r) {}
Plus::~Plus() {}
Minus::Minus(Expression *l, Expression *r) : BinaryOperator(l, r) {}
Minus::~Minus() {}
Div::Div(Expression *l, Expression *r) : BinaryOperator(l, r) {}
Div::~Div() {}
Mul::Mul(Expression *l, Expression *r) : BinaryOperator(l, r) {}
Mul::~Mul() {}

// Sub-Section: Calculate functions
double Plus::calculate() {
	return left->calculate() + right->calculate();
}
double Minus::calculate() {
	return left->calculate() - right->calculate();
}
double Div::calculate() {
	double l = left->calculate();
	double r = right->calculate();;
	if (r == 0)
		throw "division by zero";
	return l / r;
}
double Mul::calculate() {
	return left->calculate() * right->calculate();
}

// Section: Unary Operator
UnaryOperator::UnaryOperator(Expression *e) : exp(e) {}
UnaryOperator::~UnaryOperator() { 
	delete exp;
}
// Sub-Section: Constructor+Destructors
UPlus::UPlus(Expression *e) : UnaryOperator(e) {}
UPlus::~UPlus() {}
UMinus::UMinus(Expression *e) : UnaryOperator(e) {}
UMinus::~UMinus() {}

// Sub-Section: Calculate functions
double UPlus::calculate() { 
	return + exp->calculate();
}
double UMinus::calculate() {
	return -exp->calculate();
}


// Section: Variable
Variable::Variable(const string n, double v) : name(n), value(v) {}
Variable::~Variable() { }
// Sub-Section: Operators
Variable &Variable::operator++() {
	value++;
	return *this;
}
Variable &Variable::operator++(int x) {
	value += x == 0 ? 1 : x;
	return *this;
}
Variable &Variable::operator--() {
	value--;
	return *this;
}
Variable &Variable::operator--(int x) {
	value -= x == 0 ? 1 : x;
	return *this;
}
Variable &Variable::operator+=(double d) {
	value += d;
	return *this;
}
Variable &Variable::operator-=(double d) {
	value -= d;
	return *this;
}
// Sub-Section: Calculatge
double Variable::calculate() {
	return value;
}

// Section: Value
Value::Value(double d) : val(d) {}
Value::~Value() {}
double Value::calculate() {
	return val;
}

// Section: Interpreter
Interpreter::~Interpreter() { }
void Interpreter::setVariables(string s) {
	// Validate no spaces, and nothing aside from letters, equals, numbers, dots, semicolon, and dash (minus) or underscore
	for (char c : s) {
		// Not lowercase?
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '.' || c == ';' || c == '=' || c == '-' || c == '_' || (c >= '0' && c <= '9'))
			continue;
		// Bad!
		throw "illegal variable assignment!";//"bad input, invalid char";
	}
	
	// We want to split by semicolon
	size_t prevInd = 0;
	while (prevInd < s.length()) {
		size_t nextInd = s.find(';', prevInd);
		if (nextInd > s.length()) nextInd = s.length();
		// get our current variable
		string curVar = s.substr(prevInd, nextInd - prevInd);
		prevInd = nextInd + 1;
		// Empty? ignore
		if (curVar.length() == 0)
			continue;
		// Missing an equals? bad!
		size_t equalsInd = curVar.find('=');
		if (equalsInd > curVar.length())
			throw "illegal variable assignment!";//"Error. Missing equals defining value";
		// Split by equal
		string name = curVar.substr(0, equalsInd);
		string value = curVar.substr(equalsInd + 1, curVar.length() - equalsInd - 1);
		// Either empty? bad
		if (name.length() == 0 || value.length() == 0)
			throw "illegal variable assignment!";//"Error. Empty Name/Value";
		// Name must start with a letter
		if (!((name[0] >= 'a' && name[0] <= 'z') || (name[0] >= 'A' && name[0] <= 'Z') || name[0] == '_'))
			throw "illegal variable assignment!";//"Error. Name must start with lowercase letter";
		// Number must only have values
		double d;
		try {
			d = stod(value);
		}
		catch (std::invalid_argument) {
			throw "illegal variable assignment!";//"Error. Value cannot be convered to double";
		}
		catch (std::out_of_range) {
			throw "illegal variable assignment!";//"Error. Value cannot be convered to double - out of range";
			continue;
		}

		// Add it in
		variables[name] = d;
	}// next variables
	// done
}

string remove_parens(string s) {
	if (s[0] == '(' && s[s.length() - 1] == ')') {
		// Check if it's surrounded...
		int totalOpen = 1;
		size_t c;
		for (c = 1; c < s.length(); c++) {
			if (s[c] == '(')
				totalOpen++;
			if (s[c] == ')')
				totalOpen--;
			// Are we at zero? break
			if (totalOpen == 0) break;
		}
		// Did we hit zero before the end?
		if (c < s.length() - 1)
			return s;
		// Remove the parens, try another layer
		return remove_parens(s.substr(1, s.length() - 2));
	}
	return s;
}

Expression *Interpreter::interpret(string s) {
	// Validate no spaces, and nothing aside from letters, numbers, dots **No semicolon and no equals
	// In addition, allow *+-/ and parens
	for (char c : s) {
		// Not lowercase?
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '.' || c == '_' || (c >= '0' && c <= 'z') || c == '+' || c == '-' || c == '/' || c == '*' || c == '(' || c == ')')
			continue;
		// Bad!
		throw "bad input";//"bad input, invalid char";
	}
	
	Expression *ret = NULL;
	std::stack<char> unaryWrappers;
	std::stack<char> localUnaryWrappers;
	// We want to breakdown the current request
	// Find the first operator which splits it into two
	// Go, keeping track of parenthesees
	size_t breakInd = -1; int totalOpen = 0;
	while (++breakInd < s.length()) {
		// Are we at a parenthesees?
		if (s[breakInd] == '(')
			totalOpen++;
		if (s[breakInd] == ')')
			totalOpen--;
		// In the middle? continue
		if (totalOpen != 0) continue;
		// Did we hit a operator?
		if (s[breakInd] == '*' || s[breakInd] == '+' || s[breakInd] == '-' || s[breakInd] == '/') {
			// If we are in unary, just stop here
			if (breakInd == 0) {
				// If not followed by number or parens, bad
				if (s[1] != '(' && (s[1] < '0' || s[1] > '9'))
					throw "illegal math expression";

				if (s[breakInd] == '+')
					unaryWrappers.push('+');
				else if (s[breakInd] == '-')
					unaryWrappers.push('-');
				else throw "illegal math expression";//"Unknown unary operator";
				// Remove it and continue
				s = s.substr(1, s.length() - 1);
				string news = remove_parens(s);
				if (news.compare(s) == 0) {
					localUnaryWrappers.push(unaryWrappers.top());
					unaryWrappers.pop();
				}
				else s = news;
					
				breakInd--;
				continue;
			}

			string sR = s.substr(breakInd + 1, s.length() - breakInd - 1);
			string sL = s.substr(0, breakInd);
			// First calculate the right
			Expression *r = interpret(remove_parens(sR));
			Expression *l = interpret(remove_parens(sL));
			
			while (!localUnaryWrappers.empty()) {
				char c = localUnaryWrappers.top();
				localUnaryWrappers.pop();
				// Wrap
				if (c == '+') l = new UPlus(l);
				else l = new UMinus(l);
			}


			// This is the break point!
			if (s[breakInd] == '*')
				ret = new Mul(l, r);
			if (s[breakInd] == '+')
				ret = new Plus(l, r);
			if (s[breakInd] == '-')
				ret = new Minus(l, r);
			if (s[breakInd] == '/')
				ret = new Div(l, r);
			break;
		}
	}
	if (breakInd >= s.length() && ret == NULL) { 
		// Are we at the end of a string?
		// Is it a variable?
		auto it = variables.find(s);
		if (it == variables.end()) {
			// Must be a number
			try {
				ret = new Value(stod(s));
			}
			catch (std::invalid_argument) {
				throw "illegal math expression";//"Invalid input, cannot be variable or number";
			}
		}
		else ret = new Variable(s, it->second);
	}
	// We might have one local left, so first do that
	if (!localUnaryWrappers.empty()) {
		unaryWrappers.push(localUnaryWrappers.top());
		localUnaryWrappers.pop();
		// If it's not empty - invalid
		if (!localUnaryWrappers.empty())
			throw "illegal math expression";
	}
	// Wrap it
	while (!unaryWrappers.empty()) {
		char c = unaryWrappers.top();
		unaryWrappers.pop();
		if (c == '-') ret = new UMinus(ret);
		else ret = new UPlus(ret);
	}
	return ret;
}
