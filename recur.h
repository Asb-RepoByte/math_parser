#include <cctype>
#include <cmath>
#include <iostream>

#define MAX_TOKEN_SIZE 1000
#define MAX_EXP_SIZE 1000 // this the max lenght of the given math expression
#define MAX_PARAMETERS 10

enum class TokenType {
  M_Int,
  M_Float,
  M_FUNC,
  M_Symbol,
  M_Negate,
  Unknown
};

enum class Functions {
  Sqrt,
  Pow,
  Sin,
  Cos,
  Tan,
  ASin,
  ACos,
  ATan,
  Exp,
  Ln,
  Log2,
  Log10,
  Empty
};

struct Token {
  TokenType type;
  Functions func = Functions::Empty;
  char value[MAX_TOKEN_SIZE];
};

struct TreeNode {
  TreeNode(
      Token token,
      TreeNode* left,
      TreeNode* right
  );
  ~TreeNode();

  void show();
  double eval();
  // members
  TreeNode* left = NULL;
  TreeNode* right = NULL;
  TreeNode* pars[MAX_PARAMETERS];
  int i_value = 0;
  double f_value = 0;
  const char* terminal;
  Token token;
};

extern char expression[MAX_EXP_SIZE];
extern size_t lengthExp; // size of the expression because this will a null terminated string we actually need to get the lenght
extern Token nextToken;
extern size_t nextIndex;

// utility functions
size_t getLen(const char* str);
void copyStr(const char* str, char* buffer);
bool strChr(const char* str, char ch);
bool isEqStr(const char* str, char* buffer);

// tokinezier and parsing functions (the sauce)
int scanToken();
TreeNode* parseE();
TreeNode* parseT();
TreeNode* parseF();
TreeNode* parseFunction();

// usage
void lexer();
TreeNode* parse(const char* exp);
