// *************************************************************
// File:    scene.cc
// Descr:   builds visual game scene
// Author:  Novoselov Anton @ 2017
// *************************************************************

#include "scene.h"

namespace anshub {

Scene::Scene(GlWindow& win, Level& level)
  : win_{win}
  , w_{win_.Width()}
  , h_{win_.Height()}
  , half_w_{w_ >> 1}
  , half_h_{h_ >> 1}
  , level_(level)
  , timer_{}
  , buffer_(w_, h_, cfg::kFillColor)
  , text_{win_}
  , curr_fps_{0}
  , prev_fps_{0}
  , time_passed_{0}
  , trig_table_{}
{
  timer_.Start();
}

void Scene::Build()
{
  buffer_.Clear();
  DrawStarfield();
  DrawWarships();
  DrawCannon();
  DrawWarshipsAttack();
  DrawExplosions();
  buffer_.SendDataToFB();

  PrintInfo();
  if (level_.state_ == GameState::WIN)
    PrintCentered("YOU ARE WIN! RETRY? (Y/N)");    
  else if (level_.state_ == GameState::DEAD)
    PrintCentered("YOU ARE DEAD. RETRY? (Y/N)");
  CountFPS();
}

void Scene::DrawStarfield()
{
  int   color = 0;
  float brightness = 0;
  float kColor = ((cfg::kMaxBrightness*2)-cfg::kMinBrightness) / cfg::kStarFarZ; 

  // Prepare rotation stuff

  float sin_theta = trig_table_.Sin(level_.player_.Angle());
  float cos_theta = trig_table_.Cos(level_.player_.Angle());

  // Draw each star using color in depends of distance

  for (const auto& star : level_.stars_)
  {
    int z = star.z;
    
    // Emulate stars track

    for (int i = 0; i <= level_.player_.velocity_ / cfg::kStarTrack; ++i)
    {
      float x_per = half_w_ * star.x / z;
      float y_per = half_h_ * star.y / z;

      // Rotate using viewport angle

      float x = x_per * cos_theta - y_per * sin_theta;
      float y = x_per * sin_theta + y_per * cos_theta;
    
      // Screen coordinates

      int x_scr = half_w_ + x;
      int y_scr = half_h_ - y;

      brightness = (cfg::kMaxBrightness*2) - (star.z * kColor);
      color = color::IncreaseBrightness(cfg::kStarColor, brightness);
 
      if (polygon2d::PointInside(0, 0, w_-1, h_-1, x_scr, y_scr))
        buffer_[x_scr + y_scr * w_] = color;

      // Near to kNearZ - less step => more plume

      int step = (cfg::kStarFarZ / (level_.player_.velocity_ + 1));
      z -= z / step;
      if (z <= cfg::kNearZ)
        break;
    }
  }
}

void Scene::DrawWarships()
{
  // Prepare color for draw perspective
  
  int    color = 0;
  float brightness = 0;
  float kColor = (cfg::kMaxBrightness-cfg::kMinBrightness) / cfg::kShipFarZ; 

  for (auto& ship : level_.ships_)
  {
    if (ship.dead_)
      continue;

    // This block creates unreal bounding boxes for shooting purposes

    Vector& min = ship.bounding_box_.a;
    Vector& max = ship.bounding_box_.b;
    min.x = std::numeric_limits<int>::max();
    min.y = min.x;
    max.x = std::numeric_limits<int>::min();
    max.y = max.x;
    
    // Prepare colors

    brightness = cfg::kMaxBrightness - (ship.pos_.z * kColor);
    color = color::IncreaseBrightness(cfg::kShipColor, brightness);

    // Prepare rotation stuff

    float sin_theta = trig_table_.Sin(level_.player_.Angle());
    float cos_theta = trig_table_.Cos(level_.player_.Angle());

    for (const auto& edge : ship.ed_)
    {
      // Acsonometric projection

      float x_per_1 = half_w_ *
        (ship.pos_.x + ship.vx_[edge.v1].x) / 
        (ship.pos_.z + ship.vx_[edge.v1].z);
      float y_per_1 = 
        half_h_ *
        (ship.pos_.y + ship.vx_[edge.v1].y) /
        (ship.pos_.z + ship.vx_[edge.v1].z);
      float x_per_2 = 
        half_w_ *
        (ship.pos_.x + ship.vx_[edge.v2].x) /
        (ship.pos_.z + ship.vx_[edge.v2].z);
      float y_per_2 = 
        half_h_ *
        (ship.pos_.y + ship.vx_[edge.v2].y) /
        (ship.pos_.z + ship.vx_[edge.v2].z);

      // Rotate using viewport angle

      float x_1 = x_per_1 * cos_theta - y_per_1 * sin_theta;
      float y_1 = x_per_1 * sin_theta + y_per_1 * cos_theta;
      float x_2 = x_per_2 * cos_theta - y_per_2 * sin_theta;
      float y_2 = x_per_2 * sin_theta + y_per_2 * cos_theta;
      
      // Screen coordinates

      int x_scr_1 = half_w_ + x_1;
      int y_scr_1 = half_h_ - y_1;
      int x_scr_2 = half_w_ + x_2;
      int y_scr_2 = half_h_ - y_2;
      
      if (segment2d::Clip(0, 0, w_-1, h_-1, x_scr_1, y_scr_1, x_scr_2, y_scr_2))
      {
        raster::Line(x_scr_1, y_scr_1, x_scr_2, y_scr_2, color, buffer_);
      }

      // Define new bounding boxes

      min.x = std::fmin(min.x, x_scr_1);
      min.x = std::fmin(min.x, x_scr_2);
      min.y = std::fmin(min.y, y_scr_1);
      min.y = std::fmin(min.y, y_scr_2);
      max.x = std::fmax(max.x, x_scr_1);
      max.x = std::fmax(max.x, x_scr_2);
      max.y = std::fmax(max.y, y_scr_1);
      max.y = std::fmax(max.y, y_scr_2);
    }
  }
}

void Scene::DrawWarshipsAttack()
{
  // Prepare rotation stuff

  float sin_theta = trig_table_.Sin(level_.player_.Angle());
  float cos_theta = trig_table_.Cos(level_.player_.Angle());

  for (auto& shot : level_.enemy_shots_)
  {
    auto& shot_pos = shot.first;
    auto& shot_vel = shot.second;

    // Prepare start of the segment represents laser shot

    Vector shot_start;
    shot_start.x = shot_pos.x - (shot_vel.x);
    shot_start.y = shot_pos.y - (shot_vel.y);
    shot_start.z = shot_pos.z - (shot_vel.z);
    
    float x_per_1 = half_w_ * (shot_start.x / shot_start.z);
    float y_per_1 = half_h_ * (shot_start.y / shot_start.z);
    float x_per_2 = half_w_ * (shot_pos.x / shot_pos.z);
    float y_per_2 = half_h_ * (shot_pos.y / shot_pos.z);

    // Rotate using viewport angle
    
    float x_1 = x_per_1 * cos_theta - y_per_1 * sin_theta;
    float y_1 = x_per_1 * sin_theta + y_per_1 * cos_theta;
    float x_2 = x_per_2 * cos_theta - y_per_2 * sin_theta;
    float y_2 = x_per_2 * sin_theta + y_per_2 * cos_theta;
      
    // Screen coordinates (since in virtual coords 0;0 is the center)

    int x_scr_1 = half_w_ + x_1;
    int y_scr_1 = half_h_ - y_1;
    int x_scr_2 = half_w_ + x_2;
    int y_scr_2 = half_h_ - y_2;
    
    if (segment2d::Clip(0,0, w_-1, h_-1, x_scr_1, y_scr_1, x_scr_2, y_scr_2))
    {
      float c = cfg::kEnemyShotColor;
      float kColor = (cfg::kMaxBrightness-cfg::kMinBrightness) / shot_pos.z;
      float k1 = cfg::kMaxBrightness - (shot_pos.z * kColor);
      float k2 = (cfg::kMaxBrightness - (50 * kColor) ) * 2;
      raster::Line(x_scr_1, y_scr_1, x_scr_2, y_scr_2, c, k1, k2, buffer_);
    }
  }
}

void Scene::DrawCannon()
{
  // Prepare coordinates of cross

  auto& mid = level_.cannon_.mid_;
  int half_len = cfg::kCrossLen >> 1;

  Vector l (mid.x - half_len, mid.y, 0);
  Vector r (mid.x + half_len, mid.y, 0);
  Vector t (mid.x, mid.y + half_len, 0);
  Vector b (mid.x, mid.y - half_len, 0);

  // Draw cross

  if (polygon2d::PointsInside(0, 0, w_-1, h_-1, {l, r, t, b}))
  {
    raster::Line(l.x, l.y, r.x, r.y, cfg::kAimColor, buffer_);
    raster::Line(b.x, b.y, t.x, t.y, cfg::kAimColor, buffer_);
  }
  
  // Draw laser shot

  if (level_.cannon_.shot_)
  {
    float kColor = (cfg::kMaxBrightness-cfg::kMinBrightness) / cfg::kShipFarZ;
    float k1 = cfg::kMaxBrightness - (cfg::kShipFarZ * kColor);
    float k2 = cfg::kMaxBrightness - (cfg::kNearZ * kColor);

    if (rand_toolkit::coin_toss())
      raster::Line(0, 0, mid.x, mid.y, cfg::kCannonColor, k2, k1, buffer_);
    else
      raster::Line(w_-1, 0, mid.x, mid.y, cfg::kCannonColor, k2, k1, buffer_);
  }
}

void Scene::DrawExplosions()
{
  // Prepare color stuff

  int   color = 0;
  float brightness = 0;
  float kColor = (cfg::kMaxBrightness*4-cfg::kMinBrightness) / cfg::kShipFarZ;
 
  // Prepare rotation stuff

  float sin_theta = trig_table_.Sin(level_.player_.Angle());
  float cos_theta = trig_table_.Cos(level_.player_.Angle());

  for (auto& expl : level_.explosions_)
  {
    for (const auto& edge : expl.first)
    {
      float x_per_1 = half_w_ * (edge.a.x) / (edge.a.z);
      float y_per_1 = half_h_ * (edge.a.y) / (edge.a.z);
      float x_per_2 = half_w_ * (edge.b.x) / (edge.b.z);
      float y_per_2 = half_h_ * (edge.b.y) / (edge.b.z);

      // Rotate using viewport angle
      
      float x_1 = x_per_1 * cos_theta - y_per_1 * sin_theta;
      float y_1 = x_per_1 * sin_theta + y_per_1 * cos_theta;
      float x_2 = x_per_2 * cos_theta - y_per_2 * sin_theta;
      float y_2 = x_per_2 * sin_theta + y_per_2 * cos_theta;
        
      // Screen coordinates

      int x_scr_1 = half_w_ + x_1;
      int y_scr_1 = half_h_ - y_1;
      int x_scr_2 = half_w_ + x_2;
      int y_scr_2 = half_h_ - y_2;
      
      if (segment2d::Clip(0, 0, w_-1, h_-1, x_scr_1, y_scr_1, x_scr_2, y_scr_2))
      {
        brightness = cfg::kMaxBrightness*4 - (edge.a.z * kColor);
        color = color::IncreaseBrightness(cfg::kExplColor, brightness);
        raster::Line(x_scr_1, y_scr_1, x_scr_2, y_scr_2, color, buffer_);
      }
    }
  }
}

void Scene::PrintInfo()
{
  std::ostringstream oss {};
  oss << "Velocity: " << std::setw(7) << std::left << level_.player_.velocity_;
  oss << "Life: " << std::setw(7) << std::left << level_.player_.life_; 
 
  text_.PrintString(60, 70, oss.str().c_str());

  oss.str("");
  oss.clear();
  oss << "FPS: " << std::setw(7);
  if (prev_fps_)
    oss << prev_fps_ ;

  oss << std::setw(20) << std::left << "Controls: W,S,A,L";

  text_.PrintString(60, 50, oss.str().c_str());  
}

void Scene::PrintCentered(const char* msg)
{
  std::ostringstream oss {msg};
  text_.PrintString(w_/2-oss.str().size()*7/2, h_/2, oss.str().c_str());  
}

void Scene::CountFPS()
{
  timer_.End();
  ++curr_fps_;
  time_passed_ += timer_.GetEndTime() - timer_.GetStartTime();
  if (time_passed_ >= 1000) {
    time_passed_ = 0;
    prev_fps_ = curr_fps_;
    curr_fps_ = 0;
  }
  timer_.Start(); 
}
 
} // namespace anshub