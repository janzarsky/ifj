#!/bin/bash

echo "" > output

files=./tests/test*.java

for file in $files ; do
    echo "********************************************************************************"
    echo "* Testing file: $file"
    echo "********************************************************************************"
    ./ifj $file > output

    exit=$(echo -n $?)

    cat output
    
    head -n 2 $file > temp

    if [ "$(grep '//' temp | wc -l)" -gt "0" ]; then
        exp_exit=$(grep "exit" temp | awk '{ print $3 }')

        if [ "$exit" -eq "$exp_exit" ]; then
            status="PASS"
        else
            status="FAIL"
        fi

        grep "output" temp | cut -f 1,2 -d ' ' --complement > exp_output

        if [ "$(cat exp_output | wc -l)" -gt "0" ]; then
            diff output exp_output > /dev/null

            if [ "$?" -eq "0" ]; then
                output_status="PASS"
            else
                output_status="FAIL"
            fi
        else
            output_status="(x)"
        fi
    else
        exp_exit="(x)"
        exp_output="(x)"
        status="(x)"
        output_status="(x)"
    fi

    echo "********************************************************************************"
    echo "* Exit code:          $exit"
    echo "* Expected exit code: $exp_exit"
    echo "* Status:             $status"
    echo "* Output status:      $output_status"
    echo "********************************************************************************"
    echo ""

    echo -e "Test file: $file, exit: $exit,\t exp exit: $exp_exit,\t status: $status,\t output status: $output_status" >> summary
done

cat summary
rm temp summary output exp_output
