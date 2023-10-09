//
// Simulator.cpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 07.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#include "Simulator.hpp"
#include "Render.hpp"
#include "Game.hpp"

#include <random>
#include <thread>
#include <algorithm>

namespace ge
{

Simulator::Simulator(GameContext& context)
    : State{context}
    , m_pause{false}
{
  m_actualFades = FadeGrades / UpdateEvery;
  m_fadeColors.resize(m_actualFades);
  for (int i = 0; i < m_actualFades; ++i)
  {
    float t = std::sin((static_cast<float>(i) / m_actualFades) * M_PI_2);
    float tt = t * t;
    m_fadeColors[i].x = 0.1f * tt;
    m_fadeColors[i].y = 0.1f * tt;
    m_fadeColors[i].z = 0.7f * tt;
    m_fadeColors[i].w = 1.f;
  }
  init(context.GridSize, context.Side);
}

void Simulator::init(const ivec2& gridSize, const int side)
{
  m_gridSize = gridSize;
  m_side = side;
  m_cells1.resize(m_gridSize.w * m_gridSize.h);
  m_cells2.resize(m_gridSize.w * m_gridSize.h);
  initWithRandom();
  m_currentArray = m_cells1.data();
  m_nextArray = m_cells2.data();
}

void Simulator::initWithRandom()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  int initialCellsCount = m_gridSize.w * m_gridSize.h * InitCellsCountPercentage
                        / 100;
  std::uniform_int_distribution<int> rndIndex(0, m_gridSize.w * m_gridSize.h);
  for (int i = 0; i < initialCellsCount; ++i)
  {
    int candidate = rndIndex(gen);
    if (m_cells1[candidate].value == 0)
    {
      m_cells1[candidate].value = 1;
    }
    else
      --i;
  }
}

bool Simulator::update(const float delta)
{
  if (Game::isKeyJustPressed(Key::Pause))
  {
    m_context.StateStack->requestStateChange(StateChange::Push, StateId::PauseState);
  }
  if (Game::isKeyJustPressed(Key::Restart))
  {
    m_cells1.clear();
    m_cells2.clear();
    m_cells1.resize(m_gridSize.w * m_gridSize.h);
    m_cells2.resize(m_gridSize.w * m_gridSize.h);
    m_nextArray = m_cells2.data();
    m_currentArray = m_cells1.data();
    initWithRandom();
  }

  static int accum = 0;
  accum++;
  if (accum < UpdateEvery)
  {
    return false;
  }
  accum = 0;
  std::vector<std::thread> pool;
  int dividor = 8;
  size_t y_stride = m_gridSize.h / (dividor - 1);
  for (int i = 0; i < dividor; i = i + 2)
  {
    pool.emplace_back(
        &Simulator::calculateNext,
        *this,
        i * y_stride * m_gridSize.w,
        MIN((i + 1) * y_stride * m_gridSize.w, m_gridSize.w * m_gridSize.h)
        );
  }
  for (auto& t : pool) t.join();
  pool.clear();

  for (int i = 1; i < dividor; i = i + 2)
  {
    pool.emplace_back(
        &Simulator::calculateNext,
        *this,
        i * y_stride * m_gridSize.w,
        MIN((i + 1) * y_stride * m_gridSize.w, m_gridSize.w * m_gridSize.h)
        );
  }
  for (auto& t : pool) t.join();

  swapArrays();

  return false;
}

void Simulator::calculateNext(const size_t begin, const size_t end)
{
  for (size_t i = begin; i < end; ++i)
  {
    size_t x = i % m_gridSize.w;
    size_t y = i / m_gridSize.w;
    int calc =
        m_currentArray[(x + 1) % m_gridSize.w 
                + y * m_gridSize.w].value
      + m_currentArray[(x - 1 + m_gridSize.w) % m_gridSize.w
                + y * m_gridSize.w].value
      + m_currentArray[x
                + ((y + 1) % m_gridSize.h) * m_gridSize.w].value
      + m_currentArray[x
                + ((y - 1 + m_gridSize.h) % m_gridSize.h) * m_gridSize.w].value
      + m_currentArray[(x + 1) % m_gridSize.w 
                + ((y + 1) % m_gridSize.h) * m_gridSize.w].value
      + m_currentArray[(x - 1 + m_gridSize.w) % m_gridSize.w
                + ((y - 1 + m_gridSize.h) % m_gridSize.h) * m_gridSize.w].value
      + m_currentArray[(x + 1) % m_gridSize.w 
                + ((y - 1 + m_gridSize.h) % m_gridSize.h) * m_gridSize.w].value
      + m_currentArray[(x - 1 + m_gridSize.w) % m_gridSize.w
                + ((y + 1) % m_gridSize.h) * m_gridSize.w].value;

    Cell& cc = m_currentArray[i];
    Cell& nc = m_nextArray[i];
    if ((calc < 2 || calc > 3) && cc.value == 1)
    {
      nc.value = 0;
      nc.fade = m_actualFades;
    }
    else if(calc == 3 && cc.value != 1)
    {
      nc.value = 1;
      nc.fade = 0;
    }
    else
    {
      nc.value = cc.value;
      nc.fade = MAX(cc.fade - 1, 0);
    }
  }
}

void Simulator::swapArrays()
{
  SWAP(m_currentArray, m_nextArray);
}

void Simulator::draw()
{
  for (int y = 0; y < m_gridSize.h; ++y)
  {
    for (int x = 0, i = y * m_gridSize.w; x < m_gridSize.w; ++x, ++i)
    {
      if (m_nextArray[i].value)
      {
        Render::fillRect(
            { x * m_side, y * m_side },
            { 1, 1, 1, 1 }
            );
      }
      else if (m_nextArray[i].fade > 0)
      {
        Render::fillRect(
            { x * m_side, y * m_side },
            m_fadeColors[m_nextArray[i].fade - 1]
            );
      }
    }
  }
}

} // namespace ge
