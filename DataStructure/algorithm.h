#pragma once
#include <string>
#include <sstream>
#include <utility>
#include "vector.h"

namespace flow {
  Vector<std::string> split(const std::string& line, const std::string& delimiter) {
    Vector<std::string> tokens;
    for (std::size_t pos = 0;;) {
      std::size_t nextPos = line.find(delimiter, pos);
      tokens.push_back(line.substr(pos, nextPos - pos));
      if (nextPos == std::string::npos) {
        break;
      }
      pos = nextPos + delimiter.size();
    }
    return tokens;
  }

  std::string join(const Vector<std::string>& tokens, const std::string& separator) {
    if (tokens.empty()) {
      return "";
    }

    std::size_t sz = (tokens.size() - 1) * separator.size();
    for (const std::string& token : tokens) {
      sz += token.size();
    }

    std::string line;
    line.reserve(sz);
    for (std::size_t i = 0; i + 1 < tokens.size(); ++i) {
      line += tokens[i] + separator;
    }
    line += tokens.back();
    return line;
  }
}
