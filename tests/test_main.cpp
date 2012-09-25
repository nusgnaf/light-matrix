/**
 * @file test_main.cpp
 *
 * The main file for a testing program
 *
 * @author Dahua Lin
 */

#include <light_test/std_test_mon.h>

#ifdef _MSC_VER
#pragma warning(disable:4100)
#endif

using namespace ltest;

extern void larr_add_test_packs();
extern ltest::test_suite larr_main_suite;

int main(int argc, char *argv[])
{
	::larr_add_test_packs();

	if (std_test_main(larr_main_suite))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}




