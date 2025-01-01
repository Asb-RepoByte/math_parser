import sys
import math

"""
this is going to be a simple recursive decent implementation
where in the end I'm gonna have a simple math expression parser,
who knows in the end I myte even add support for function and varaibles
"""
"""
Grammars:
    E ::= T + E | T - E | T
    T ::= F * T | F / T | F
    F ::= Identifier | <Function> | Interger | (E) | -F
    Function ::= cos(E), sin(E), tan(E), abs(E), pow(E, E), gcd(E:int, E:int), norme(<E><n>)
"""

# some initial for optimezation
MAX_TERMS = 20
factorial_cached = [1]
for i in range(1, 2 * MAX_TERMS + 2):
    factorial_cached.append(factorial_cached[-1] * i)

PI = 3.14159265358979323846 


class Function:
    terminal = ""
    min_par = 1
    max_par = 1

    def __init__(self):
        self.paramters = []

    def show(self):
        content =  f'{self.terminal}['
        for i in range(len(self.paramters)):
            content += f"{self.paramters[i].show()}"
            if i < len(self.paramters) - 1:
                content += ", "
        content += "]"
        return content

    def eval(self):
        pass

class Sqrt(Function):
    terminal = "sqrt"
    min_par = 1
    max_par = 1

    def eval(self):
        n = self.paramters[0].eval()
        tolerence = 1e-10
        if n < 0:
            print(f"{self.paramters[0].show()} evaluated to a negative number")
            raise ValueError

        if n == 0 or n == 1: return n

        # initial guess
        x = n
        while True:
            y = 0.5 * (x + n / x)
            if abs(x - y) <= tolerence:
                return y
            x = y 

class Abs(Function):
    terminal = "abs"
    min_par = 1
    max_par = 1

    def eval(self):
        return abs(self.paramters[0].eval())

class Gcd(Function):
    terminal = "gcd"
    min_par = 2
    max_par = 2

    def eval(self):
        a = self.paramters[0].eval()
        b = self.paramters[1].eval()
        a, b = min(a, b), max(a, b)
        while b != 0:
            a, b = b, a % b
        return a

class Cos(Function):
    terminal = "cos"
    min_par = 1
    max_par = 1

    def eval(self):
        x = self.paramters[0].eval()
        x = x % (2 * PI)
        cose = 0
        for i in range(MAX_TERMS):
            sign = (-1) ** i # alternate between 1 and -1 respectively
            term = (x ** (2 * i)) / factorial_cached[2 * i]
            cose += sign * term
        return cose

class Sin(Function):
    terminal = "sin"
    min_par = 1
    max_par = 1

    def eval(self):
        x = self.paramters[0].eval()
        if abs(x) < 1e-5: return x
        x = x % (2 * PI)
        sine = 0
        for i in range(MAX_TERMS):
            sign = (-1) ** i # alternate between 1 and -1 respectively
            term = (x ** (2 * i + 1)) / factorial_cached[2 * i + 1]
            sine += sign * term
        return sine


class Tan(Function):
    terminal = "tan"
    min_par = 1
    max_par = 1

    def eval(self):
        cos = Cos()
        cos.paramters  = self.paramters.copy()
        sin = Sin()
        sin.paramters  = self.paramters.copy()
        cos = cos.eval()
        sin = sin.eval()
        if cos == 0:
            print(f"tan of n * pi / 2 cause division by zero you stupid ass")
            raise ValueError
        return sin / cos

class Acos(Function):
    terminal = "acos"
    min_par = 1
    max_par = 1

    def eval(self):
        return math.acos(self.paramters[0].eval())

class Asin(Function):
    terminal = "asin"
    min_par = 1
    max_par = 1

    def eval(self):
        return math.asin(self.paramters[0].eval())

class Atan(Function):
    terminal = "atan"
    min_par = 1
    max_par = 1

    def eval(self):
        return math.atan(self.paramters[0].eval())


class Pow(Function):
    terminal = "pow"
    min_par = 2
    max_par = 2

    def eval(self):
        x = self.paramters[0].eval()
        y = self.paramters[1].eval()

        if (x == 0 and y >= 0) or x == 1: return x
        if x == 0 and y < 0:
            print(f"{self.paramters[0].show()} evaluated to zero")
            print(f"{self.paramters[1].show()} < zero")
            print(f"zero to the power of negative isn't allowed you dum F***")
            raise ValueError

        result = x
        for i in range(abs(y)):
            result *= x
        if y < 0:
            result = x / result

        return result

class Norme(Function):
    terminal = "norme"
    min_par = 2
    max_par = 1000

    def eval(self):
        result = 0
        for val in self.paramters:
            result += val.eval() ** 2
        return math.sqrt(result)

possible_function = [Sqrt, Cos, Sin, Tan, Acos, Asin, Atan, Pow, Norme, Abs, Gcd]


