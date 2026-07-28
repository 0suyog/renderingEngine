#pragma once
#include <fstream>
#include <memory>
#include <string>
namespace Core {
bool ReadTextFile(const std::string &path, std::string &data);
} // namespace Core
