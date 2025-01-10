#include "recur.h"

/*
this is going to be a simple recursive decent implementation
where in the end I'm gonna have a simple math expression parser,
who knows in the end I myte even add support for function and varaibles
"""
"""
Grammars:
    E ::= T [+, -] {T} 
    T ::= F [*, /] {F}
    F ::= Identifier | <Function> | Interger | (E) | -F
    Function ::= cos(E), sin(E), tan(E), abs(E), pow(E, E), gcd(E:int, E:int), norme(<E><n>)
*/

char expression[MAX_EXP_SIZE]{};
size_t lengthExp{}; // size of the expression because this will a null terminated string we actually need to get the lenght
Token nextToken{};
size_t nextIndex{};


TreeNode::TreeNode(
      Token token,
      TreeNode* left,
      TreeNode* right
  ) : token { token }, left{ left }, right{ right}
  {
    for (size_t i = 0; i < MAX_PARAMETERS; i++) {
      this->pars[i] = NULL;
    }
    switch (token.type) {
      case TokenType::M_Int:
        this->f_value = atof(&this->token.value[0]);
        break;
      case TokenType::M_Float:
        this->f_value = atof(&this->token.value[0]);
        break;
      case TokenType::M_FUNC:
        this->terminal = &this->token.value[0];
      case TokenType::M_Symbol:
        this->terminal = &this->token.value[0];
        break;
      default:
        std::cout << "case Unknown this is carzy work\n";
        break;

    }
  }

TreeNode::~TreeNode() {
  if (this->left) { delete this->left; }
  if (this->right) { delete this->right; }
  for (int i = 0; i < MAX_PARAMETERS; i++) {
    delete this->pars[i];
  }
}

void TreeNode::show() {
    if (this->token.type == TokenType::M_Int | this->token.type == TokenType::M_Float) {
      std::cout << this->f_value;
      return;
    }
    if (this->token.type == TokenType::M_Symbol && this->left && !this->right) {
      std::cout << "(-";
      this->left->show();
      std::cout << ")";
      return;
    }
    if (this->token.type == TokenType::M_Symbol) {
      std::cout << "(";
      this->left->show();
      std::cout << " " << this->token.value[0] << " ";
      this->right->show();
      std::cout << ")";
      return;
    }
    if (this->token.type == TokenType::M_FUNC) {
      std::cout << token.value << "[";
      int i = 0;
      while (this->pars[i]) {
        this->pars[i]->show();
        std::cout << ", ";
        i++;
      }
      
      std::cout << "\b\b]";
    }
  }

double TreeNode::eval() {
  if (this->token.type == TokenType::M_Int | this->token.type == TokenType::M_Float) {
    return this->f_value;
  }
  if (this->token.type == TokenType::M_Symbol && this->left && !this->right) {
    return -this->left->eval();
  }
  if (this->token.type == TokenType::M_Symbol && this->token.value[0] == '+') {
    return this->left->eval() + this->right->eval();
  }
  if (this->token.type == TokenType::M_Symbol && this->token.value[0] == '-') {
    return this->left->eval() - this->right->eval();
  }
  if (this->token.type == TokenType::M_Symbol && this->token.value[0] == '*') {
    return this->left->eval() * this->right->eval();
  }
  if (this->token.type == TokenType::M_Symbol && this->token.value[0] == '/') {
    return this->left->eval() / this->right->eval();
  }
  if (this->token.type == TokenType::M_FUNC) {
    switch (this->token.func) {
      case (Functions::Sqrt):
        return sqrt(this->pars[0]->eval());
        break;
      case (Functions::Pow):
        return pow(this->pars[0]->eval(), this->pars[1]->eval());
        break;
      case (Functions::Sin):
        return sin(this->pars[0]->eval());
        break;
      case (Functions::Cos):
        return cos(this->pars[0]->eval());
        break;
      case (Functions::Tan):
        return tan(this->pars[0]->eval());
        break;
      case (Functions::ASin):
        return asin(this->pars[0]->eval());
        break;
      case (Functions::ACos):
        return acos(this->pars[0]->eval());
        break;
      case (Functions::ATan):
        return atan(this->pars[0]->eval());
        break;
      case (Functions::Exp):
        return exp(this->pars[0]->eval());
        break;
      case (Functions::Log2):
        return log2(this->pars[0]->eval());
        break;
      case (Functions::Log10):
        return log10(this->pars[0]->eval());
        break;
      case (Functions::Ln):
        return log(this->pars[0]->eval());
        break;
      default:
        std::cout <<  "Unknown functions" << std::endl;
        return -1;
        break;
    }
  }
  return 0;

}


