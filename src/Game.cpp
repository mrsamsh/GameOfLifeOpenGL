//
// Game.cpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#include "Game.hpp"
#include "Log.hpp"
#include "Render.hpp"
#include "Vector.hpp"
#include "Simulator.hpp"
#include "PauseState.hpp"

#include <chrono>
#include <thread>
#include <ctime>

namespace ge
{

static KeyState keyState[2];

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  static bool just = true;
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && just)
  {
    just = false;
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
  {
    just = true;
  }
}

Game::Game(std::string_view title, int width, int height, int tile_w, int tile_h)
: m_window(nullptr)
, m_size(width, height)
, m_tileSize(tile_w, tile_h)
, m_title(title)
{

}

void Game::init()
{
  m_stack.registerState<Simulator>(StateId::Simulator);
  m_stack.registerState<PauseState>(StateId::PauseState);
  m_stack.requestStateChange(StateChange::Push, StateId::Simulator);
}

int Game::run()
{
  int side;
  std::cout << "side: ";
  std::cin >> side;
  m_tileSize.w = m_tileSize.h = side;

  bool rc = Render::Init(&m_window, m_title, m_size, m_tileSize);
  Assert(m_window != nullptr, "SHOT");
  glfwGetFramebufferSize(m_window, &m_size.w, &m_size.h);
  glfwSetKeyCallback(m_window, keyCallBack);
  Assert(rc, "Unable to initialize game. Quitting.");

  init();
  m_stack.applyAndInitBack<Simulator>(ivec2{m_size.w / side, m_size.h / side}, side);

  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  double lastTime, thisTime, accumulator = 0;
  float delta = 1.f / 120.f;
  int frames = 0;
  double start = glfwGetTime();
  int ticks = 0;
  while(!glfwWindowShouldClose(m_window))
  {
    lastTime = glfwGetTime();
    update(delta);
    thisTime = glfwGetTime();
    while (thisTime - lastTime < delta)
    {
      thisTime = glfwGetTime();
    }
    accumulator += delta;
    frames++;
    ticks++;
    if (accumulator >= 1.f)
    {
      accumulator -= 1;
      char title[20];
      snprintf(title, 20, "%s fps: %d",m_title.c_str(), frames);
      glfwSetWindowTitle(m_window, title);
      frames = 0;
    }
  }

  double totalTime = glfwGetTime() - start;
  Printf("Total ticks: %d, average tick time: %f", ticks,
      totalTime / (double)ticks);

  return 0;
}

void Game::update(const float delta)
{
  keyState[0].previous = keyState[0].current;
  keyState[1].previous = keyState[1].current;
  keyState[0].current = glfwGetKey(m_window, GLFW_KEY_SPACE);
  keyState[1].current = glfwGetKey(m_window, GLFW_KEY_R);
  m_stack.update(delta);
  Render::begin();
  m_stack.draw();
  Render::end();
  glfwPollEvents();
}

Game::~Game()
{
  Render::Quit();
}

bool Game::isKeyJustPressed(Key code)
{
  return keyState[static_cast<int>(code)].current 
     && !keyState[static_cast<int>(code)].previous;
}

} // namespace ge
