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
#include <optional>

namespace ge
{

namespace File
{

std::optional<std::string> read(std::string_view filename);

} // namespace File

} // namespace ge

#endif // READBUFFER_HPP_
