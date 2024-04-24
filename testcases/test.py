x = 5

def fn_a():
    print("x =", x)
    return x

def fn_b(x):
    print("x =", x)
    fn_a()
    return x

b = fn_b(10)
print("b =", b)
