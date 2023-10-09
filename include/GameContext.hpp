//
// GameContext.hpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 09.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#ifndef GAMECONTEXT_HPP_
#define GAMECONTEXT_HPP_

#include "Vector.hpp"

namespace ge
{

class StateStack;

struct GameContext
{
  StateStack* StateStack;
  ivec2 GridSize;
  int Side;
};

} // namespace ge

#endif // GAMECONTEXT_HPP_