// begin utility functions
size_t getLen(const char* str) {
  size_t lenght = 0;
  while (str[lenght] != '\0') {
    lenght++;
  }
  return lenght;
}

void copyStr(const char* str, char* buffer) {
  size_t string_lenght = getLen(str);
  if (string_lenght > MAX_TOKEN_SIZE ) {
    std::cout << "Copy of " << str << "Failed str too long ( Not something i here to often)\n";
    return;
  }
  size_t index = 0;
  while (index < string_lenght) {
    buffer[index] = str[index];
    index++;
  }
  buffer[index++] = '\0';
  buffer[index] = '\0';
  return;
}

bool strChr(const char* str, char ch) {
  size_t len = getLen(str);
  for (size_t i = 0; i < len; i++) {
    if (str[i] == ch) {
      return true;
    }
  }
  return false;
}

bool isEqStr(const char* str, char* buffer) {
  size_t string_lenght = getLen(str);
  if (string_lenght > MAX_TOKEN_SIZE ) {
    std::cout << "Copy of " << str << "Failed str too long ( Not something i here to often)\n";
    return false;
  }
  size_t index = 0;
  while (index < string_lenght) {
    if (buffer[index] != str[index]) { return false; }
    index++;
  }
  return true;
}
// end utility functions

TreeNode* parseE() {
  auto a = parseT();
  if (!a) return NULL;
  while (true) {
    if (
      nextToken.type == TokenType::M_Symbol &&
      (nextToken.value[0] == '+' | nextToken.value[0] == '-')
    )
    {
      auto tempToken = nextToken;
      scanToken();
      auto b = parseT();
      if (!b) return NULL;
      a = new TreeNode(tempToken, a, b);
      if (!a) return NULL;
    } else { return a; }
  }
}

TreeNode* parseT() {
  auto a = parseF();
  if (!a) return NULL;
  while (true) {
    if (
      nextToken.type == TokenType::M_Symbol &&
      (nextToken.value[0] == '*' | nextToken.value[0] == '/')
    )
    {
      Token currentToken = nextToken;
      scanToken();
      auto b = parseF();
      if (!b) return NULL;
      a = new TreeNode(currentToken, a, b);
      if (!a) { return NULL; }
    }
    else {
      return a;
    }
  }
}

TreeNode* parseF() {
  if (nextToken.type == TokenType::M_Int | nextToken.type == TokenType::M_Float) {
    Token tempToken = nextToken;
    scanToken();
    return new TreeNode(tempToken, NULL, NULL);
  }
  if (nextToken.type == TokenType::M_Symbol && nextToken.value[0] == '(') {
    scanToken();
    auto a = parseE();
    if (a == NULL) { return NULL; }
    if (nextToken.type == TokenType::M_Symbol && nextToken.value[0] == ')') {
      scanToken();
      return a;
    } else { return NULL; }
  }
  if (nextToken.type == TokenType::M_Symbol && nextToken.value[0] == '-') {
    auto tempToken = nextToken;
    scanToken();
    return new TreeNode(tempToken, parseF(), NULL);
  }
  // add the bit to do function after implementing parseFunction
  return parseFunction();
}

TreeNode* parseFunction() {
  auto tempToken = nextToken;
  scanToken();
  if (nextToken.type == TokenType::M_Symbol && nextToken.value[0] == '(' ) {
    scanToken();
    auto fc = new TreeNode(tempToken, NULL, NULL);
    auto a = parseE();
    if (!a) return NULL;
    fc->pars[0] = a;
    size_t i = 1;
    while (nextToken.value[0] == ',' and i <= MAX_PARAMETERS) {
      scanToken();
      a = parseE();
      if (!a) return NULL;
      fc->pars[i] = a;
      i++;
    }
    if (nextToken.type == TokenType::M_Symbol && nextToken.value[0] == ')' ) {
      scanToken();
      return fc;
    }

  } else {
    std::cout << "soemthing Wrong went with the function\n";
    return NULL;
  }
  return NULL;
}

