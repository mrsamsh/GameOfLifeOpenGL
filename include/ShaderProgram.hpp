//
// ShaderProgram.hpp
// GEL
//
// Created by Usama Alshughry 26.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef SHADERPROGRAM_HPP_
#define SHADERPROGRAM_HPP_

#include <string_view>

namespace ge
{

class ShaderProgram
{
public:
  ShaderProgram();

  void load(std::string_view vertFilename, std::string_view fragFilename);

  void quit();
  ShaderProgram& use();
  uint32_t getId() const;

private:
  uint32_t m_id;
};

} // namespace ge

#endif // SHADERPROGRAM_HPP_
