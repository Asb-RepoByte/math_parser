# Info
**@moctar 5la9 anak 4i partie**

this is a math parser that support functions like {sin, cos, tan, pow, gcd, etc ...}
and any combination of the operations and float/int and '()' and functions will
get evaluated correctly if it followed the Grammar
## 4a la5ruju
Grammars:

    E ::= T + E | T - E | T

    T ::= F * T | F / T | F

    F ::= Identifier | Function | Interger | (E) | -F

    Function ::= cos(E), sin(E), tan(E), abs(E), pow(E, E), gcd(E:int, E:int), norme(Ei)

E is an expression, T is a term F is a factor, Function is a function

# usage
## of the recursive_descent.py file
- `python recursive_descent.py "math expressioin"`
    this will evaluate and print the result of the given math expression
- example:
    `python recursive_descent.py "(7 - -4) + sqrt(4) - pow(gcd(25, 5), 4)"`
- output:
```
    (
    7
    -
    -
    4
    )
    +
    sqrt
    (
    4
    )
    -
    pow
    (
    gcd
    (
    25
    ,
    5
    )
    ,
    4
    )
########### eval #############
    show: (((7 - (-4)) + sqrt[4]) - pow[gcd[25, 5], 4])
    eval: -3112.0
```
- if no math expression is given the "1 + 2" will be evaluated

## of the test.py file
- `python test.py [ n ]` 
    to test n math expression from the database constraint: 1 <= n <= 1_000_000
    if 'n' is ommited then the all the math expressions in the DB will be evaluated which can take some time
- example:
    `python test.py 100`
- output:
```
    27/38 are valid expression
    11/38 are wrong check the log.txt file for more detaills
    62/100 are skiped over
    my calculator is 71.05263157894737% reliable for this data set
```

# Requirement
basicly nothing
