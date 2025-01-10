#include "recur.h"


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "no math expression passsed\n";
    return 1;
  }
  if (argc > 2) {
    std::cout << "to many argument\n";
    return 1;
  }
  const char* math_expression = argv[1];
  //const char* math_expression = "1 + cos(0)";
  copyStr(math_expression, &expression[0]);
  std::cout << expression << std::endl;
  std::cout << "Len: " << getLen(expression) << std::endl;
  //std::cout << "iseqaul: " << isEqStr(math_expression, &expression[0]) << std::endl;
  lengthExp = getLen(expression);
  lexer();
  nextIndex = 0;
  scanToken();
  auto resultTree = parseE();
  std::cout << "exp: ";
  resultTree->show();
  std::cout << std::endl;
  std::cout << "eval: " << resultTree->eval() << std::endl;
  delete resultTree;

  return 0;
}
