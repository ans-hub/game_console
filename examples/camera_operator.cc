// *******************************************************************
// File:    camera_operator.cc
// Descr:   used to manage camera movements, rotating and other stuff
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *******************************************************************

#include "camera_operator.h"

namespace anshub {

CameraOperator::CameraOperator(
  float fov,
  float dov, 
  int scr_w, 
  int scr_h,
  const Vector& vrp, 
  const Vector& dir,
  float z_near, 
  float z_far
)
  : GlCamera(fov, dov, scr_w, scr_h, vrp, dir, z_near, z_far)
  , move_left_ {KbdBtn::NONE}
  , move_right_ {KbdBtn::NONE}
  , move_forward_ {KbdBtn::NONE}
  , move_backward_ {KbdBtn::NONE}
  , move_up_ {KbdBtn::NONE}
  , move_down_ {KbdBtn::NONE}
  , zoom_in_ {KbdBtn::NONE}
  , zoom_out_ {KbdBtn::NONE}
  , switch_type_ {KbdBtn::NONE}
  , switch_roll_ {KbdBtn::NONE}
  , switch_wired_ {KbdBtn::NONE}
  , roll_mode_{false}
  , wired_mode_{false}
  , prev_mouse_pos_{-1,-1}
  , clarity_{1.0f}
{ }

void CameraOperator::Process(const BaseWindow& win)
{
  // Handle movements
  
  if (win.IsKeyboardBtnPressed(move_forward_))  this->MoveForward();
  if (win.IsKeyboardBtnPressed(move_backward_))  this->MoveBackward();
  if (win.IsKeyboardBtnPressed(move_right_))  this->MoveRight();
  if (win.IsKeyboardBtnPressed(move_left_))  this->MoveLeft();
  if (win.IsKeyboardBtnPressed(move_up_))  this->MoveUp();
  if (win.IsKeyboardBtnPressed(move_down_))  this->MoveDown();
  if (win.IsKeyboardBtnPressed(zoom_in_)) this->ChangeFov(this->fov_ - 1.0f);
  if (win.IsKeyboardBtnPressed(zoom_out_)) this->ChangeFov(this->fov_ + 1.0f);

  // Handle swithching camera type

  if (win.IsKeyboardBtnPressed(switch_type_))
  {
    if (this->type_ == GlCamera::Type::EULER)
      this->SwitchType(GlCamera::Type::UVN);
    else if (this->type_ == GlCamera::Type::UVN)
      this->SwitchType(GlCamera::Type::EULER);
  }

  // Handle switching wired mode

  if (win.IsKeyboardBtnPressed(switch_wired_))
    wired_mode_ = !wired_mode_;

  // Handle roll mode

  if (win.IsKeyboardBtnPressed(switch_roll_))
    roll_mode_ = !roll_mode_;

  // Handle camera rotating

  auto mpos = win.ReadMousePos();

  if (roll_mode_)
    this->dir_.z -= (prev_mouse_pos_.x - mpos.x) / 2;
  else
    this->dir_.y -= (prev_mouse_pos_.x - mpos.x) / 2;

  this->dir_.x -= (prev_mouse_pos_.y - mpos.y) / 2;
  prev_mouse_pos_ = mpos;
}

}  // namespace anshub