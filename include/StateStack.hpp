//
// StateStack.hpp
// GameOfLifeGEL
//
// Created by Usama Alshughry 07.10.2023.
// Copyright © 2023 Usama Alshughry. All rights reserved.
//

#ifndef STATESTACK_HPP_
#define STATESTACK_HPP_

#include "State.hpp"

#include <unordered_map>
#include <memory>
#include <vector>
#include <concepts>
#include <functional>
#include <cassert>

namespace ge
{

template <typename T>
concept A_State = std::is_base_of_v<State, T>;

enum class StateChange
{
  Push,
  Pop,
  Clear
};

enum class StateId
{
  Simulator,
  PauseState,
  None
};

class StateStack
{
public:
  StateStack() = default;
  StateStack(const StateStack&) = delete;
  StateStack operator=(const StateStack&) = delete;
  StateStack(StateStack&&) = delete;
  StateStack operator=(StateStack&&) = delete;

  void update(const float delta);
  void draw();

  void requestStateChange(StateChange change, StateId id = StateId::None);

  template <A_State T>
  void registerState(StateId id)
  {
    assert(!m_factories.contains(id));
    m_factories[id] = [](GameContext& context) {
      return new T(context);
    };
  }

  template <A_State T>
  T* getState()
  {
    for (auto& state : m_stack)
    {
      T* res = dynamic_cast<T*>(state.get());
      if (res)
      {
        return res;
      }
    }
    return nullptr;
  }

private:
  void applyPendingChanges();

private:
  using StatePtr = std::unique_ptr<State>;
  std::vector<StatePtr> m_stack;
  using PendingChange = std::pair<StateChange, StateId>;
  std::vector<PendingChange> m_pendingChanges;
  std::unordered_map<StateId, std::function<State*(GameContext&)>> m_factories;
};

} // namespace ge

#endif // STATESTACK_HPP_
