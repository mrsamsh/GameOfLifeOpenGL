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

class StateStack;

class State
{
public:
  explicit State(StateStack& stack) : m_stack{stack} { }
  virtual ~State() { }

  virtual bool update(const float delta) = 0;
  virtual void draw() = 0;

protected:
  StateStack& m_stack;
};

} // namespace ge

#endif // STATE_HPP_
