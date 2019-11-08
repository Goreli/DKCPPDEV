/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

*/

#include <iostream>
#include <memory>
#include "dk_cdt.hpp"
#include "dk_utg.hpp"
#include "dk_ut.hpp"

using namespace dk;

int main() {

	std::cout << "Groups" << std::endl;
	for (auto& pGrp : UnitTestGroup::list())
		std::cout << pGrp->getKey() << " " << pGrp->getDescription() << std::endl;

	std::cout << std::endl;
	std::cout << "Tests" << std::endl;
	for (auto& pTst : UnitTest::list()) {
		pTst->identify();
		pTst->initCompositeKey();
		std::cout << pTst->getKey().groupKey() << "." << pTst->getKey().testKey() << " " << pTst->getDescription() << std::endl;
		if (pTst->exec())
			std::cout << "\t" << "Passed" << std::endl;
		else
			std::cout << "\t" << "Failed" << std::endl;
	}
}
