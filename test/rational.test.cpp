/**
 * @file	rational.test.cpp
 * @author  Essam A. El-Sherif <esm.elsh@gmail.com>
 * @version v1.0.0
 *
 * A testing module for the rational template class defined by rational.h
 */

#include <iostream>
#include <list>
#include <stdexcept>
#include <sstream>
#include <string>

#include "../include/rational.h"

using namespace esm;

struct {
	bool verbose;
} cmdOptions = { true };

typedef struct Test{
	void (*method)(void);
	std::string desc;
	bool skip;
}
*TestPtr;

typedef struct Suite{
	std::string desc;
	std::list<TestPtr> *testList;
}
*SuitePtr;

std::list<SuitePtr> suites;

void loadTestData();
void verboseExit();
void testRunner();
void makeTest(TestPtr);

inline void _assert(bool test, const char *msg = ""){
	if(!test) throw std::logic_error(msg);
}

void test1(); void test2(); void test3(); void test4(); void test5();
void test6(); void test7(); void test8(); void test9(); void test10();
void test11(); void test12(); void test13(); void test14(); void test15();
void test16(); void test17(); void test18(); void test19(); void test20();

/* Prepare test environment */
int testCount   = 1;
int passCount   = 0;
int failCount   = 0;
int cancelCount = 0;
int skipCount   = 0;
int todoCount   = 0;

int main(){

	loadTestData();
	testRunner();
/*

//	// Print some of the above results
//	cout << half << " + " << half << " = " << one << endl;
//	cout << one  << " - " << half << " = " << half << endl;
//	cout << two  << " * " << half << " = " << one << endl;
//	cout << one  << " / " << half << " = " << two << endl;
//	cout << "abs(" << minus_half << ")=" << half << endl;
//	cout << "2 * " << big << "=" << maxint
//		 << " (rational: " << rational<int>(maxint) << ")" << endl;
*/
	return 0;
}

