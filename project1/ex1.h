#pragma once

#include <string>
#include <stack>
#include <map>
#include <iostream>
#include "Expression.h"

#ifndef EX1_H
#define EX1_H

class BinaryOperator : public Expression {
protected:
	Expression *right, *left;
public:
	BinaryOperator(Expression *, Expression *);
	virtual ~BinaryOperator();
	virtual double calculate() = 0;
};

class Plus : public BinaryOperator {
public:
	Plus(Expression *, Expression *);
	virtual ~Plus();
	double calculate();
};
class Minus : public BinaryOperator {
public:
	Minus(Expression *, Expression *);
	virtual ~Minus();
	double calculate();
};
class Mul : public BinaryOperator {
public:
	Mul(Expression *, Expression *);
	virtual ~Mul();
	double calculate();
};
class Div : public BinaryOperator {
public:
	Div(Expression *, Expression *);
	virtual ~Div();
	double calculate();
};

class UnaryOperator : public Expression {
protected:
	Expression *exp;
public:
	UnaryOperator(Expression *);
	virtual ~UnaryOperator();
	virtual double calculate() = 0;
};	

class UPlus : public UnaryOperator {
public:
	UPlus(Expression *);
	virtual ~UPlus();
	double calculate();
};
class UMinus : public UnaryOperator {
public:
	UMinus(Expression *);
	virtual ~UMinus();
	double calculate();
};

class Variable : public Expression {
private:
	string name;
	double value;
public:
	Variable(string, double);
	virtual ~Variable();
	Variable &operator--();
	Variable &operator++();
	Variable &operator++(int);
	Variable &operator--(int);
	Variable &operator+=(double d);
	Variable &operator-=(double d);
	double calculate();
};

class Value : public Expression {
private:
	double val;
public:
	Value(double);
	virtual ~Value();
	double calculate();
};

class Interpreter {
private:
	map<string, double> variables;
public:
	virtual ~Interpreter();
	void setVariables(string);
	void setVariables(map<string, double> vars) {
		variables = vars;
	}
	Expression *interpret(string);
};

#endif
