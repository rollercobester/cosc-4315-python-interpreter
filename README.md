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

```python
a = 3
b = 4
c = 5

if (a > c or b > c):
    if (a > c and a > b):
        c = a
    else:
        c = b

if (a * a  + b * b == c * c):
    print("Triangle is a right triangle")
    perimeter_check = a + b + c < 10
    print("Perimeter is less than 10? ", perimeter)
else:
    print("Triangle is not a right triangle")
```