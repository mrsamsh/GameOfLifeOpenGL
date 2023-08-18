//
// main.cpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#include "Game.hpp"

int main(int argc, char *argv[])
{
  ge::Game game("test", 1440, 900, 1, 1);
  return game.run();
}

