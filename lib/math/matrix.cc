// *************************************************************
// File:    matrix.cc
// Descr:   represents matrix
// Author:  Novoselov Anton @ 2017
// *************************************************************

#include "matrix.h"

namespace anshub {

float matrix::Determinant(const Matrix<2,2>& mx)
{
  return (
    mx(0,0) * mx(1,1) - mx(0,1) * mx(1,0)
  );
}

float matrix::Determinant(const Matrix<3,3>& mx)
{
  return (
    mx(0,0) * (mx(1,1) * mx(2,2) - mx(2,1) * mx(1,2)) - 
    mx(0,1) * (mx(1,0) * mx(2,2) - mx(2,0) * mx(1,2)) + 
    mx(0,2) * (mx(1,0) * mx(2,1) - mx(2,0) * mx(1,1))
  );
}

// Returns invert matrix

// Result matrix given through the reference due to avoid using 
// exceptions if determinant is 0 (when inverse matrix is absent)

bool matrix::Inverse(const Matrix<2,2>& mx, Matrix<2,2>& res)
{
  float det = matrix::Determinant(mx);
  float inv = 1 / det;

  if (!math::Fzero(det))
  {
    res(0,0) =  mx(1,1) * inv;
    res(0,1) = -mx(0,1) * inv;
    res(1,0) = -mx(1,0) * inv;
    res(1,1) =  mx(0,0) * inv;
    return true;
  }
  else
    return false;
}

// The same as above but for 3x3 matrix

bool matrix::Inverse(const Matrix<3,3>& mx, Matrix<3,3>& res)
{
  float det = matrix::Determinant(mx);
  float inv = 1 / det;

  if (!math::Fzero(det))
  {
    res(0,0) = (mx(1,1) * mx(2,2) - mx(2,1) * mx(1,2)) * inv;
    res(0,1) = (mx(0,1) * mx(2,2) - mx(2,1) * mx(0,2)) * (-inv);
    res(0,2) = (mx(0,1) * mx(1,2) - mx(1,1) * mx(0,2)) * inv;

    res(1,0) = (mx(1,0) * mx(2,2) - mx(2,0) * mx(1,2)) * (-inv);
    res(1,1) = (mx(0,0) * mx(2,2) - mx(2,0) * mx(0,2)) * inv;
    res(1,2) = (mx(0,0) * mx(1,2) - mx(1,0) * mx(0,2)) * (-inv);

    res(2,0) = (mx(1,0) * mx(2,1) - mx(2,0) * mx(1,1)) * inv;
    res(2,1) = (mx(0,0) * mx(2,1) - mx(2,0) * mx(0,1)) * (-inv);
    res(2,2) = (mx(0,0) * mx(1,1) - mx(1,0) * mx(0,1)) * inv;
    return true;
  }
  else
    return false;
}

// Interpret vector as 2d vector with ficted w=1 (which real is unused z)

Vector matrix::Multiplie(const Vector& v, const Matrix<3,3>& mx)
{
  Vector res;
  res.x = v.x * mx(0,0) + v.y * mx(1,0) + v.w * mx(2,0);
  res.y = v.x * mx(0,1) + v.y * mx(1,1) + v.w * mx(2,1);
  res.w = v.x * mx(0,2) + v.y * mx(1,2) + v.w * mx(2,2);
  return res;
}

// Interpret vector as 3d vector with ficted w=1 (which is real absent)
// and translate position

Vector matrix::Multiplie(const Vector& v, const Matrix<4,4>& mx)
{
  Vector res {};
  res.x = v.x * mx(0,0) + v.y * mx(1,0) + v.z * mx(2,0) + v.w * mx(3,0);
  res.y = v.x * mx(0,1) + v.y * mx(1,1) + v.z * mx(2,1) + v.w * mx(3,1);
  res.z = v.x * mx(0,2) + v.y * mx(1,2) + v.z * mx(2,2) + v.w * mx(3,2);
  res.w = v.x * mx(0,3) + v.y * mx(1,3) + v.z * mx(2,3) + v.w * mx(3,3);
  return res;
}

Matrix<4,4> matrix::MakeRotateX(float angle)
{
  float tsin = std::sin(trig::Deg2rad(angle));
  float tcos = std::cos(trig::Deg2rad(angle));
  return {
    1.0f,   0.0f,   0.0f,   0.0f,
    0.0f,   tcos,   tsin,   0.0f, 
    0.0f,  -tsin,   tcos,   0.0f,
    0.0f,   0.0f,   0.0f,   1.0f 
  };
}

Matrix<4,4> matrix::MakeRotateY(float angle)
{
  float tsin = std::sin(trig::Deg2rad(angle));
  float tcos = std::cos(trig::Deg2rad(angle));
  return {
    tcos,   0.0f,  -tsin,   0.0f,
    0.0f,   1.0f,   0.0f,   0.0f, 
    tsin,   0.0f,   tcos,   0.0f,
    0.0f,   0.0f,   0.0f,   1.0f 
  };
}

Matrix<4,4> matrix::MakeRotateZ(float angle)
{
  float tsin = std::sin(trig::Deg2rad(angle));
  float tcos = std::cos(trig::Deg2rad(angle));
  return {
    tcos,   tsin,   0.0f,   0.0f,
    -tsin,  tcos,   0.0f,   0.0f, 
    0.0f,   0.0f,   1.0f,   0.0f,
    0.0f,   0.0f,   0.0f,   1.0f 
  };
}

} // namespace anshub