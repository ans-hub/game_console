// *************************************************************
// File:    fx_colors.h
// Descr:   helpers color functions 
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GL_FXCOLORS_H
#define GL_FXCOLORS_H

#include <iostream>
#include <string>
#include <map>
#include <limits>
#include <type_traits>
#include <cmath>

#include "lib/render/gl_aliases.h"
#include "lib/render/exceptions.h"

#include "lib/math/math.h"

namespace anshub {

template<class T = uint>
struct Color
{
  static_assert(
    (std::is_floating_point<T>::value) || 
    (std::is_unsigned<T>::value && std::is_integral<T>::value),
    "Color<T> - T should be float or unsigned int"
  );
  constexpr Color() noexcept;
  constexpr Color(T r, T g, T b) noexcept;
  constexpr Color(T r, T g, T b, float a) noexcept;
  constexpr explicit Color(uint) noexcept;

  uint GetARGB() const;
  void Modulate(const Color&);
  void Clamp();

  bool   operator==(const Color<T>& rhs) const;
  bool   operator!=(const Color<T>& rhs) const;
  Color& operator/=(T scalar);
  Color& operator/=(int scalar);
  Color& operator*=(T scalar);
  Color& operator*=(int scalar);
  Color& operator*=(double scalar);   // is dirty trick to overload T scalar 
  Color& operator*=(const Color& rhs);
  Color& operator-=(const Color& rhs);
  Color& operator+=(const Color& rhs);
  template<class U> friend Color<U> operator/(Color<U>, U scalar);
  template<class U> friend Color<U> operator/(Color<U>, int scalar);
  template<class U> friend Color<U> operator*(Color<U>, U scalar);
  template<class U> friend Color<U> operator*(Color<U>, int scalar);
  template<class U> friend Color<U> operator*(Color<U>, const Color<U>&);
  template<class U> friend Color<U> operator-(Color<U>, const Color<U>&);
  template<class U> friend Color<U> operator+(Color<U>, const Color<U>&);

  T r_;
  T g_;
  T b_;
  float a_;

}; // struct Color

struct ColorTable
{
  ColorTable();
  FColor operator[](const std::string&);

private:
  std::map<std::string, FColor> data_;

}; // struct ColorTable

//***************************************************************************
// Color consts
//***************************************************************************

namespace color {

  constexpr uint White  {0xffffff01};
  constexpr uint Blue   {0xff000001};
  constexpr uint Red    {0x0000ff01};
  constexpr uint Green  {0x00ff0001};
  constexpr uint Cyan   {0xffff0001};
  constexpr uint Black  {0x00000001};
  const FColor fWhite  (255.0f, 255.0f, 255.0f);
  const FColor fBlack  {0.0f, 0.0f, 0.0f};
  const FColor fYellow {255.0f, 255.0f, 0.0f};
  const FColor fRed    {255.0f, 0.0f, 0.0f};
  const FColor fGreen  {0.0f, 255.0f, 0.0f};
  const FColor fBlue   {0.0f, 0.0f, 255.0f};
  const FColor fCyan   {0.0f, 255.0f, 255.0f};
  const FColor fOceanBlue {143.0f, 175.0f, 201.0f};
  const FColor fDeepPink {255.0f, 20.0f, 147.0f};

//***************************************************************************
// Interface to Color helper functions
//***************************************************************************
  
  int  MakeARGB(uchar a, uchar r, uchar g, uchar b);
  void SplitARGB(int color, uchar& b, uchar& g, uchar& r, uchar& a);
  void SplitARGB(int color, uint& b, uint& g, uint& r, uint& a);
  int  IncreaseBrightness(int color, float k);
  void ShiftRight(Color<uint>&, uint cnt);
  template<class Src, class Dest> 
    Color<Dest> Convert(const Color<Src>&);
  template<class T> 
    T MakeUnreal();

  // Output functions

