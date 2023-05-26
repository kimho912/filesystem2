#!/bin/bash
echo "Checking the "-h" command ouput"


expected_output="
Options:
-i    Create or specify an input file (1 argument)
-o    Create or specify an output file (Set to same as input file by default) (1 argument)
-h    Print list of arguments and their meanings
-c    Copy the input file to the output file without change
-v    Check if the main and backup boot sectors are the same
Sorry, you need to include -i along with an input file name! (Use -h for help)"

actual_output=$(/workspaces/cse3310_s004_group_22/extfat -h)
if [ "$expected_output" = "$actual_output" ]; then
    echo "Test passed: $actual_output"
    result=0
else
    echo "Test failed: expected '$expected_output' "
    echo "but got '$actual_output'"
    result=1
fi
echo ""
exit $result

