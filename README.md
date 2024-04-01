# COSC 4315 // HW2 // basic python interpreter

## Coby Walters, Jason Pedder

This is a basic python interpreter that has the following functionality:

- string, boolean, and integer variables
- boolean and mathematical expressions
- if and else statements
- indent syntax validation
- print statements

To run this program, first build the program using one of the following commands:

1) Run the provided build script: `./build.sh`
2) `g++ -std=c++11 src/*.cpp -o mypython.exe`

Then, just run mypython.exe with the path of the python file you would like to run:

`./mypython.exe in01.py`


Heres an example of a file that uses the full functionality of the interpreter so far:

```python
a = 5
b = 12
c = 13

if a > c or b > c:
    print("Invalid input")
else:
    print("Calculating..") # For some reason our parser needs a line before another if statement
    if a * a  + b * b == c * c:
        print("Triangle is a right triangle")
        perimeter = a + b + c < 15
        print("Perimeter less than 15? ", perimeter)
    else:
        print("Triangle is not a right triangle")
```