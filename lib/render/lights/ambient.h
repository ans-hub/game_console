// *************************************************************
// File:    ambient.h
// Descr:   represents ambient light source
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef GL_LIGHT_AMBIENT_H
#define GL_LIGHT_AMBIENT_H

#include "light_source.h"

#include "lib/math/trig.h"

#include "lib/render/gl_aliases.h"
#include "lib/render/fx_colors.h"

namespace anshub {

//****************************************************************************
// Ambient light source
//****************************************************************************

struct AmbientArgs
{
  FColor base_color_;

}; // struct IlluminateArgs

struct LightAmbient : public LightSource<AmbientArgs>
{
  LightAmbient(cFColor& color, float intense);
  ~LightAmbient() override { }
  
  void   Reset() override { }
  void   World2Camera(const GlCamera&, const TrigTable&) override { }
  FColor Illuminate() const override;

}; // struct LightAmbient

//****************************************************************************
// Implementation of member functions
//****************************************************************************

inline LightAmbient::LightAmbient(cFColor& c, float i)
  : LightSource{c, i}
{ }

inline FColor LightAmbient::Illuminate() const
{
  return (args_.base_color_ * color_ * intense_) / 256.0f;
}

}  // namespace anshub

#endif  // GL_LIGHT_AMBIENT_H