// *************************************************************
// File:    starship.h
// Descr:   starship entity
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GAME_STARSHIP_H
#define GAME_STARSHIP_H

#include <vector>

#include "config.h"
#include "lib/math/vector.h"
#include "lib/math/segment.h"
#include "lib/math/segment.h"

namespace anshub {

// Represent edge in Vertices meaning

struct Edge
{
  Edge() : v1{-1.0}, v2{-1.0} { }
  Edge(float a, float b) : v1{a}, v2{b} { }
  float v1;
  float v2;

}; // struct Edge

// Represents starship entity

struct Starship
{
  using Vertices = std::vector<Vector>;  // n points
  using Edges = std::vector<Edge>;       // two number of points
  using Rect = Segment;                  // simple left bottom and right top 

  Starship();

  Vector    pos_;
  Vector    vel_;
  int       color_;
  Vertices  vx_;            // local coords
  Edges     ed_;            // model edges (pair of indexes in vx_)
  Rect      bounding_box_;  // used for laser hit detection
  
  // States

  bool      dead_;
  bool      audible_;       // is audible for current viewpoint 
  int       attack_seq_;    // how much shots should do this ship
  int       attack_wait_;   // times between shots in sequence

}; // struct Starship

}  // namespace anshub

#endif  // GAME_STARSHIP_H