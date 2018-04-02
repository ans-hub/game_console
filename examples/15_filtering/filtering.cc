// *************************************************************
// File:    filtering.cc
// Descr:   filtering example
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip> 

#include "lib/window/gl_window.h"
#include "lib/window/helpers.h"
#include "lib/system/timer.h"
#include "lib/system/fps_counter.h"
#include "lib/system/rand_toolkit.h"
#include "lib/math/vector.h"
#include "lib/math/segment.h"
#include "lib/math/trig.h"
#include "lib/draw/gl_render_ctx.h"
#include "lib/draw/gl_draw.h"
#include "lib/draw/gl_text.h"
#include "lib/draw/gl_lights.cc"
#include "lib/draw/gl_coords.h"
#include "lib/draw/gl_object.h"
#include "lib/draw/gl_z_buffer.h"
#include "lib/draw/gl_camera.h"
#include "lib/math/matrix_rotate_eul.h"
#include "lib/math/matrix_rotate_uvn.h"
#include "lib/math/matrix_persp.h"
#include "lib/math/matrix_trans.h"
#include "lib/math/matrix_view.h"
#include "lib/math/matrix_scale.h"
#include "lib/math/matrix_camera.h"

#include "../helpers.h"
#include "../camera_operator.h"

using namespace anshub;
using namespace helpers;

void PrintInfo(
  GlText& text, FpsCounter& fps, 
  const Vector& obj_pos, const Vector& obj_rot, 
  const Vector& cam_pos, const Vector& cam_rot,
  int nfo_culled, int nfo_hidden, const RenderContext& ctx)
{
  std::ostringstream oss {};
  
  oss << "FPS: " << fps.ReadPrev()
      << ", culled: " << nfo_culled << ", hidden: " << nfo_hidden;
  text.PrintString(60, 110, oss.str().c_str());

  oss.str("");
  oss.clear();
  oss << "Obj pos: " << obj_pos << ", cam pos: " << cam_pos;
  text.PrintString(60, 90, oss.str().c_str());

  oss.str("");
  oss.clear();
  oss << "Obj rot: " << obj_rot << ", cam rot: " << cam_rot;
  text.PrintString(60, 70, oss.str().c_str());
  
  oss.str("");
  oss.clear();
  oss << "Controls: W,S,A,D,ARROWS - move, Z-N - rotate";
  text.PrintString(60, 50, oss.str().c_str());

  oss.str("");
  oss.clear();
  oss << "Mipmap: " << ctx.is_mipmapping_
      << ", bifilter: " << ctx.is_bifiltering_ << '\n';
  text.PrintString(60, 30, oss.str().c_str());  
}

