//
// PauseState.cpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 08.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#include "PauseState.hpp"
#include "GameContext.hpp"
#include "Game.hpp"

namespace ge
{

PauseState::PauseState(GameContext& context) : State(context)
{

}

bool PauseState::update(const float)
{
  if (Game::isKeyJustPressed(Key::Pause))
    m_context.StateStack->requestStateChange(StateChange::Pop);
  return false;
}

void PauseState::draw()
{

}

} // namespace ge
