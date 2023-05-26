# for this project, there is only one makefile
#
# this makefile is expected to be run from the project 
# root.
#
# all executables are expected to be ran from the project 
# root.
#
# this makefile will clean up when asked politely
#
# No intermediate .o files or libraries are created


# CPPFLAGS=-Wall -Wextra -O0 -std=c17 -g3 -fsanitize=address -fsanitize=bounds-strict
CPPFLAGS=-Wall -Wextra -O0 -std=gnu17 -g3 -fsanitize=address
# note address sanitizer "-fsanitize=address" is new. it can be
# removed from the makefile if it causes problems.

CPPINCS=-I./include

all:mmap fread unit_tests extfat crc_example

# the utility that is a focus of this project
extfat:src/extfat.c common/routines.c src/command.c src/copy.c src/verify.c src/delete.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^


# unit tests
#unit_tests: munit_example 

unit_tests: munit_test1 munit_test2 munit_test3 munit_test4 munit_test5

# this test needs to be deleted once we get some real tests
# for the problem at hand
#munit_example:unit_tests/munit/example.c
#	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

munit_test1:unit_tests/munit/test1.c ./src/verify.c ./common/routines.c ./src/command.c
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

munit_test2:unit_tests/munit/test2.c ./src/command.c 
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

munit_test3:unit_tests/munit/test3.c ./src/print_directory.c 
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

munit_test4:unit_tests/munit/test4.c ./src/copy.c
	${CC} ${CPPFLAGS}  unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

munit_test5:unit_tests/munit/test5.c ./src/extfat.c ./src/command.c ./src/copy.c ./src/verify.c ./src/print_directory.c ./common/routines.c
	${CC} ${CPPFLAGS}  -DNOMAIN unit_tests/munit/munit.c -I./unit_tests/munit/ ${CPPINCS} -o $@ $^

system_tests: extfat
	bash tests/system_tests.bash

test_1: extfat
	bash tests/test_1.bash

test_2: extfat
	bash tests/test_2.bash

test_3: extfat
	bash tests/test_3.bash

test_4: extfat
	bash tests/test_4.bash

test_5: extfat
	bash tests/test_5.bash

# requirements tests

# example code
mmap:examples/mmap.c  common/routines.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

fread:examples/fread.c  
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^

crc_example:examples/crc_example.c
	${CC} ${CPPFLAGS} ${CPPINCS} -o $@ $^ -lz

# run tests
tests: run_unit_tests run_tests

run_unit_tests: munit_example
	./munit_example

run_unit_tests: munit_test1
	./munit_test1

run_unit_tests: munit_test2
	./munit_test2

run_unit_tests: munit_test3
	./munit_test3

run_unit_tests: munit_test4
	./munit_test4

run_unit_tests: munit_test5
	./munit_test5

run_tests:
	echo "here i would be running the requirements tests"

clean:
	-rm -f mmap fread munit_example extfat crc_example cluster munit_test1 munit_test2 munit_test3 munit_test4 munit_test5
