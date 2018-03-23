// *******************************************************************
// File:    camera_operator.h
// Descr:   used to manage camera movements, rotating and other stuff
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *******************************************************************

#include "lib/window/enums.h"
#include "lib/window/base_window.h"
#include "lib/draw/gl_camera.h"
#include "lib/math/vector.h"

namespace anshub {

struct CameraOperator : public GlCamera
{
  CameraOperator(
    float fov, float dov, int scr_w, int scr_h,
    const Vector& vrp, const Vector& dir,
    float z_near, float z_far
  );
  ~CameraOperator() noexcept override { }
  
  bool IsWired() const { return wired_mode_; }
  auto GetClarity() const { return clarity_; }

  void Process(const BaseWindow&);
  void SetPrevMousePos(const Pos& pos) { prev_mouse_pos_ = pos; }
  void SetLeftButton(KbdBtn b) { move_left_ = b; }
  void SetRightButton(KbdBtn b)  { move_right_ = b; }
  void SetForwardButton(KbdBtn b) { move_forward_ = b; }
  void SetBackwardButton(KbdBtn b) { move_backward_ = b; }
  void SetUpButton(KbdBtn b) { move_up_ = b; }
  void SetDownButton(KbdBtn b) { move_down_ = b; }
  void SetZoomInButton(KbdBtn b) { zoom_in_ = b; }
  void SetZoomOutButton(KbdBtn b) { zoom_out_ = b; }
  void SetSwitchTypeButton(KbdBtn b) { switch_type_ = b; }
  void SetSwitchRollButton(KbdBtn b) { switch_roll_ = b; }
  void SetWiredModeButton(KbdBtn b) { switch_wired_ = b; }
  void SetClarity(float c) { clarity_ = c; }

private:

  KbdBtn  move_left_;
  KbdBtn  move_right_;
  KbdBtn  move_forward_;
  KbdBtn  move_backward_;
  KbdBtn  move_up_;
  KbdBtn  move_down_;
  KbdBtn  zoom_in_;
  KbdBtn  zoom_out_;
  KbdBtn  switch_type_;     // euler or uvn
  KbdBtn  switch_roll_;     // switch on/off roll mode
  KbdBtn  switch_wired_;
  bool    roll_mode_;       // if true, then cam rotate z axis, else x
  bool    wired_mode_;      // if true, then all object are draws as wired
  Pos     prev_mouse_pos_;  // used to determine where was mouse in prev frame
  float   clarity_;         // max distance between cam and object to see precisely

}; // class CameraOperator 

}  // namespace anshub