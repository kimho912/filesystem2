#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <getopt.h>
#include <string.h>
#include <command.h>

#include "munit.h"

//testing the cmd_ops_process() function
static MunitResult command_testing(const MunitParameter params[], void *data){
    // silence compiler warnings
    (void)params;
    (void)data;

    int argc = 7;
    char* argv[] = {"./extfat", "-i", "test.image", "-o", "test2.image"};

    argStruct expected = {"test.image", "test2.image", 0, 0, 0, 0, 0};
    argStruct actual = cmd_ops_process(argc, argv);

    munit_assert_string_equal(expected.inFile, actual.inFile);
    munit_assert_string_equal(expected.outFile, actual.outFile);
    munit_assert_int(expected.cArg, ==, actual.cArg);
    munit_assert_int(expected.vArg, ==, actual.vArg);
    munit_assert_int(expected.dArg, ==, actual.dArg);

    return MUNIT_OK;
}

//the test cases
static MunitTest test_suite_tests[] = {
    { (char*) "/cmd_ops_process testing", 
        command_testing,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

//building the test suite
static MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

//running the test suite
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]){
    return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}