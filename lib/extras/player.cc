// *************************************************************
// File:    player.cc
// Descr:   represents player in game
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#include "player.h"

namespace anshub {

Player::Player(
  GlObject&& model,
  float player_height,
  cVector& world_pos,
  cVector& dir,
  cTrigTable& trig
)
  : Inputer()
  , GlObject(std::move(model))
  , yaw_{}
  , pitch_{}
  , roll_{}
  , trig_{trig}
{
  SetValue(ObjValue::PLAYER_HEIGHT, player_height);
  this->world_pos_ = world_pos;
  this->dir_ = dir;
}

void Player::ProcessInput(const BaseWindow& win)
{
  // Start process speed

  bool speed_up_mode = IsButtonPressed(win, Btn::SPEED_UP);
  if (speed_up_mode)
  {
    float factor = GetValue(ObjValue::SPEED_UP);
    dyn_.SpeedUp(factor);
  }
  
  // Start input
  
  if (IsButtonPressed(win, Btn::MOVE_FORWARD))
    dyn_.MoveForward(dir_);
  
  if (IsButtonPressed(win, Btn::MOVE_BACKWARD))
    dyn_.MoveBackward(dir_);

  if (IsButtonPressed(win, Btn::TURN_LEFT))
    dyn_.RotateYaw(yaw_.vel_);

  if (IsButtonPressed(win, Btn::TURN_RIGHT))
    dyn_.RotateYaw(-yaw_.vel_);

  if (IsButtonPressed(win, Btn::LOOK_UP))
    dyn_.RotatePitch(-pitch_.vel_);

  if (IsButtonPressed(win, Btn::LOOK_DOWN))
    dyn_.RotatePitch(pitch_.vel_);

  // Finish input

  dyn_.ProcessVelocity(false, GetState(ObjState::ON_GROUND));
  world_pos_ += dyn_.GetVelocity();
  dyn_.ProcessDirVelocity();
  dir_ += dyn_.GetDirVelocity();

  // End speed-up

  if (speed_up_mode)
  {
    float factor = GetValue(ObjValue::SPEED_UP);
    dyn_.SpeedDown(factor);
  }
}

void Player::ProcessMovement(const Terrain& terrain)
{
  ProcessGroundPosition(terrain);
  ProcessGroundDirection(terrain);
  ProcessPlayerOrientation();
  ProcessPlayerRotating();
}

void Player::SetGroundPosition(float ypos)
{
  auto player_height = GetValue(ObjValue::PLAYER_HEIGHT);
  world_pos_.y += dyn_.GetVelocity().y;
  if (world_pos_.y <= ypos + player_height)
  {
    world_pos_.y = ypos + player_height;
    SetState(ObjState::ON_GROUND, true);
  }
  else
    SetState(ObjState::ON_GROUND, false);
}

// Find new ground position and renew vrp position

void Player::ProcessGroundPosition(const Terrain& terrain)
{
  float ground = terrain.FindGroundPosition(world_pos_);
  SetGroundPosition(ground);
}

// Find current ground normal and apply it to player direction

void Player::ProcessGroundDirection(const Terrain& terrain)
{
  if (GetState(ObjState::ON_GROUND) == true)
  {
    auto normal = terrain.FindGroundNormal(world_pos_);
    normal.Normalize();
    
    // Process pitch acceleration

    if (!pitch_.locked_)
    {
      auto& or_z = v_orient_z_;
      or_z.Normalize();

      auto angle_x = (90.0f - vector::AngleBetween(normal, or_z, true));
      dir_.x += (angle_x / pitch_.reduce_);
      dir_.x = std::max(pitch_.low_, dir_.x);
      dir_.x = std::min(pitch_.high_, dir_.x);
    }

    // Process roll acceleration

    if (!roll_.locked_)
    {
      auto& or_x = v_orient_x_;
      or_x.Normalize();

      auto angle_z = 90.0f - vector::AngleBetween(normal, or_x, true);
      dir_.z += (angle_z / roll_.reduce_);
      dir_.z = std::max(roll_.low_, dir_.z);
      dir_.z = std::min(roll_.high_, dir_.z);
    }
  }
}

// Refresh orientation of player by reset orientation vectors to default
// and rotating them to the given direction by XYZ sequence

void Player::ProcessPlayerOrientation()
{
  auto& or_x = v_orient_x_;
  auto& or_y = v_orient_y_;
  auto& or_z = v_orient_z_;

  // Restore original orientation (always as origin)
  
  or_x = {1.0f, 0.0f, 0.0f};
  or_y = {0.0f, 1.0f, 0.0f};
  or_z = {0.0f, 0.0f, 1.0f};

  // Rotate orientation vectors by given direction in XYZ seq

  coords::RotatePitch(or_x, dir_.x, trig_);
  coords::RotateYaw(or_x, dir_.y, trig_);
  coords::RotateRoll(or_x, dir_.z, trig_);
  coords::RotatePitch(or_y, dir_.x, trig_);
  coords::RotateYaw(or_y, dir_.y, trig_);
  coords::RotateRoll(or_y, dir_.z, trig_);
  coords::RotatePitch(or_z, dir_.x, trig_);
  coords::RotateYaw(or_z, dir_.y, trig_);
  coords::RotateRoll(or_z, dir_.z, trig_);
}

// Rotate player coordinates by the XYZ (!) sequence. Note that we rotate
// not local coordinates to get right rotating sequence based on global
// player direction (dir_)

void Player::ProcessPlayerRotating()
{
  SetCoords(Coords::TRANS);
  CopyCoords(Coords::LOCAL, Coords::TRANS);

  for (auto& v : vxs_trans_)
  {
    coords::RotatePitch(v.pos_, dir_.x, trig_);
    coords::RotateYaw(v.pos_, dir_.y, trig_);
    coords::RotateRoll(v.pos_, dir_.z, trig_);
  }
}

}  // namespace anshub