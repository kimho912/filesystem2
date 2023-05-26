#include <verify.h>
#include <routines.h>

#include "munit.h"

static MunitResult verify_test(const MunitParameter params[], void* data){
    // silence compiler warnings
    (void)params;
    (void)data;

    Main_Boot* mb = (Main_Boot*)data;

    //call the function with test data
    int result = verify_file("test.image", mb);

    //checking the result to the actual return value
    munit_assert_int(result, ==, 1);

    return MUNIT_OK; 
}

static void* test_setup(const MunitParameter params[], void* user_data){
    // silence compiler warnings
    (void)params;
    (void)user_data;

    //set up any necessary test data here 
    Main_Boot* mb = malloc(sizeof(Main_Boot));

    //initialize the struct values as necessary
    return mb;
}

static void test_teardown(void* data){
    //clean up
    free(data);
}

//the test cases
static MunitTest test_suite_tests[] = {
    {
        "/verify_test",
        verify_test,
        test_setup,
        test_teardown,
        MUNIT_SUITE_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

//building the test suite
static const MunitSuite test_suite = {
    (char*)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

//running the test suite
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}