// This function does return int just for error handling
// and doesn't take any paramters just changes the state of some global variables
int scanToken() {
  char ch;
  char token[MAX_TOKEN_SIZE];
  size_t tokenIndex = 0;
  nextToken.func = Functions::Empty;
  while (nextIndex < lengthExp) {
    ch = expression[nextIndex];
    if (ch == ' ') {
      nextIndex++;
      continue;
    }
    if (ch == '(' | ch == ')' | ch == '+' | ch == '-' | ch == '*' | ch == '/' | ch == ',') {
      copyStr(&ch, &nextToken.value[0]);
      nextToken.type = TokenType::M_Symbol;
      nextIndex++;
      return 0;
    }
    if (isdigit(ch)) {
      while (
        nextIndex < lengthExp &&
        (isdigit(expression[nextIndex]) | (expression[nextIndex] == '.'))
        )
      {
        token[tokenIndex] = expression[nextIndex];
        tokenIndex++;
        nextIndex++;
      }
      token[tokenIndex] = '\0';
      copyStr(&token[0], &nextToken.value[0]);
      if (strChr(&nextToken.value[0], '.')) {
        nextToken.type = TokenType::M_Float;
      } else {
        nextToken.type = TokenType::M_Int;
      }
      return 0;
    }
    if (isalpha(ch)) {
      while (
        nextIndex < lengthExp &&
        (isalpha(expression[nextIndex]) | isdigit(expression[nextIndex]) | (expression[nextIndex] == '_'))
        )
      {
        token[tokenIndex] = expression[nextIndex];
        tokenIndex++;
        nextIndex++;
      }
      token[tokenIndex] = '\0';
      copyStr(&token[0], &nextToken.value[0]);
      nextToken.type = TokenType::M_FUNC;
      if (isEqStr("sqrt", &nextToken.value[0])) { nextToken.func = Functions::Sqrt; }
      if (isEqStr("pow", &nextToken.value[0])) { nextToken.func = Functions::Pow; }
      if (isEqStr("sin", &nextToken.value[0])) { nextToken.func = Functions::Sin; }
      if (isEqStr("cos", &nextToken.value[0])) { nextToken.func = Functions::Cos; }
      if (isEqStr("tan", &nextToken.value[0])) { nextToken.func = Functions::Tan; }
      if (isEqStr("asin", &nextToken.value[0])) { nextToken.func = Functions::ASin; }
      if (isEqStr("acos", &nextToken.value[0])) { nextToken.func = Functions::ACos; }
      if (isEqStr("atan", &nextToken.value[0])) { nextToken.func = Functions::ATan; }
      if (isEqStr("exp", &nextToken.value[0])) { nextToken.func = Functions::Exp; }
      if (isEqStr("ln", &nextToken.value[0])) { nextToken.func = Functions::Ln; }
      if (isEqStr("log10", &nextToken.value[0])) { nextToken.func = Functions::Log10; }
      if (isEqStr("log2", &nextToken.value[0])) { nextToken.func = Functions::Log2; }
      return 0;
    }
    // syntax not valid return 1 to signal error
    return 1;
  }
  return 2;
}

void lexer() {
  std::cout << "############## BEGIN LEXER #########################" << std::endl;
  int r_value = scanToken();
  while (r_value == 0) {
    std::cout << nextToken.value << std::endl;
    r_value = scanToken();
  }
  std::cout << "############## END LEXER #########################" << std::endl;
}

TreeNode* parse(const char* exp) {
  const char* math_expression = exp;
  copyStr(math_expression, &expression[0]);
  lengthExp = getLen(expression);
  nextIndex = 0;
  scanToken();
  auto resultTree = parseE();
  return resultTree;
}

