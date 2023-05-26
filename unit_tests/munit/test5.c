#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <extfat.h>
#include <command.h>
#include <copy.h>
#include <verify.h>
#include <print_directory.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <routines.h>
#define _GNU_SOURCE

#include "munit.h"

void* file_info(fileInfo *file);

//testing the file_info() function
static MunitResult testing_file_info(const MunitParameter params[], void* data) {
    // silence compiler warnings
    (void)params;

    fileInfo* file = (fileInfo*) data;
    void* fp = file_info(file);

    // Ensure that the file was successfully opened and memory-mapped
    munit_assert_not_null(fp);
    munit_assert_int(file->fd, >=, 0);
    munit_assert_int(file->FileLength, >, 0);
    munit_assert_not_null(file->M_Boot);
    munit_assert_not_null(file->B_Boot);
    munit_assert_not_null(file->FAT);
    munit_assert_not_null(file->Data);

    return MUNIT_OK;
}

static void* test_setup(const MunitParameter params[], void* user_data) {
    // silence compiler warnings
    (void)params;
    (void)user_data;

    fileInfo* file = (fileInfo*) malloc(sizeof(fileInfo));
    file->fileName = "test_file.img";
    return (void*) file;
}

static void test_teardown(void* data) {
    fileInfo* file = (fileInfo*) data;
    free(file);
}

//the test cases
static MunitTest test_suite_tests[] = {
    {
        (char*)"",
        testing_file_info,
        test_setup,
        test_teardown,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

//building the test suite
static const MunitSuite test_suite = {
    (char*)"",
    test_suite_tests,
    NULL,
    1,
    MUNIT_TEST_OPTION_NONE
};

//running the test_suite
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}