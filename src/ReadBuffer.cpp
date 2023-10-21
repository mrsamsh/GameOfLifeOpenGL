//
// ReadBuffer.cpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#include "ReadBuffer.hpp"

#include "Log.hpp"

#include <fstream>
#include <sstream>

namespace ge
{

std::optional<std::string> File::read(std::string_view filename)
{
  std::ifstream file(filename, std::ifstream::binary);
  if (!file.is_open())
  {
    ErrorReturn(std::nullopt, "Failed to open file %s", filename.data());
  }
  std::stringstream ss;
  ss << file.rdbuf();
  file.close();
  return ss.str();
}

} // namespace ge
