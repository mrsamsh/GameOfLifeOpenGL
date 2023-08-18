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

ReadBuffer::ReadBuffer(std::string_view filename)
: m_valid(false)
, m_data()
{
  std::ifstream file(filename, std::ifstream::binary);
  if (!file.is_open())
  {
    std::string name(filename);
    ErrorReturn(, "Failed to open file %s", name.c_str());
  }
  std::stringstream ss;
  ss << file.rdbuf();
  m_data = ss.str();
  file.close();
}

bool ReadBuffer::isValid() const
{
  return m_valid;
}

const std::string& ReadBuffer::string() const
{
  return m_data;
}

const char* ReadBuffer::charBuffer() const
{
  return m_data.c_str();
}

} // namespace ge
