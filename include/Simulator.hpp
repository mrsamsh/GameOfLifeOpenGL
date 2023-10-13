//
// Simulator.hpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 07.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#ifndef SIMULATOR_HPP_
#define SIMULATOR_HPP_

#include "State.hpp"
#include "Vector.hpp"
#include "GameContext.hpp"

#include <vector>
#include <array>

namespace ge
{

class Simulator : public State
{
public:
  Simulator(GameContext& context);

  bool update(const float delta) override;
  void draw() override;

private:
  void init(const ivec2& gridSize, const int side);
  void initWithRandom();
  void initFadeColors();

  void calculateNext(const size_t begin, const size_t end);
  void swapArrays();

  struct Cell
  {
    int value;
    int fade;
  };

  std::vector<Cell> m_cells1;
  std::vector<Cell> m_cells2;
  std::vector<int> m_calcs;

  std::vector<vec4> m_fadeColors;

  ivec2 m_gridSize;
  int m_side;
  bool m_pause;
  int m_actualFades;

  Cell* m_currentArray;
  Cell* m_nextArray;
  static constexpr int InitCellsCountPercentage = 16;
  static constexpr int FadeGrades = 60;
  static constexpr int UpdateEvery = 3;
};

} // namespace ge

#endif // SIMULATOR_HPP_
