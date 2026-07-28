#include "file.h"
#include <fstream>
#include <iostream>
#include <string>
namespace Core {
bool ReadTextFile(const std::string &path, std::string &data) {
  std::ifstream f(path.c_str());
  if (!f.is_open()) {
    return 0;
  }
  f.seekg(0, std::ios::end);
  int size = f.tellg();
  f.seekg(0);
  data.resize(size);
  f.read(data.data(), size);
  f.close();
  return 1;
}
}; // namespace Core
