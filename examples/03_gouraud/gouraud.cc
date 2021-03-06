// *************************************************************
// File:    gouraud.cc
// Descr:   demo of gouraud triangle
// Author:  Novoselov Anton @ 2017
// *************************************************************

#include <iostream>

#include "lib/window/gl_window.h"
#include "lib/window/helpers.h"

#include "lib/system/timer.h"
#include "lib/system/fps_counter.h"
#include "lib/system/rand_toolkit.h"

#include "lib/math/vector.h"
#include "lib/math/segment.h"
#include "lib/math/trig.h"

#include "lib/render/gl_draw.h"
#include "lib/render/gl_text.h"
#include "lib/render/gl_coords.h"
#include "lib/render/gl_object.h"
#include "lib/render/cameras/gl_camera.h"

#include "lib/math/matrices/mx_rotate_eul.h"
#include "lib/math/matrices/mx_rotate_uvn.h"
#include "lib/math/matrices/mx_perspective.h"
#include "lib/math/matrices/mx_translate.h"
#include "lib/math/matrices/mx_view.h"
#include "lib/math/matrices/mx_scale.h"
#include "lib/math/matrices/mx_camera.h"

#include "lib/extras/cameraman.h"

#include "../helpers.h"

using namespace anshub;

int main()
{
  TrigTable trig {};
  rand_toolkit::start_rand();

  constexpr int kFpsWait = 1000;
  Timer timer (kFpsWait);

  constexpr int kWidth = 800;
  constexpr int kHeight = 600;
  auto pos  = io_helpers::GetXYToMiddle(kWidth, kHeight); 
  GlWindow win (pos.x, pos.y, kWidth, kHeight, "Camera"); 

  auto obj = object::Make("../00_data/objects/tri_gouraud.ply",
    trig,
    {2.0f, 2.0f, 2.0f},   // initial scale
    {0.0f, 0.0f, 7.0f},   // world pos
    {0.0f, 0.0f, 5.0f}    // initial rotate
  );
  
  float    dov     {2};
  float    fov     {90};
  Vector   cam_pos {0.0f, 0.0f, 0.0f};
  Vector   cam_dir {0.0f, 0.0f, 0.0f};
  float    near_z  {dov};
  float    far_z   {500};
  auto camman = helpers::MakeCameraman(
    fov, dov, kWidth, kHeight, cam_pos, cam_dir, near_z, far_z, trig);
  
  ScrBuffer buf (kWidth, kHeight, color::Black);
  GlText    text {win};
  Pos       mpos_prev {win.ReadMousePos()}; // to calc mouse pos between frames

  do {
    timer.Start();
    win.Clear();

    auto& cam = camman.GetCurrentCamera();
    auto kbtn = win.ReadKeyboardBtn(BtnType::KB_DOWN);
    Vector  obj_rot  {0.0f, 0.0f, 0.0f};
    helpers::HandleRotateObject(kbtn, obj_rot);

    if (kbtn == Btn::ENTER)
    {
      obj.vxs_local_[0].color_.r_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[0].color_.g_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[0].color_.b_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[1].color_.r_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[1].color_.g_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[1].color_.b_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[2].color_.r_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[2].color_.g_ = rand_toolkit::get_rand(0, 255);
      obj.vxs_local_[2].color_.b_ = rand_toolkit::get_rand(0, 255);
    }

    MatrixRotateEul   mx_rot {obj_rot, trig};
    MatrixTranslate   mx_trans {obj.world_pos_};
    MatrixPerspective mx_per {cam.dov_, cam.ar_};

    obj.SetCoords(Coords::LOCAL);
    object::ApplyMatrix(mx_rot, obj);
    object::RefreshOrientation(obj, mx_rot);
    obj.CopyCoords(Coords::LOCAL, Coords::TRANS);

    obj.SetCoords(Coords::TRANS);
    object::ApplyMatrix(mx_trans, obj);
    
    MatrixCamera      mx_cam {};
    if (cam.type_ == CamType::EULER)
    {
      MatrixTranslate   mx_cam_trans  {cam.vrp_ * (-1)};
      MatrixRotateEul   mx_cam_rot    {cam.dir_ * (-1), trig};
      mx_cam = matrix::Multiplie(mx_cam, mx_cam_trans);
      mx_cam = matrix::Multiplie(mx_cam, mx_cam_rot);
    }
    else
    {
      auto& uvn = camman.GetCamera(CamType::Uvn::type);      
      uvn.LookAt(obj.world_pos_);
      MatrixTranslate   mx_cam_trans  {cam.vrp_ * (-1)};
      MatrixRotateUvn   mx_cam_rot    {uvn.GetU(), uvn.GetV(), uvn.GetN()};
      mx_cam = matrix::Multiplie(mx_cam_trans, mx_cam_rot);
    }

    object::ResetAttributes(obj);
    object::Cull(obj, cam, mx_cam);
    object::RemoveHiddenSurfaces(obj, cam);
    
    Matrix<4,4> mx_total {};
    matrix::MakeIdentity(mx_total);
    mx_total = matrix::Multiplie(mx_total, mx_cam);
    mx_total = matrix::Multiplie(mx_total, mx_per);
    object::ApplyMatrix(mx_total, obj);

    object::Homogenous2Normal(obj);

    MatrixViewport mx_view {cam.wov_, cam.scr_w_, cam.scr_h_};
    object::ApplyMatrix(mx_view, obj);

    buf.Clear();
    draw_object::Solid(obj, buf);
    buf.SendDataToFB();

    win.Render();
    timer.Wait();    

  } while (!win.Closed());

  return 0;
}