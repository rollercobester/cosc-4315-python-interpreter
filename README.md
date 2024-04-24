# COSC 4315 // HW2 // basic python interpreter

## Coby Walters, Jason Pedder

This is a basic python interpreter that has the following functionality:

- string, boolean, and integer variables
- boolean and mathematical expressions
- if and else statements
- logical operator key words (and, or, not)
- indent syntax validation
- print statements with any amount of variables
- local and global scope
- recursive functions
- nested functions
- comments


To run this program, first build the program using one of the following commands:

1) Run the provided build script: `./build.sh`
2) `g++ -std=c++11 src/*.cpp -o mypython.exe`

Then, just run mypython.exe with the path of the python file you would like to run:

`./mypython.exe in01.py`

If you would like, I have a testing script that will automatically test the 21 test cases provided for phase 2.
It prints out the result, passed if output is an exact match.
All output files are sent to the testcases/output directory.

You can use it by running:

`python3 test.py`