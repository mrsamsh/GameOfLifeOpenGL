//
// StateStack.cpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 07.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#include "StateStack.hpp"
#include "Game.hpp"

namespace ge
{

void StateStack::update(const float delta)
{
  for (auto state = m_stack.rbegin(); state != m_stack.rend(); ++state)
  {
    if (!(*state)->update(delta))
    {
      break;
    }
  }
  if (m_pendingChanges.size() > 0)
    applyPendingChanges();
}

void StateStack::draw()
{
  for (auto& state : m_stack)
  {
    state->draw();
  }
}

void StateStack::requestStateChange(StateChange change, StateId id)
{
  m_pendingChanges.emplace_back(change, id);
}

void StateStack::applyPendingChanges()
{
  for (auto& [change, id] : m_pendingChanges)
  {
    switch(change)
    {
    case StateChange::Push: {
      m_stack.emplace_back(m_factories[id](Game::Context));
    } break;
    case StateChange::Pop: {
      m_stack.pop_back();
    } break;
    case StateChange::Clear: {
      m_stack.clear();
    } break;
    }
  }
  m_pendingChanges.clear();
}

} // namespace ge
