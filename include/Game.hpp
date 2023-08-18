//
// Game.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef GAME_HPP_
#define GAME_HPP_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <string>

#include "Vector.hpp"

namespace ge
{

class Game
{
public:
  Game(std::string_view title, int width, int height, int tile_w, int tile_h);
  ~Game();

  int run();
  virtual void init();
  virtual void update(float delta);

private:
  bool internal_init();
  void internal_update(float delta);

private:
  GLFWwindow* m_window;
  std::string m_title;
  ivec2 m_size, m_tileSize;
};

} // namespace ge

#endif // GAME_HPP_
