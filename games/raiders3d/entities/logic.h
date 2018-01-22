// *************************************************************
// File:    logic.h
// Descr:   main game logic
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "lib/window/gl_window.h"
#include "lib/audio/audio_out.h"
#include "lib/system/rand_toolkit.h"
#include "lib/draw/fx_colors.h"
#include "lib/math/point.h"
#include "lib/math/fx_polygons.h"
#include "lib/system/timer.h" 

#include "config.h"
#include "entities/aliases.h"
#include "entities/level.h"

namespace anshub {

class Logic
{
public:
  Logic(GlWindow&, Level&, AudioOut&);
  bool Process();

private:
  void InitCannon();
  void InitStarfield();
  void InitWarship(Starship&);

  void MoveStarfield();
  void MoveWarships();
  void AttackWarships();
  void ProcessEnemyShots();

  void ProcessExplosions();
  void PrepareExplosion(Starship&);
  void ProcessCannon(Pos&, Btn);
  void ProcessViewport(Btn);
  bool ProcessGameState(Btn);
  
  GlWindow&   win_;
  Level&      level_;
  AudioOut&   audio_;

}; // class Logic

}  // namespace anshub

#endif  // GAME_LOGIC_H