#include "../include/command.h"
#include "../include/copy.h"
#include "../include/verify.h"
#include "../include/print_directory.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#define _GNU_SOURCE
// #define _POSIX_SOURCE
#undef _POSIX_SOURCE

#include "munit.h"

//Test the explore() function
static MunitResult testing_directory(const MunitParameter params[], void* data){
    // silence compiler warnings
    (void)params;
    (void)data;

    char* dir_path = "/workspaces/cse3310_s004_group_22";

    //call explore() and check return value
    int result = explore(dir_path, 0);
    munit_assert_int(result, ==, 1);

    return MUNIT_OK;
}

//the test cases
static MunitTest test_suite_tests[] = {
    {
        (char*)"/testing_explore",
        testing_directory,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

//building the test suite
static const MunitSuite test_suite = {
    (char*)"",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

//running the test suite
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}