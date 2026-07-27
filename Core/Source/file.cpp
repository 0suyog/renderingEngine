#include "file.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
namespace Core {
const char *ReadTextFile(const std::string &path) {
  std::ifstream f(path.c_str());
  if (!f.is_open()) {
    return nullptr;
  }
  auto start = f.tellg();
  f.seekg(EOF);
  int size = f.tellg() - start;
  f.seekg(start);

  char *result;
  f.read(result, size);
  f.close();
  return result;
}
}; // namespace Core
