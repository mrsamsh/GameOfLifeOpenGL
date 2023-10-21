//
// ShaderProgram.cpp
// GEL
//
// Created by Usama Alshughry 26.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#include "ShaderProgram.hpp"
#include "ReadBuffer.hpp"
#include "Log.hpp"

#include <glad/glad.h>

namespace ge
{

static const char* ShadersLut(GLenum type)
{
  switch (type)
  {
    case GL_VERTEX_SHADER:
      return "vertex shader";
    case GL_FRAGMENT_SHADER:
      return "fragment shader";
    default:
      return "?";
  }
}

static uint32_t compileShader(std::string_view filename, GLenum type)
{
  auto buffer = File::read(filename);
  if (!buffer)
    ErrorReturn(-1, "Failed to create %s from %s", ShadersLut(type),
        filename.data());
  uint32_t shader = glCreateShader(type);
  const char* source = buffer->c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    glDeleteShader(shader);
    ErrorReturn(-1, "Failed to compile %s from %s: %s", ShadersLut(type),
        filename.data(), log);
  }
  return shader;
}

ShaderProgram::ShaderProgram() : m_id(-1) { }

void ShaderProgram::load(std::string_view vertFilename, std::string_view fragFilename)
{
  uint32_t vertexShader = compileShader(vertFilename, GL_VERTEX_SHADER);
  uint32_t fragmentShader = compileShader(fragFilename, GL_FRAGMENT_SHADER);
  m_id = glCreateProgram();
  glAttachShader(m_id, vertexShader);
  glAttachShader(m_id, fragmentShader);
  glLinkProgram(m_id);
  int success;
  glGetProgramiv(m_id, GL_LINK_STATUS, & success);
  if (!success)
  {
    char log[512];
    glGetProgramInfoLog(m_id, 512, NULL, log);
    std::string names(std::string(vertFilename) + " and "
        + std::string(fragFilename));
    Error("Failed to link Program from %s: %s", names.c_str(), log);
    m_id = -1;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

ShaderProgram& ShaderProgram::use()
{
  glUseProgram(m_id);
  return *this;
}

void ShaderProgram::quit()
{
  if (m_id != -1)
    glDeleteProgram(m_id);
}

uint32_t ShaderProgram::getId() const
{
  return m_id;
}

} // namespace ge
