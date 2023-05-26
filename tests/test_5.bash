#!/bin/bash
echo "Checking the "-o" command ouput"

expected_output="input file name: test.image 
output file name: test2.image "

actual_output=$(/workspaces/cse3310_s004_group_22/extfat -i test.image -o test2.image)
if [ "$expected_output" = "$actual_output" ]; then
    echo "Test passed: 
$actual_output"
    result=0
else
    echo "Test failed: expected '$expected_output' "
    echo "but got '$actual_output'"
    result=1
fi
echo""
exit $result