void loadTestData(){

	SuitePtr suitePtr = nullptr;
	TestPtr testPtr = nullptr;

	// TEST SUITE #1 - Constructors
	suitePtr = new Suite();

	suitePtr->desc = "Test Suite #1 - Constructors";
	suitePtr->testList = new std::list<TestPtr>();

	suites.push_back(suitePtr);

	// TEST ### - Default constructor
	testPtr = new Test();

	testPtr->desc   = "Default constructor";
	testPtr->skip   = false;
	testPtr->method = test1;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - One argument constructor
	testPtr = new Test();

	testPtr->desc   = "One argument constructor";
	testPtr->skip   = false;
	testPtr->method = test2;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Two arguments constructor
	testPtr = new Test();

	testPtr->desc   = "Two arguments constructor";
	testPtr->skip   = false;
	testPtr->method = test3;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Two arguments constructor invalid
	testPtr = new Test();

	testPtr->desc   = "Two arguments constructor invalid";
	testPtr->skip   = false;
	testPtr->method = test4;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Copy constructor
	testPtr = new Test();

	testPtr->desc   = "Copy constructor";
	testPtr->skip   = false;
	testPtr->method = test5;

	suitePtr->testList->push_back(testPtr);

	// TEST SUITE #2 - Assignment
	suitePtr = new Suite();

	suitePtr->desc = "Test Suite #2 - Assignment";
	suitePtr->testList = new std::list<TestPtr>();

	suites.push_back(suitePtr);

	// TEST ### - Assignment
	testPtr = new Test();

	testPtr->desc   = "Assignment";
	testPtr->skip   = false;
	testPtr->method = test6;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Arithmetic assignment
	testPtr = new Test();

	testPtr->desc   = "Arithmetic assignment";
	testPtr->skip   = false;
	testPtr->method = test7;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Arithmetic assignment invalid
	testPtr = new Test();

	testPtr->desc   = "Arithmetic assignment invalid";
	testPtr->skip   = false;
	testPtr->method = test8;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Arithmetic assignment with int type
	testPtr = new Test();

	testPtr->desc   = "Arithmetic assignment with int type";
	testPtr->skip   = false;
	testPtr->method = test9;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Arithmetic assignment with int type invalid
	testPtr = new Test();

	testPtr->desc   = "Arithmetic assignment with int type invalid";
	testPtr->skip   = false;
	testPtr->method = test10;

	suitePtr->testList->push_back(testPtr);

	// TEST SUITE #3 - Member operators
	suitePtr = new Suite();

	suitePtr->desc = "Test Suite #3 - Member operators";
	suitePtr->testList = new std::list<TestPtr>();

	suites.push_back(suitePtr);

	// TEST ### - Increment / decrement
	testPtr = new Test();

	testPtr->desc   = "Increment / decrement";
	testPtr->skip   = false;
	testPtr->method = test11;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Operator not
	testPtr = new Test();

	testPtr->desc   = "Operator not";
	testPtr->skip   = false;
	testPtr->method = test12;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Boolean conversion
	testPtr = new Test();

	testPtr->desc   = "Boolean conversion";
	testPtr->skip   = false;
	testPtr->method = test13;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Comparison operators
	testPtr = new Test();

	testPtr->desc   = "Comparison operators";
	testPtr->skip   = false;
	testPtr->method = test14;

	suitePtr->testList->push_back(testPtr);

	// TEST SUITE #4 - Global operators
	suitePtr = new Suite();

	suitePtr->desc = "Test Suite #4 - Global operators";
	suitePtr->testList = new std::list<TestPtr>();

	suites.push_back(suitePtr);

	// TEST ### - Global unary operators
	testPtr = new Test();

	testPtr->desc   = "Global unary operators";
	testPtr->skip   = false;
	testPtr->method = test15;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Global binary operators
	testPtr = new Test();

	testPtr->desc   = "Global binary operators";
	testPtr->skip   = false;
	testPtr->method = test16;

	suitePtr->testList->push_back(testPtr);

	// TEST SUITE #5 - Others
	suitePtr = new Suite();

	suitePtr->desc = "Test Suite #5 - Others";
	suitePtr->testList = new std::list<TestPtr>();

	suites.push_back(suitePtr);

	// TEST ### - Absolute member function
	testPtr = new Test();

	testPtr->desc   = "Absolute member function";
	testPtr->skip   = false;
	testPtr->method = test17;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Type conversion
	testPtr = new Test();

	testPtr->desc   = "Type conversion";
	testPtr->skip   = false;
	testPtr->method = test18;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Sign handling
	testPtr = new Test();

	testPtr->desc   = "Sign handling";
	testPtr->skip   = false;
	testPtr->method = test19;

	suitePtr->testList->push_back(testPtr);

	// TEST ### - Overflow
	testPtr = new Test();

	testPtr->desc   = "Overflow";
	testPtr->skip   = false;
	testPtr->method = test20;

	suitePtr->testList->push_back(testPtr);
}

void testRunner(){

	std::atexit(verboseExit);
	if(cmdOptions.verbose) std::cerr << std::endl;

	std::list<SuitePtr>::const_iterator it1;
	for(it1 = suites.begin(); it1 != suites.end(); ++it1){

		std::string suiteDesc = (*it1)->desc;
		std::cerr << suiteDesc << std::endl;

		std::list<TestPtr> *testList = (*it1)->testList;

		std::list<TestPtr>::const_iterator it2;
		for(it2 = testList->begin(); it2 != testList->end(); ++it2){
			makeTest(*it2);
		}
	}

	if(cmdOptions.verbose) std::cerr << std::endl;
}

void makeTest(TestPtr ptr){

	const int testID = testCount++;

	std::ostringstream ss;
	ss << "... Test#" << std::setw(3) << std::setfill('0') << testID << " ... ";

	std::string preMsg = ss.str(), postMsg = preMsg;

	preMsg += "Initiate ... " + ptr->desc;
	if(cmdOptions.verbose) std::cerr << preMsg << std::endl;

	if(!cmdOptions.verbose){
		ptr->method();
	}
	else{
		try{
			ptr->method();
			passCount++;

			postMsg += "Success  ... " + ptr->desc;;
			if(cmdOptions.verbose) std::cerr << postMsg << std::endl;
		}
		catch(...){
			failCount++;

			postMsg += "Failure  ... " + ptr->desc;
			if(cmdOptions.verbose) std::cerr << postMsg << std::endl;
		}
	}
}