  template<class T>
  std::ostream& operator<<(std::ostream& oss, const Color<T>& c);

} // namespace color

//***************************************************************************
// Implementation of inline member functions
//***************************************************************************

// Default Color constructor

template<class T>
inline constexpr Color<T>::Color() noexcept
  : r_{0}
  , g_{0}
  , b_{0}
  , a_{1}
{ }

// Constructs Color with given color components (using alpha)

template<class T>
inline constexpr Color<T>::Color(T cr, T cg, T cb, float ca) noexcept
  : r_{static_cast<T>(cr)}
  , g_{static_cast<T>(cg)}
  , b_{static_cast<T>(cb)}
  , a_{static_cast<T>(ca)}
{ }

// Constructs Color with given color components (using alpha)

template<class T>
inline constexpr Color<T>::Color(T cr, T cg, T cb) noexcept
  : r_{static_cast<T>(cr)}
  , g_{static_cast<T>(cg)}
  , b_{static_cast<T>(cb)}
  , a_{1}
{ }

// Constructs Color with unsigned represent of color

template<class T>
inline constexpr Color<T>::Color(unsigned int c) noexcept
  : r_{static_cast<T>((c >> 8) & 0xff)}
  , g_{static_cast<T>((c >> 16) & 0xff)}
  , b_{static_cast<T>((c >> 24) & 0xff)}
  , a_{1}
{ }

// Returns color in uint representation

template<class T>
inline uint Color<T>::GetARGB() const
{
  return ((int)b_ << 24) | ((int)g_ << 16) | ((int)r_ << 8) | 1;
}

// Partial specialization of function above (optimized for uint)

template<>
inline uint Color<uint>::GetARGB() const
{
  return (b_ << 24) | (g_ << 16) | (r_ << 8) | 1;
}

// Modulates 2 colors

template<class T>
inline void Color<T>::Modulate(const Color<T>& rhs)
{ 
  this->r_ *= rhs.r_;
  this->g_ *= rhs.g_;
  this->b_ *= rhs.b_;
  this->r_ >>= 8;
  this->g_ >>= 8;
  this->b_ >>= 8;
}

// Modulates 2 colors (float specialization)

template<>
inline void Color<float>::Modulate(const Color<float>& rhs)
{ 
  this->r_ *= rhs.r_;
  this->g_ *= rhs.g_;
  this->b_ *= rhs.b_;
  this->r_ *= 0.00390625f;  // divide by 256.0f
  this->g_ *= 0.00390625f;
  this->b_ *= 0.00390625f;
}

// Clamps color components after addition

template<class T>
inline void Color<T>::Clamp()
{
  r_ = std::min(255.0f, r_);
  g_ = std::min(255.0f, g_);
  b_ = std::min(255.0f, b_);
}

// Other member and friend functions implementation

template<class T>
inline bool Color<T>::operator==(const Color<T>& rhs) const
{
  return (this->r_ == rhs.r_) && (this->g_ == rhs.g_) && (this->b_ == rhs.b_);
}

template<class T>
inline bool Color<T>::operator!=(const Color<T>& rhs) const
{
  return !(*this == rhs);
}

template<class T>
inline Color<T>& Color<T>::operator/=(T scalar)
{
  float mp = 1.0f / scalar;  
  this->r_ *= mp;
  this->g_ *= mp;
  this->b_ *= mp;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator/=(int scalar)
{
  float mp = 1.0f / scalar;  
  this->r_ *= mp;
  this->g_ *= mp;
  this->b_ *= mp;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator*=(T scalar)
{
  this->r_ *= scalar;
  this->g_ *= scalar;
  this->b_ *= scalar;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator*=(int scalar)
{
  this->r_ *= scalar;
  this->g_ *= scalar;
  this->b_ *= scalar;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator*=(double scalar)
{
  this->r_ *= scalar;
  this->g_ *= scalar;
  this->b_ *= scalar;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator*=(const Color& rhs)
{
  this->r_ *= rhs.r_;
  this->g_ *= rhs.g_;
  this->b_ *= rhs.b_;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator-=(const Color& rhs)
{
  this->r_ -= rhs.r_;
  this->g_ -= rhs.g_;
  this->b_ -= rhs.b_;
  return *this;
}

template<class T>
inline Color<T>& Color<T>::operator+=(const Color& rhs)
{
  this->r_ += rhs.r_;
  this->g_ += rhs.g_;
  this->b_ += rhs.b_;
  return *this;
}

template<class U>
inline Color<U> operator/(Color<U> lhs, U scalar)
{
  lhs /= scalar;
  return lhs;
}

template<class U>
inline Color<U> operator/(Color<U> lhs, int scalar)
{
  lhs /= scalar;
  return lhs;
}

template<class U>
inline Color<U> operator*(Color<U> lhs, U scalar)
{
  lhs *= scalar;
  return lhs;
}

template<class U>
inline Color<U> operator*(Color<U> lhs, int scalar)
{
  lhs *= scalar;
  return lhs;
}

template<class U>
inline Color<U> operator*(Color<U> lhs, const Color<U>& rhs)
{
  lhs *= rhs;
  return lhs;
}

template<class U>
inline Color<U> operator-(Color<U> lhs, const Color<U>& rhs)
{
  lhs -= rhs;
  return lhs;
}

template<class U>
inline Color<U> operator+(Color<U> lhs, const Color<U>& rhs)
{
  lhs += rhs;
  return lhs;
}

// Split ARGB byte-ordered little-endian into r,g,b,a components

inline void color::SplitARGB(int color, uchar& b, uchar& g, uchar& r, uchar& a)
{
  b = (color >> 24) & 0xff;
  g = (color >> 16) & 0xff;
  r = (color >> 8)  & 0xff;
  a = color & 0xff;
}

// The same as above but with uint

inline void color::SplitARGB(int color, uint& b, uint& g, uint& r, uint& a)
{
  b = (color >> 24) & 0xff;
  g = (color >> 16) & 0xff;
  r = (color >> 8)  & 0xff;
  a = color & 0xff;
}

// Returns color in word-order ARGB format (little-endian) where the most
// signification byte is b, and less is a

inline int color::MakeARGB(uchar a, uchar r, uchar g, uchar b)
{
  return (b << 24) | (g << 16) | (r << 8) | a;
}

// Shifts right every color component but not alpha

inline void color::ShiftRight(Color<uint>& c, uint cnt)
{
  c.r_ >>= cnt;
  c.g_ >>= cnt;
  c.b_ >>= cnt;
}

// Prints on output stream color information

template<class T>
std::ostream& operator<<(std::ostream& oss, const Color<T>& c)
{
  oss << c.r_ << ';' << c.g_ << ';' << c.b_ << ';' << c.a_;
  return oss;
}

// Converts one color type to another

template<class Src, class Dest>
Color<Dest> color::Convert(const Color<Src>& src)
{
  Color<Dest> res (
    static_cast<Dest>(src.r_),
    static_cast<Dest>(src.g_),
    static_cast<Dest>(src.b_)
  );
  return res;
}

// Makes unreal color (based on unsigned int component)

template<>
inline Color<uint> color::MakeUnreal<Color<uint>>()
{
  auto max = std::numeric_limits<uint>::max();
  return Color<uint>(max, max, max);
}

// Makes unreal color (based on float component)

template<>
inline Color<float> color::MakeUnreal<Color<float>>()
{ 
  auto max = std::numeric_limits<float>::max();
  return Color<float>(max, max, max);
}

} // namespace anshub

#endif  // GL_FXCOLORS_H