//
// Game.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef GAME_HPP_
#define GAME_HPP_

#include "Vector.hpp"
#include "StateStack.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <string>

namespace ge
{

enum class Key
{
  Pause,
  Restart,
};

class Game
{
public:
  Game(std::string_view title, int width, int height, int tile_w, int tile_h);
  ~Game();

  int run();
  void init();
  void update(const float delta);

  static bool isKeyJustPressed(Key code);

private:
  GLFWwindow* m_window;
  std::string m_title;
  ivec2 m_size, m_tileSize;
  StateStack m_stack;
};

struct KeyState
{
  int keyCode;
  bool current;
  bool previous;
};

} // namespace ge

#endif // GAME_HPP_