class TreeNode:
    def __init__(self, left=None, right=None):
        self.left = left
        self.right = right

    def show(self):
        return f"({self.left.show()} {self.terminal} {self.right.show()})"

    def eval(self):
        if self.terminal == "+":
            return self.left.eval() + self.right.eval()
        if self.terminal == "-":
            return self.left.eval() - self.right.eval()
        if self.terminal == "*":
            return self.left.eval() * self.right.eval()
        if self.terminal == "/":
            return self.left.eval() / self.right.eval()

class Numeric:
    def __init__(self, value=0):
        self.value = value

    def eval(self):
        return self.value

    def show(self):
        return f"{self.value}"

    def __str__(self):
        return f"{self.value}"

class Identifier:
    def __init__(self):
        self.name = ""

class Negate():
    terminal = "-"
    def __init__(self, child=None):
        self.child = child

    def show(self):
        return f"(-{self.child.show()})"

    def eval(self):
        return -self.child.eval()

class Mul(TreeNode):
    terminal = "*"

class Div(TreeNode):
    terminal = "/"

class Substract(TreeNode):
    terminal = "-"

class Add(TreeNode):
    terminal = "+"

def scan_token():
    global expression
    global next_token
    global next_index

    token = ""
    while next_index < (len(expression)):
        ch = expression[next_index]
        if ch == " ":
            next_index += 1
            continue
        if ch in "()":
            next_token = ch
            next_index += 1
            return
        if ch in "+-*/,":
            next_token = ch
            next_index += 1
            return
        if ch.isdigit():
            while next_index < len(expression) and (expression[next_index].isdigit() or expression[next_index] == "."):
                token += expression[next_index]
                next_index += 1
            if "." in token:
                next_token = Numeric(float(token))
            else:
                next_token = Numeric(int(token))
            return
        if ch.isalpha():
            token = ""
            while next_index < len(expression) and expression[next_index].isalpha():
                token += expression[next_index]
                next_index += 1
            for func_class in possible_function:
                if func_class.terminal == token:
                    next_token = token
                    return
            return
        else:
            next_token = None
            print(expression)
            print(f"'{ch}' syntax is not allowed")
            return
    next_token = None
    return

def parseE():
    #print("parse E")
    a = parseT()
    if not a: return None
    while True:
        if next_token == "+":
            scan_token()
            b = parseT()
            a = Add(a, b)
            if not a: return None
        elif next_token == "-":
            scan_token()
            b = parseT()
            a = Substract(a, b)
            if not a: return None
        else:
            return a

def parseT():
    #print("parse T")
    a = parseF()
    scan_token()
    if not a: return None
    if next_token == "*":
        scan_token()
        b = parseT()
        if not b: return None
        return Mul(a, b)
    if next_token == "/":
        scan_token()
        b = parseT()
        if not b: return None
        return Div(a, b)
    else:
        return a

def parseF():
    #print("parse F")
    if isinstance(next_token, Numeric):
        #print(f"{next_token} detected")
        return next_token
    if "(" == next_token:
        scan_token()
        a = parseE()
        if not a: return None
        if ")" == next_token:
            return a
        else:
            return None
    if next_token == "-":
        scan_token()
        return Negate(parseF())
    else:
        return parseFunc()

def parseFunc():
    func_class = None
    for fun in possible_function:
        if fun.terminal == next_token:
            func_class = fun
    scan_token()
    if "(" == next_token:
        scan_token()
        fc = func_class()
        a = parseE()
        if not a: return None
        fc.paramters.append(a)
        i = 1
        while next_token == "," and i <= fc.max_par:
            scan_token()
            a = parseE()
            if not a: return None
            fc.paramters.append(a)
        if next_token == ")":
            return fc
        else:
            print("something with function went wrong")
            return None
    else: return None


def main(my_expression=""):
    global expression
    global next_index
    global next_token
    if my_expression:
        expression = my_expression
        next_token = None
        next_index = 0
    scan_token()
    result_tree = parseE()
    if not result_tree:
        print("something went wrong")
        return
    """
    if next_token:
        print(f"{next_token} left the expression is faulty")
        return
    """
    return result_tree

def lexer():
    global next_index
    global next_token

    scan_token()
    while next_token:
        print(next_token)
        scan_token()
    next_token = None
    next_index = 0

if __name__ == "__main__":
    #Function ::= cos(E), sin(E), tan(E), abs(E), pow(E, E), gcd(E:int, E:int), norme(<E><n>)
    next_index = 0
    next_token = None
    if sys.argv[1]:
        expression = sys.argv[1]
    else:
        expression = "1 + 2"
    lexer()
    print("########### eval #############")
    result_tree = main()
    print(f"show: {result_tree.show()}")
    print(f"eval: {result_tree.eval()}")