int main(int argc, const char** argv)
{
  // Hanle input filename

  const char* obj_fname {};

  if (argc != 2) {
    std::cerr << "Incorrect file names\n";
    return 1;
  }
  else {
    obj_fname = argv[1];
  }
   
  // Math processor
  
  TrigTable trig {};
  rand_toolkit::start_rand();

  // Timers

  FpsCounter fps {};
  constexpr int kFpsWait = 1000;
  Timer timer (kFpsWait);

  // Constants

  constexpr int kWidth = 800;
  constexpr int kHeight = 600;

  // Window

  auto pos  = io_helpers::GetXYToMiddle(kWidth, kHeight); 
  GlWindow win (pos.x, pos.y, kWidth, kHeight, "Camera"); 

  // Objects

  auto obj = object::Make(
    obj_fname, trig, 
    {1.5f, 1.5f, 1.5f},     // initial scale
    {0.0f, 0.0f, 10.0f},    // world pos
    {0.0f, 0.0f, 0.0f}      // initial rotate
  );

  // Camera

  float    dov     {1.0f};
  float    fov     {75.0f};
  Vector   cam_pos {3.0f, 3.45f, 3.65f};
  Vector   cam_dir {27.0f, -31.0f, 0.0f};
  float    near_z  {dov};
  float    far_z   {500};

  CameraOperator cam {
    fov, dov, kWidth, kHeight, cam_pos, cam_dir, near_z, far_z
  };
  cam.SetPrevMousePos(win.ReadMousePos());
  cam.SetLeftButton(KbdBtn::A);
  cam.SetRightButton(KbdBtn::D);
  cam.SetForwardButton(KbdBtn::W);
  cam.SetBackwardButton(KbdBtn::S);
  cam.SetUpButton(KbdBtn::R);
  cam.SetDownButton(KbdBtn::F);
  cam.SetSpeedUpButton(KbdBtn::LSHIFT);
  cam.SetZoomInButton(KbdBtn::NUM9);
  cam.SetZoomOutButton(KbdBtn::NUM0);
  cam.SetSwitchRollButton(KbdBtn::L);
  cam.SetWiredModeButton(KbdBtn::T);
  cam.SetSwitchTypeButton(KbdBtn::ENTER);
  cam.SetMoveVelocity({0.0f, 0.0f, 0.05f});
  cam.SetSpeedUpValue(3.0f);
  cam.SetFlyMode(true);

  // Prepare lights sources
 
  Lights lights {};
  lights.AddAmbient(color::fWhite, 0.3f);
  lights.AddInfinite(color::fWhite, 0.7f, {-1.0f, -2.0f, -0.9f});
  lights.AddPoint(color::fBlue, 0.6f, {0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, -1.0f});

  // Create render context

  RenderContext render_ctx(kWidth, kHeight, color::Black);
  render_ctx.is_zbuf_  = true;
  render_ctx.is_wired_ = false;
  render_ctx.is_alpha_ = false;
  render_ctx.is_bifiltering_ = true;
  render_ctx.is_mipmapping_  = true;
  render_ctx.mipmap_dist_ = 170.0f;
  render_ctx.clarity_  = cam.z_far_;

  GlText  text {win};
  Vector  obj_rot {0.0f, 0.0f, 0.0f};

  // Make triangles arrays

  auto tris_base = triangles::MakeBaseContainer(0);
  auto tris_ptrs = triangles::MakePtrsContainer(0);

  do {
    timer.Start();
    win.Clear();

    // Handle input

    Vector  obj_vel    {0.0f, 0.0f, 0.0f};
    Vector  obj_scale  {1.0f, 1.0f, 1.0f};

    cam.ProcessInput(win);
    render_ctx.is_wired_ = cam.IsWired();    
    auto kbtn = win.ReadKeyboardBtn(BtnType::KB_DOWN);
    helpers::HandlePause(kbtn, win);
    helpers::HandleObject(kbtn, obj_vel, obj_rot, obj_scale);
    obj.world_pos_ += obj_vel;

    if (win.IsKeyboardBtnPressed(KbdBtn::I))
      render_ctx.is_mipmapping_ = !render_ctx.is_mipmapping_;
    if (win.IsKeyboardBtnPressed(KbdBtn::O))
      render_ctx.is_bifiltering_ = !render_ctx.is_bifiltering_;

    // Some hand transformation

    obj.SetCoords(Coords::LOCAL);
    object::Rotate(obj, obj_rot, trig);
    obj.CopyCoords(Coords::LOCAL, Coords::TRANS);
    obj.SetCoords(Coords::TRANS);
    object::Translate(obj, obj.world_pos_);

    // Culling

    object::ResetAttributes(obj);
    auto hidden = object::RemoveHiddenSurfaces(obj, cam);

    // Light objects

    object::ComputeFaceNormals(obj);
    object::ComputeVertexNormalsV2(obj);
    light::Object(obj, lights);

    // Camera routines (go to cam coords)

    MatrixCamera mx_cam {};
    if (cam.type_ == GlCamera::Type::EULER)
    {
      MatrixTranslate   mx_cam_trans  {cam.vrp_ * (-1)};
      MatrixRotateEul   mx_cam_rot    {cam.dir_ * (-1), trig};
      mx_cam = matrix::Multiplie(mx_cam, mx_cam_trans);
      mx_cam = matrix::Multiplie(mx_cam, mx_cam_rot);
    }
    else
    {
      cam.LookAt(obj.world_pos_);
      MatrixTranslate   mx_cam_trans  {cam.vrp_ * (-1)};
      MatrixRotateUvn   mx_cam_rot    {cam.u_, cam.v_, cam.n_};
      mx_cam = matrix::Multiplie(mx_cam_trans, mx_cam_rot);
      cam.dir_ = coords::RotationMatrix2Euler(mx_cam_rot);
    }
    object::ApplyMatrix(mx_cam, obj);

    // Make triangles

    tris_base.resize(0);
    tris_ptrs.resize(0);
    triangles::AddFromObject(obj, tris_base);
    auto culled = triangles::CullAndClip(tris_base, cam);
    triangles::MakePointers(tris_base, tris_ptrs);
    triangles::SortZAvg(tris_ptrs);
    
    // Finally
    
    triangles::Camera2Persp(tris_base, cam);
    triangles::Persp2Screen(tris_base, cam);

    // Draw

    render::Context(tris_ptrs, render_ctx);
    
    // Print fps and other info
    
    PrintInfo(
      text, fps, obj.world_pos_, obj_rot,
      cam.vrp_, cam.dir_, culled, hidden, render_ctx
    );
    fps.Count();

    win.Render();
    timer.Wait();

  } while (!win.Closed());

  return 0;
}