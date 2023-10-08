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

#include <vector>
#include <array>

namespace ge
{

class Simulator : public State
{
public:
  Simulator(StateStack& stack);

  void init(const ivec2& gridSize, const int side);
  bool update(const float delta) override;
  void draw() override;

private:
  void initWithRandom();

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

  Cell* m_currentArray;
  Cell* m_nextArray;
  static constexpr int InitCellsCountPercentage = 6;
  static constexpr int FadeGrades = 60;
};

} // namespace ge

#endif // SIMULATOR_HPP_
