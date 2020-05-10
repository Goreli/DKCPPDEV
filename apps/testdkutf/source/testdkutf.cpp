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

#include "dkutf_utf.hpp"
using namespace dk;

/*
CLI options:
 - Run. Execute UTs that are eligible to execute. Eligible UTs are those in an
intersection of the following two sets: (1) Unique Used Groups and (2) Unique
Tests. Uniqueness is identified by the key, not by the description. List them
as you go. A limiting filter applies;
 - Dry run. Only list UTs that are eligible to execute, but don't actually
execute them. A limiting filter applies;
 - Verify. Report issues if any detected. Not impacted by the limiting filter;
 - Verbose verify. Report all types of checks performed even if there are
no respective issues detected. Not impacted by the limiting filter;
 - Quiet - supress the copyright message and only list failed tests.
Display a progress bar as you go. Print a summary of counts of passed
and failed tests.
 - Combinations of Run, Dry Run, Verify and Verbose;
 - Help;
 - GUI driven mode(s) (reserved).
*/

int main() {
	UTGListVerifier utgVerifier;
	UTListVerifier utVerifier;

	verify(utgVerifier, utVerifier);
	run(utgVerifier.uniqueLinkedGroupList(),
		utVerifier.uniqueLinkedUnitTestList()
	);
	return 0;
}
