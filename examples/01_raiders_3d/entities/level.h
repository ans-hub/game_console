// *************************************************************
// File:    level.h
// Descr:   contains game object and state
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>

#include "config.h"
#include "lib/math/vector.h"
#include "lib/math/segment.h"

#include "entities/enums.h"
#include "entities/starship.h"
#include "entities/cannon.h"
#include "entities/player.h"

namespace anshub {

struct Level
{
  using VStarships  = std::vector<Starship>;
  using VSegments   = std::vector<Segment>;
  using VStarfield  = std::vector<Vector>;
  using VVelocity   = std::vector<Vector>;
  using VExplosions = std::vector<std::pair<VSegments, VVelocity>>;
  using VEnemyShots = std::vector<std::pair<Vector, Vector>>; // pos, vel

  Level(int ships);

  VStarships    ships_;
  VStarfield    stars_;
  Cannon        cannon_;
  VExplosions   explosions_;
  VEnemyShots   enemy_shots_;
  Player        player_;
  
  GameState     state_;
  unsigned int  score_;
  unsigned int  ships_destroyed_;

}; // struct Level

}  // namespace anshub

#endif  // GAME_LEVEL_H