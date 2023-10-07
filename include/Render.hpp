//
// Render.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef RENDER_HPP_
#define RENDER_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vector.hpp"

namespace ge
{

class Render
{
public:
  static bool Init(GLFWwindow** window, const std::string& title, ivec2 size, ivec2 tileSize);
  static void Quit();
  static void fillRect(vec2 pos, vec4 color);

private:
  static void begin();
  static void end();

private:
  friend class Game;
};

} // namespace ge

#endif // RENDER_HPP_
