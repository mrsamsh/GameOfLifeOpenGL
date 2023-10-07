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

#define EVAL_NEW

namespace ge
{

struct Cell
{
  uint32_t value = 0;
  int32_t fade = 0;
#ifndef EVAL_NEW
  uint32_t neighbors[8];
#endif
};

std::vector<Cell> cells1;
std::vector<Cell> cells2;
Cell* current;
Cell* next;
int maxCells;
ivec2 grid;
bool pause = false;

int Side = 3;
const int InitCellsRatio = 12; // %

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
    pause = !pause;
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

int Game::run()
{
  std::cout << "side: ";
  std::cin >> Side;
  m_tileSize.w = m_tileSize.h = Side;
  Assert(internal_init(), "Unable to initialize game. Quitting.");
  init();

  double lastTime, thisTime, accumulator = 0;
  float delta = 1.f / 60.f;
  int frames = 0;
  double start = glfwGetTime();
  int ticks = 0;
  while(!glfwWindowShouldClose(m_window))
  {
    lastTime = glfwGetTime();
    internal_update(delta);
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
  Printf("Total ticks: %d, average tick time: %f", ticks, totalTime / (double)ticks);

  return 0;
}

void Game::init()
{
  srand(time(NULL));
  grid.w = m_size.w / Side;
  grid.h = m_size.h / Side;
  int totalCell = grid.w * grid.h;
  cells1.resize(grid.w * grid.h);
  cells2.resize(grid.w * grid.h);
#ifndef EVAL_NEW
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
#endif
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
  current = cells1.data();
  next = cells2.data();
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

#ifndef EVAL_NEW
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
#endif

void calculateNext(std::vector<char>& res)
{
  int count = grid.w * grid.h;
  res.clear();
  res.resize(grid.w * grid.h, 0);
  int threadNum = 2;
  std::vector<std::thread> threadPool;

  auto calcN = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = x + ((y - 1 + grid.h) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcS = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = x + ((y + 1) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcE = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x + 1) % grid.w + y * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcW = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x - 1 + grid.w) % grid.w + y * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcNE = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x + 1) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcNW = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x - 1 + grid.w) % grid.w + ((y - 1 + grid.h) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcSE = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x + 1) % grid.w + ((y + 1) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  auto calcSW = [&](int i) {
    int begin = i * (grid.h / threadNum);
    int end = MIN((i + 1) * grid.h / threadNum, grid.h);
    for (int y = begin; y < end; ++y)
    {
      for (int x = 0; x < grid.w; ++x)
      {
        int ni = x + y * grid.w;
        int pi = (x - 1 + grid.w) % grid.w + ((y + 1) % grid.h) * grid.w;
        res[ni] += current[pi].value;
      }
    }
  };

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcN, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcS, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcE, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcW, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcNE, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcNW, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcSE, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

  for (int i = 0; i <= threadNum; ++i)
    threadPool.emplace_back(calcSW, i);
  for (int i = 0; i <= threadNum; ++i)
    threadPool[i].join();
  threadPool.clear();

}

void populateNext(const std::vector<char>& res)
{
  for (int i = 0; i < grid.w * grid.h; ++i)
  {
    Cell& c = next[i];
    Cell& p = current[i];
    if ((res[i] < 2 || res[i] > 3) && p.value == 1)
    {
      c.value = 0;
      c.fade = 60;
    }
    else if (res[i] == 3 && p.value != 1)
    {
      c.value = 1;
      c.fade = 0;
    }
    else
    {
      c.value = p.value;
      c.fade = MAX(p.fade - 1, 0);
    }
  }
}

#ifndef EVAL_NEW
void updateGrid()
{
  int count = grid.w * grid.h;
  for (int i = 0; i < count; ++i)
  {
    evalCell(i);
  }
}
#endif

void drawGrid()
{
  // uint32_t totalQuadsDrawn = 0;
  for (int i = 0; i < grid.w * grid.h; ++i)
  {
    if (next[i].value)
    {
      int x = (i % grid.w) * Side;
      int y = (i / grid.w) * Side;
      Render::fillRect({x, y}, {1, 1, 1, 1});
      // totalQuadsDrawn++;
    }
    else if (current[i].fade > 0)
    {
      int x = (i % grid.w) * Side;
      int y = (i / grid.w) * Side;
      Render::fillRect({x, y},
          {0.002 * static_cast<float>(current[i].fade),
           0.002 * static_cast<float>(current[i].fade),
           0.008 * static_cast<float>(current[i].fade),
           1}
      );
      // totalQuadsDrawn++;
    }
  }
  // Print("TotalQuads: ", totalQuadsDrawn);
}

void Game::update(float delta)
{
  std::vector<std::thread> threadPool;
#ifndef EVAL_NEW
  if (!pause)
  {
    threadPool.emplace_back(updateGrid);
    // updateGrid();
  }
  threadPool.emplace_back(drawGrid);
  // drawGrid();
  for (auto& t : threadPool) t.join();
#else
  std::vector<char> res;
  if (!pause)
  {
    threadPool.emplace_back(calculateNext, std::ref(res));
  }
  threadPool.emplace_back(drawGrid);
  for (auto& t : threadPool) t.join();
  if (!pause)
    populateNext(res);
#endif
  // std::this_thread::sleep_for(std::chrono::milliseconds(60));
  if (!pause)
    SWAP(current, next);
}

bool Game::internal_init()
{
  bool rc = Render::Init(&m_window, m_title, m_size, m_tileSize);
  Assert(m_window != nullptr, "SHOT");

  glfwGetFramebufferSize(m_window, &m_size.w, &m_size.h);
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
