//
// PauseState.hpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 08.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#ifndef PAUSESTATE_HPP_
#define PAUSESTATE_HPP_

#include "State.hpp"

namespace ge
{

class PauseState : public State
{
public:
  PauseState(GameContext& context);

  bool update(const float) override;
  void draw() override;
};

} // namespace ge

#endif // PAUSESTATE_HPP_
