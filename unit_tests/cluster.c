#include "munit.h"
#include <cluster.h>

static MunitResult
clusterTest(const MunitParameter params[], void *data)
{
  // silence compiler warnings
  (void)params;
  (void)data;

  // set up input data
  Main_Boot MB;
  MB.BytesPerSectorShift = 9;
  MB.SectorsPerClusterShift = 3;
  MB.ClusterHeapOffset = 328;

  // check for an expected NULL
  void *result = cluster(&MB, 0);
  munit_assert_ptr_equal(result, NULL);

  result = cluster(&MB, 1);
  munit_assert_ptr_equal(result, NULL);

  result = cluster(NULL, 0);
  munit_assert_ptr_equal(result, NULL);

  // some valid values
  result = cluster(&MB, 2);
  munit_assert_ptr_equal((void *)&MB + 0x29000, result);

  /*
  cluster  2 heap starts at 0x7f294d629000 relative 29000
  cluster  3 heap starts at 0x7f294d62a000 relative 2a000
  cluster  4 heap starts at 0x7f294d62b000 relative 2b000
  cluster  5 heap starts at 0x7f294d62c000 relative 2c000
  cluster  6 heap starts at 0x7f294d62d000 relative 2d000
  cluster  7 heap starts at 0x7f294d62e000 relative 2e000
  */

  return MUNIT_OK;
}
/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */

static MunitTest test_suite_tests[] = {
    {(char *)"cluster", clusterTest, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite = {
    (char *)"",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE};

#include <stdlib.h>

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{
  return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}