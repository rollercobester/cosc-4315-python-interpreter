# File: functional.py
# Content: test source code
# predecessor(), pred()
def p(x):
    if (x>0):
        return x-1
    else:
        return 0

def succ(x):
    if (x >= 0):
        return x+1
    else:
        return 0

def recadd(x,y):
    if y==0:
        return x
    else:
        return add(1,recadd(x,p(y)))
    
def add(x,y):
    z=x+y
    return z

def multiply(x,y):
    z=x*y
    return z

def multefficient(x,y):
    if x!=0:
        if y!=0:
            z=x*y
            return z
    return 0

def dup(x):
    z=2*x
    return z

def divide(a,b):
    if b!=0:
        c=a/b
        return c
    else:
        return -1
    
x1=add(multiply(1,3),multiply(4,6))
y1=1
y2=succ(0)
x2=multiply(add(y1,y1),add(2,2))
x3=dup(x1)
x4 =add(x1,x2)
y=divide(5,2)
print("x1=",x1)
print("x2=",x2)
print("x3=",x3)
print("x4=",x4)
print("y1=",y1)
print("y=",y)