// *************************************************************
// File:    player.h
// Descr:   player (viewport) entity
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "lib/math/vector.h"
#include "config.h"

namespace anshub {

struct Player
{
  Player() 
    : w_(cfg::kPlayerW)
    , h_(cfg::kPlayerH)
    , pos_(0,0,0)
    , velocity_{cfg::kStartVelocity}
    , life_{cfg::kPlayerLife}
    , alarm_on_{false}
    , curr_angle_{0}
    , offset_angle_{0}
  { } 

  float   Angle() const { return curr_angle_ + offset_angle_; }

  int     w_;             // player width
  int     h_;             // player height
  Vector  pos_;           // player pos
  int     velocity_;      // player velocity
  int     life_;          // player life
  bool    alarm_on_;      // flag means that alarm pre-destroy is on 
  float   curr_angle_;    // rotate angle
  float   offset_angle_;  // offset to rotate angle (shake imitation)

}; // struct Player

}  // namespace anshub

#endif  //  GAME_PLAYER_H