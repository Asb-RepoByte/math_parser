#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "recur.h"

int valid = 0;
unsigned int c_out_range = 0;
unsigned int c_invalid = 0;

int check_error(int err, char* messErr) {
  if (err) {
    std::cout << "Error " << messErr << std::endl;
    sqlite3_free(messErr);
    return 1;
  }
  return 0;
}

int createFile(const char* file_path) {

  std::ofstream outFile(file_path);
  if (!outFile) {
    std::cerr << "Error opening file " << std::endl;
    outFile.close();
    return 1;
  }
  outFile.close();
  return 0;
}

int log_file(const char* file_path, const char* data) {
  std::ofstream outFile(file_path, std::ios::app);
  if (!outFile) {
    std::cerr << "Error opening file " << std::endl;
    outFile.close();
    return 1;
  }
  outFile << data << std::endl;
  outFile.close();
  return 0;
}

bool areClose(double a, double b, double tolerance = 1e-4) {
    return std::fabs(a - b) < tolerance * std::fmax(std::fabs(a), std::fabs(b));
}

static int callback(void* data, int argc, char** argv, char** colName) {
  // i don't why i do this
  //fprintf(stderr, "%s:", (const char*)data);

  const char* id = argv[0];
  const char* expression = argv[1];
  //std::cout << id << ": " << expression << std::endl;
  //std::cout << "res: " << argv[2] << std::endl;
  double result{};
  try {
    result = std::stod(argv[2]);
  } catch (std::invalid_argument e) {
    c_invalid++;
    //std::cout << e.what() << std::endl;
  } catch (std::out_of_range e) {
    c_out_range++;
    //std::cout << e.what() << std::endl;
  }
  auto result_tree = parse(expression);
  auto my_result = result_tree->eval();
  if (areClose(my_result, result) || my_result == result) {
    valid++;
  } else {
    std::string messLog = "got [" + std::to_string(result_tree->eval()) + "] expected [" + std::to_string(result) + "] exp: " + expression;
    log_file("./cpp_log.txt", messLog.c_str());
  }
  delete result_tree;
  return 0;
}

int main(int argc, char** argv) {
  // initiliazing
  sqlite3* DB;
  char* messageError;
  int exit = sqlite3_open("./math_expressions.db", &DB);
  createFile("cpp_log.txt");

  if (check_error(exit, NULL)) { return 1; }
  std::string s_size = argv[1];
  int size = std::stoi(argv[1]);

  std::string querry = "SELECT * FROM data LIMIT " + s_size + ";";
  exit = sqlite3_exec(DB, querry.c_str(), callback, NULL, NULL);
  if (check_error(exit, NULL)) { return 1; }

  // cleaning
  sqlite3_close(DB);

  std::cout << size << " expression where tested: " << std::endl;
  std::cout << valid << " where valid." << std::endl;
  std::cout << size - valid << " where false." << std::endl;
  std::cout << c_invalid << " where invalid." << std::endl;
  std::cout << c_out_range << " where out of range." << std::endl;
  std::cout << "the taux of success is: " << (float)valid / (float)size * 100 << "%" << std::endl;
  return 0;
}
