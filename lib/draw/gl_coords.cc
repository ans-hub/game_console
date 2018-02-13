// *****************************************************************
// File:    gl_coords.h
// Descr:   functions to convert different coordinates (non-marices)
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *****************************************************************

#include "gl_coords.h"

namespace anshub {

// Convertes given vertexes into the world coordinates

Vertexes coords::Local2World(const Vertexes& vxs, const Vector& move)
{
  Vertexes res {};
  res.reserve(vxs.size());

  for (auto& vx : vxs)
  {
    float x = vx.x + move.x;
    float y = vx.y + move.y;
    float z = vx.z + move.z;
    res.emplace_back(x, y, z);
  }
  return res;
}

// Translate all vertexes into the camera coordinates

Vertexes coords::World2Camera(
  const Vertexes& vxs, Vector& cam_pos, Vector& cam_dir, TrigTable& trig)
{
  Vertexes res {};
  res.reserve(vxs.size());
  for (const auto& vx : vxs)
  {
    // Translate position

    float y = vx.y - cam_pos.y;
    float x = vx.x - cam_pos.x;
    float z = vx.z - cam_pos.z;

    // Y-axis rotate

    if (math::FNotZero(cam_dir.y))
    {
      float sine_y = trig.Sin(-cam_dir.y);
      float cosine_y = trig.Cos(-cam_dir.y);      
      x = x * cosine_y + z * sine_y;
      z = x * -sine_y  + z * cosine_y;
    }

    // X-axis rotate

    if (math::FNotZero(cam_dir.x))
    {
      float sine_x = trig.Sin(-cam_dir.x);      
      float cosine_x = trig.Cos(-cam_dir.x);  
      y = y * cosine_x - z * sine_x;
      z = y * sine_x   + z * cosine_x;
    }

    // Z-axis rotate

    if (math::FNotZero(cam_dir.z))
    {
      float sine_z = trig.Sin(-cam_dir.z);
      float cosine_z = trig.Cos(-cam_dir.z);
      x = x * cosine_z - y * sine_z;
      y = x * sine_z   + y * cosine_z;
    }
  }
  return res;
}

// Translates all vertexes from camera (world) to perspective

Vertexes coords::Camera2Persp(const Vertexes& vxs, float dov, float ar)
{
  Vertexes res {};
  res.reserve(vxs.size());

  for (const auto& vx : vxs)
  {
    float x = vx.x * dov / vx.z;
    float y = vx.y * dov * ar / vx.z;
    res.emplace_back(x, y, vx.z);
  }
  return res;
}

// Where wov - width of view

Vertexes coords::Persp2Screen(const Vertexes& vxs, float wov, int scr_w, int scr_h)
{
  // Define proportion koefficients

  float kx = scr_w / wov; // how much pixels in one unit of projection plane 
  float ky = scr_h / wov;
  
  float half_wov = wov / 2;
  Vertexes res {};

  // Convert all points from persp to screen

  res.reserve(vxs.size());
  for (const auto& vx : vxs)
  {
    res.emplace_back(
      (vx.x + half_wov) * kx,   // convert -half_wov +half_wov
      (vx.y + half_wov) * ky,   // to 0-width, 0-height
      0);
  }
  return res;
}

// Extracts eulers angles from yxz rotation matrix. As example, but in another
// matrixes (row based) - https://goo.gl/AjhkFN

Vector coords::RotationMatrix2Euler(const MatrixRotateUvn& mx)
{
  float yaw {};     // y
  float pitch {};   // x
  float roll {};    // z

  // In depends of gimbal locked or not we have several ways

  float sin_x = mx(1,2);

  // a. If gimbal not locked
  
  if (sin_x > -1.0f && sin_x < 1.0f)
  {
    pitch = std::asin(mx(1,2));             // +sin_x
    yaw   = std::atan2(-mx(0,2), mx(2,2));  // -cos_x*sin_y / cos_x*cos_y
    roll  = std::atan2(-mx(1,0), mx(1,1));  // -cos_x*sin_z / cos_x*cos_z
  }

  // b. If gimbal is locked and look up

  else if (sin_x <= -1.0f) {
    pitch = +math::kPI * 0.5f;              // +90 (if in degrees)
    yaw   = -std::atan2(mx(0,1), -mx(2,1));
    roll  = 0.0f;
  }
  
  // c. If gimbal is locked and look down

  else if (sin_x >= +1.0f) {
    pitch = -math::kPI * 0.5f;              // -90 (if in degrees)
    yaw   = +std::atan2(mx(0,1), -mx(2,1));
    roll  = 0.0f;
  }

  // Finally, convert to degrees and clamp to range 0..360 degrees

  pitch = -trig::Rad2deg(pitch);
  yaw   = -trig::Rad2deg(yaw);
  roll  = -trig::Rad2deg(roll);

  return {pitch, yaw, roll};
}

} // namespace anshub