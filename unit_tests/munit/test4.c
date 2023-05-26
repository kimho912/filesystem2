#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <copy.h>

#include "munit.h"

#define TEXT_CONTENT "Unit testing the copy function."

//test cases
static MunitResult testing_copy(const MunitParameter params[], void *data){
    // silence compiler warnings
    (void)params;
    (void)data;

    //call the function with the input and output file
    data_memory_output("test.image", "test2.image");

    //open the output file and read its contents
    FILE* fp = fopen("test2.image", "r");
    if(fp == NULL){
        perror("Failed to open test output file.");
        return MUNIT_FAIL;
    }

    char buf[100];
    fgets(buf, 100, fp);

    //checking the string by assert
    munit_assert_string_equal(buf, TEXT_CONTENT);

    //close the file
    fclose(fp);

    return MUNIT_OK;

}

static void* test_setup(const MunitParameter params[], void* user_data){
    // silence compiler warnings
    (void)params;
    (void)user_data;

    FILE* fp = fopen("test.image", "w+");
    if(fp == NULL){
        perror("Failed to create a test input file");
        exit(EXIT_FAILURE);
    }

    //write text to the test input file
    fprintf(fp, "%s", TEXT_CONTENT);

    //close the file
    fclose(fp);

    return NULL;
}

//test cleanup function
static void test_teardown(void* data){
    // silence compiler warnings
    (void)data;

    remove("test.image");
    remove("test2.image");
}

//the test cases
static MunitTest test_suite_tests[] = {
    {
        "/testing copy",
        testing_copy,
        test_setup,
        test_teardown,
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
    MUNIT_TEST_OPTION_NONE
};

////running the test suite
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}