void verboseExit(){
	if(cmdOptions.verbose){
		std::cout << std::endl;
		std::cout << "▶ tests     " << --testCount   << std::endl;
		std::cout << "▶ suites    " << suites.size() << std::endl;
		std::cout << "▶ pass      " << passCount     << std::endl;
		std::cout << "▶ fail      " << failCount     << std::endl;
		std::cout << "▶ cancelled " << cancelCount   << std::endl;
		std::cout << "▶ skipped   " << skipCount     << std::endl;
		std::cout << "▶ todo      " << todoCount     << std::endl;
//		std::cout << "▶ duration_ms" <<  Math.round(Date.now() - startTime));
	}
}

/** Test#1 - Default constructor */
void test1(){
	_assert(rational<int>().numerator() == 0);
	_assert(rational<int>().denominator() == 1);
}

/** Test#2 - One argument constructor */
void test2(){
	_assert(rational<int>(1).numerator() == 1);
	_assert(rational<int>(1).denominator() == 1);

	_assert(rational<int>(-1).numerator() == -1);
	_assert(rational<int>(-1).denominator() == 1);

	_assert(rational<int>(0).numerator() == 0);
	_assert(rational<int>(0).denominator() == 1);
}

/** Test#3 - Two arguments constructor */
void test3(){
	_assert(rational<int>(1, 2).numerator() == 1);
	_assert(rational<int>(1, 2).denominator() == 2);

	_assert(rational<int>(-1, 2).numerator() == -1);
	_assert(rational<int>(-1, 2).denominator() == 2);

	_assert(rational<int>(1, -2).numerator() == -1);
	_assert(rational<int>(1, -2).denominator() == 2);

	_assert(rational<int>(-1, -2).numerator() == 1);
	_assert(rational<int>(-1, -2).denominator() == 2);

	_assert(rational<int>(0, 2).numerator() == 0);
	_assert(rational<int>(0, 2).denominator() == 1);
}

/** Test#4 - Two arguments constructor invalid */
void test4(){
	try{
		rational<int>(2, 0);
		throw bad_rational();
	}
	catch(bad_rational&){}
	catch(...){
		throw bad_rational();
	}
}

/** Test#5 - Copy constructor */
void test5(){
	rational<int> r(rational<long>(1, 2));

	_assert(r.numerator() == 1);
	_assert(r.denominator() == 2);
}

/** Test#6 - Assignment */
void test6(){
	rational<int> r = 3;

	_assert(r.numerator() == 3);
	_assert(r.denominator() == 1);

	r.assign(1, -2);
	_assert(r.numerator() == -1);
	_assert(r.denominator() == 2);
}

/** Test#7 - Arithmetic assignment */
void test7(){
	const rational<int> one(1);
	const rational<int> half(1, 2);
	rational<int> r;

	r = 1; r += one;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 2; r -= one;
	_assert(r.numerator() == 1);
	_assert(r.denominator() == 1);

	r = 2; r *= one;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 2; r /= one;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 1; r /= half;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 0; r /= one;
	_assert(r.numerator() == 0);
	_assert(r.denominator() == 1);

}

/** Test#8 - Arithmetic assignment invalid */
void test8(){
	const rational<int> zero;
	rational<int> r;

	try{
		r = 1;
		r /= zero;
		throw bad_rational();
	}
	catch(bad_rational&){}
	catch(...){
		throw bad_rational();
	}
}

/** Test#9 - Arithmetic assignment with int type */
void test9(){
	rational<int> r = 3;

	r = 1; r += 1;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 2; r -= 1;
	_assert(r.numerator() == 1);
	_assert(r.denominator() == 1);

	r = 2; r *= 1;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 2; r /= 1;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);

	r = 0; r /= 1;
	_assert(r.numerator() == 0);
	_assert(r.denominator() == 1);
}

