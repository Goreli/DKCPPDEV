#include <iostream>
#include <memory>
#include "dkmrx_matrix.hpp"
#include "dk_cdt.hpp"

using namespace dkmrx;
using namespace dk;

void test_0003_0001()
{
	matrix mrx3(3, 3, 1.0);
	matrix mrx2(2, 2, 1.0);
	matrix mrx1 = mrx2 + mrx3;
}

void test_0002_0004()
{
	matrix mrx2;
	matrix mrx1 = std::move(mrx2);
}
void test_0002_0003()
{
	matrix mrx2;
	matrix mrx1 = mrx2;
}
void test_0002_0002()
{
	matrix mrx2;
	matrix mrx1;

	mrx1 = std::move(mrx2);
}
void test_0002_0001()
{
	matrix mrx2;
	matrix mrx1;

	mrx1 = mrx2;
}


void test_0001_0004()
{
	ConstDestTracker obj2;
	ConstDestTracker obj1 = std::move(obj2);
}
void test_0001_0003()
{
	ConstDestTracker obj2;
	ConstDestTracker obj1 = obj2;
}
void test_0001_0002()
{
	ConstDestTracker obj2;
	ConstDestTracker obj1;

	obj1 = std::move(obj2);
}
void test_0001_0001()
{
	ConstDestTracker obj2;
	ConstDestTracker obj1;

	obj1 = obj2;
}

void executeTestFunction(const std::string strTestTitle, void(*pTstFun)(void)) {
	std::cout << strTestTitle << std::endl;
	ConstDestTracker::reset();
	pTstFun();

	std::cout << "\t" << "Constructors executed: " << ConstDestTracker::getConstCount()
		<< ". Destructors executed: " << ConstDestTracker::getDestCount()
		<< "." << std::endl;

	std::cout << "\t" << "Control sum: " << ConstDestTracker::getCtrlSum()
		<< "." << std::endl;

	// Use ANSI escape sequences to colorise the status output.
	if (ConstDestTracker::getConstCount() == ConstDestTracker::getDestCount()
		&& ConstDestTracker::getCtrlSum() == 0
		)
		std::cout << "\t" << "\033[3;42;30m" << "Succeeded" << "\033[0m" << std::endl;
	else {
		std::cout << "\t" << "\033[3;41;37m" << "Failed" << "\033[0m" << std::endl;
		std::cerr << "\t" << "\033[3;41;37m" << strTestTitle << " failed." << "\033[0m" << std::endl;
	}
}

int main() {
	executeTestFunction("Test (   1,    1)", test_0001_0001);
	executeTestFunction("Test (   1,    2)", test_0001_0002);
	executeTestFunction("Test (   1,    3)", test_0001_0003);
	executeTestFunction("Test (   1,    4)", test_0001_0004);

	executeTestFunction("Test (   2,    1)", test_0002_0001);
	executeTestFunction("Test (   2,    2)", test_0002_0002);
	executeTestFunction("Test (   2,    3)", test_0002_0003);
	executeTestFunction("Test (   2,    4)", test_0002_0004);

	executeTestFunction("Test (   3,    1)", test_0003_0001);
}
