//
// ReadBuffer.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef READBUFFER_HPP_
#define READBUFFER_HPP_

#include <string_view>
#include <string>

namespace ge
{

class ReadBuffer
{
public:
  ReadBuffer(std::string_view filename);
  bool isValid() const;
  const std::string& string() const;
  const char* charBuffer() const;

private:
  bool m_valid;
  std::string m_data;
};

} // namespace ge

#endif // READBUFFER_HPP_
