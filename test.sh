#!/bin/bash

# Set Python executable name based on the platform
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    PYTHON_EXECUTABLE="./mypython.exe"
    FILE_COMPARISON_COMMAND="fc /b"
else
    PYTHON_EXECUTABLE="python3"
    FILE_COMPARISON_COMMAND="diff -q"
fi

# Directory containing test cases
TESTCASES_DIR="./testcases"

# Directory to store output files
OUTPUT_DIR="./output"

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Iterate over each input file in the testcases directory
for input_file in "$TESTCASES_DIR"/in*.py; do
    # Extract the filename without directory and extension
    filename=$(basename -- "$input_file")
    filename_no_ext="${filename%.*}"

    # Run the Python program with the current input file
    $PYTHON_EXECUTABLE "$TESTCASES_DIR/$filename_no_ext.py" < "$input_file" > "$OUTPUT_DIR/output_$filename_no_ext.txt"

    # Get the corresponding expected output file
    expected_output_file="$TESTCASES_DIR/out${filename_no_ext#in}.txt"

    # Compare the output file with the expected output file based on platform
    if $FILE_COMPARISON_COMMAND "$OUTPUT_DIR/output_$filename_no_ext.txt" "$expected_output_file" > /dev/null; then
        echo "Test $filename_no_ext passed"
    else
        echo "Test $filename_no_ext failed"
    fi
done
