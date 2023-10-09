//
// State.hpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 07.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#ifndef STATE_HPP_
#define STATE_HPP_

namespace ge
{

class GameContext;

class State
{
public:
  explicit State(GameContext& context) : m_context{context} { }
  virtual ~State() { }

  virtual bool update(const float delta) = 0;
  virtual void draw() = 0;

protected:
  GameContext& m_context;
};

} // namespace ge

#endif // STATE_HPP_
