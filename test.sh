#!/bin/bash

for file in test*.java ; do
    echo "********************************************************************************"
    echo "* Testing file: $file"
    echo "********************************************************************************"
    echo ""

    ./ifj $file | tee output

    echo ""
    echo ""
done
