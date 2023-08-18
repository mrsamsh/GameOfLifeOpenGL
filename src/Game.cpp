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

#include <chrono>
#include <thread>
#include <ctime>

namespace ge
{

struct Cell
{
  uint32_t value = 0;
  int32_t fade = 0;
  uint32_t neighbors[8];
};

Cell* cells1;
Cell* cells2;
Cell* current;
Cell* next;
int maxCells;
ivec2 grid;

int Side = 3;
const int InitCellsRatio = 20; // %

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

Game::Game(std::string_view title, int width, int height, int tile_w, int tile_h)
: m_window(nullptr)
, m_size(width, height)
, m_tileSize(tile_w, tile_h)
, m_title(title)
{

}

int Game::run()
{
  std::cout << "side: ";
  std::cin >> Side;
  Assert(internal_init(), "Unable to initialize game. Quitting.");
  init();

  double lastTime, thisTime, accumulator = 0;
  float delta;
  int frames = 0;
  double start = glfwGetTime();
  int ticks = 0;
  while(!glfwWindowShouldClose(m_window))
  {
    lastTime = glfwGetTime();
    internal_update(delta);
    thisTime = glfwGetTime();
    delta = thisTime - lastTime;
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
  Printf("Total ticks: %d, average tick time: %f", ticks, totalTime / (double)ticks);

  return 0;
}

void Game::init()
{
  srand(time(NULL));
  grid.w = m_size.w / Side;
  grid.h = m_size.h / Side;
  int totalCell = grid.w * grid.h;
  cells1 = new Cell[grid.w * grid.h];
  cells2 = new Cell[grid.w * grid.h];
  for (int y = 0; y < grid.h; ++y)
  {
    for (int x = 0; x < grid.w; ++x)
    {
      int index = x + y * grid.w;
      cells1[index].neighbors[0] = y * grid.w + (x + 1) % grid.w;
      cells1[index].neighbors[1] = y * grid.w + (x - 1 + grid.w) % grid.w;
      cells1[index].neighbors[2] = x + ((y + 1) % grid.h) * grid.w;
      cells1[index].neighbors[3] = x + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells1[index].neighbors[4] = (x + 1) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells1[index].neighbors[5] = (x - 1 + grid.w) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells1[index].neighbors[6] = (x + 1) % grid.w + ((y + 1) % grid.h) * grid.w;
      cells1[index].neighbors[7] = (x - 1 + grid.w) % grid.w + ((y + 1) % grid.h) * grid.w;

      cells2[index].neighbors[0] = y * grid.w + (x + 1) % grid.w;
      cells2[index].neighbors[1] = y * grid.w + (x - 1 + grid.w) % grid.w;
      cells2[index].neighbors[2] = x + ((y + 1) % grid.h) * grid.w;
      cells2[index].neighbors[3] = x + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells2[index].neighbors[4] = (x + 1) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells2[index].neighbors[5] = (x - 1 + grid.w) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
      cells2[index].neighbors[6] = (x + 1) % grid.w + ((y + 1) % grid.h) * grid.w;
      cells2[index].neighbors[7] = (x - 1 + grid.w) % grid.w + ((y + 1) % grid.h) * grid.w;
    }
  }
  int initialCells = totalCell * InitCellsRatio / 100;
  for (int i = 0; i < initialCells; ++i)
  {
    int x = rand() % grid.w;
    int y = rand() % grid.h;
    if (cells1[x + y * grid.w].value == 0)
    {
      cells1[x + y * grid.w].value = 1;
    }
    else
    {
      --i;
    }
  }
  current = cells1;
  next = cells2;
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

inline void evalCell(int index)
{
  int value = current[current[index].neighbors[0]].value
        + current[current[index].neighbors[1]].value
        + current[current[index].neighbors[2]].value
        + current[current[index].neighbors[3]].value
        + current[current[index].neighbors[4]].value
        + current[current[index].neighbors[5]].value
        + current[current[index].neighbors[6]].value
        + current[current[index].neighbors[7]].value;

  if ((value < 2 || value > 3) && current[index].value)
  {
    next[index].value = 0;
    next[index].fade = 60;
  }
  else if (value == 3 && current[index].value != 1)
  {
    next[index].value = 1;
    next[index].fade = 0;
  }
  else
  {
    next[index].value = current[index].value;
    next[index].fade = MAX(current[index].fade - 1, 0);
  }
}

void updateGrid()
{
  int count = grid.w * grid.h;
  for (int i = 0; i < count; ++i)
  {
    evalCell(i);
  }
  SWAP(current, next);
}

void drawGrid()
{
  uint32_t totalQuadsDrawn = 0;
  for (int i = 0; i < grid.w * grid.h; ++i)
  {
    if (current[i].value)
    {
      float x = (i % grid.w) * Side + static_cast<float>(Side) / 2.f;
      float y = (i / grid.w) * Side + static_cast<float>(Side) / 2.f;
      Render::fillRect({x, y}, {Side - 1, Side - 1}, {1, 1, 1, 1});
      totalQuadsDrawn++;
    }
    else if (current[i].fade > 0)
    {
      float x = (i % grid.w) * Side + static_cast<float>(Side) / 2.f;
      float y = (i / grid.w) * Side + static_cast<float>(Side) / 2.f;
      Render::fillRect({x, y}, {Side - 1, Side - 1},
          {0.002 * static_cast<float>(current[i].fade),
           0.002 * static_cast<float>(current[i].fade),
           0.008 * static_cast<float>(current[i].fade),
           1}
      );
      totalQuadsDrawn++;
    }
  }
  // Print("TotalQuads: ", totalQuadsDrawn);
}

void Game::update(float delta)
{
  updateGrid();
  drawGrid();
  // std::this_thread::sleep_for(std::chrono::milliseconds(60));
}

bool Game::internal_init()
{
  bool rc = Render::Init(&m_window, m_title, m_size, m_tileSize);
  Assert(m_window != nullptr, "SHOT");

  glfwGetWindowSize(m_window, &m_size.w, &m_size.h);
  glfwSetKeyCallback(m_window, keyCallBack);
  return rc;
}

void Game::internal_update(float delta)
{
  Render::begin();
  update(delta);
  Render::end();
  glfwPollEvents();
}

Game::~Game()
{
  Render::Quit();
}

} // namespace ge