/** Test#10 - Arithmetic assignment with int type invalid */
void test10(){
	rational<int> r;

	try{
		r = 1;
		r /= 0;
		throw bad_rational();
	}
	catch(bad_rational&){}
	catch(...){
		throw bad_rational();
	}
}

/** Test#11 - Increment/decrement */
void test11(){
	rational<int> r, s;

	r = 1; s = ++r;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);
	_assert(s == r);

	r = 1; s = --r;
	_assert(r.numerator() == 0);
	_assert(r.denominator() == 1);
	_assert(s == r);

	r = 1; s = r++;
	_assert(r.numerator() == 2);
	_assert(r.denominator() == 1);
	_assert(s != r);

	r = 1; s = r--;
	_assert(r.numerator() == 0);
	_assert(r.denominator() == 1);
	_assert(s != r);
}

/** Test#12 - Operator not */
void test12(){
	rational<int> r;

	r = 0;
	_assert(!r);

	r = 1;
	_assert(!!r);
}

/** Test#13 - Boolean conversion */
void test13(){
	rational<int> r;

	r = 1;
	_assert(r);
}

/** Test#14 - Comparison operators */
void test14(){
	_assert(rational<int>() < rational<int>(1));
	_assert(rational<int>(-1) < rational<int>());

	_assert(rational<int>() == rational<int>());
	_assert(rational<int>(-1) == rational<int>(-1));

	_assert(rational<int>() < 1);
	_assert(rational<int>(-1) < 0);

	_assert(rational<int>(1) > 0);
	_assert(rational<int>(0) > -1);

	_assert(rational<int>() == 0);
	_assert(rational<int>(-1) == -1);
}

/** Test#15 - Global unary operators */
void test15(){
	_assert( (+rational<int>(1, 2)).numerator() == 1);
	_assert( (+rational<int>(1, 2)).denominator() == 2);

	_assert( (+rational<int>(1, -2)).numerator() == -1);
	_assert( (+rational<int>(1, -2)).denominator() == 2);

	_assert( (-rational<int>(1, 2)).numerator() == -1);
	_assert( (-rational<int>(1, 2)).denominator() == 2);

	_assert( (-rational<int>(-1, 2)).numerator() == 1);
	_assert( (-rational<int>(-1, 2)).denominator() == 2);
}

/** Test#16 - Global binary operators */
void test16(){
	const rational<int> zero;
	const rational<int> half(1, 2);
	const rational<int> one(1);
	const rational<int> two(2);

	_assert(half + half == one);
	_assert(one - half == half);
	_assert(two * half == one);
	_assert(one / half == two);
	_assert(zero / one == zero);

	_assert(zero + 1 == one);
	_assert(one - 0 == one);
	_assert(two * 1 == two);
	_assert(two / 1 == two);
	_assert(zero / 1 == zero);

	_assert(1 + zero == one);
	_assert(1 - zero == one);
	_assert(2 * one == two);
	_assert(2 / one == two);
	_assert(0 / one == zero);

	_assert(half + half == 1);
	_assert(2 * half == one);
	_assert(2 * half == 1);
	_assert(one / half == 2);
	_assert(1 / half == 2);
}

/** Test#17 - Absolute member function */
void test17(){
	const rational<int> half(1, 2);
	const rational<int> minus_half(-1, 2);

	_assert(abs(minus_half) == half);
}

/** Test#18 - Type conversion */
void test18(){
	const rational<int> pi(22,7);

	_assert((rational_cast<double>(pi) - 22.0/7.0) < 1e-6);
}

/** Test#19 - Sign handling */
void test19(){

	const rational<int> half(1, 2);
	const rational<int> minus_half(-1, 2);

	_assert(-half == minus_half);
	_assert(abs(minus_half) == half);
}

/** Test#20 - Overflow */
void test20(){
	int maxint = (std::numeric_limits<int>::max)();
	rational<int> big(maxint, 2);

	_assert(2 * big == maxint);
}
