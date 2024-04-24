# Test file for a basic Python interpreter

# Demonstrate variable types and assignments
string_var = "Hello, world!"
int_var = 42
bool_var = True

# Print variables
print("String Variable:", string_var)
print("Integer Variable:", int_var)
print("Boolean Variable:", bool_var)

# Demonstrate mathematical expressions
sum = int_var + 58
diff = int_var - 22
product = int_var * 2
quotient = int_var / 7
print("Sum:", sum)
print("Difference:", diff)
print("Product:", product)
print("Quotient:", quotient)

# Demonstrate boolean expressions and logical operators
bool_expr1 = not bool_var
bool_expr2 = (int_var > 40) and (int_var < 45)
bool_expr3 = (int_var == 42) or (int_var == 50)
print("Boolean Expression 1 (not):", bool_expr1)
print("Boolean Expression 2 (and):", bool_expr2)
print("Boolean Expression 3 (or):", bool_expr3)

# Demonstrate if-else statements
if int_var == 42:
    print("The answer to life, the universe, and everything.")
else:
    print("Keep calculating.")

# Demonstrate local and global scope
def test_scope():
    local_var = "I'm local."
    print(local_var)
    print(string_var)

test_scope()

# Demonstrate recursive function (factorial)
def factorial(n):
    if n == 0:
        return 1
    else:
        return n * factorial(n - 1)

print("Factorial of 5:", factorial(5))

# Demonstrate nested functions
def outer():
    outer_var = "Outside!"

    def inner():
        inner_var = "Inside!"
        print(outer_var)
        print(inner_var)

    inner()

outer()

# Comments in code
# This is a single-line comment.

# Done!