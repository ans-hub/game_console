// ***********************************************************************
// File:    fx_colors.cc
// Descr:   helpers color functions 
// Author:  Novoselov Anton @ 2017
//***********************************************************************

#include "fx_colors.h"

namespace anshub {

ColorTable::ColorTable()
{
  data_["white"] = {255.0f, 255.0f, 255.0f};
  data_["black"] = {0.0f, 0.0f, 0.0f};
  data_["yellow"] = {255.0f, 255.0f, 0.0f};
  data_["blue"] = {0.0f, 0.0f, 255.0f};
  data_["cyan"] = {0.0f, 255.0f, 255.0f};
  data_["oceanblue"] = {143.0f, 175.0f, 201.0f};
  data_["deeppink"] = {255.0f, 20.0f, 147.0f};
}

FColor ColorTable::operator[](const std::string& name)
{
  return data_[name];
}

// Increase brightness of rgba color (given in word-order)
// todo: quite slow but clear

int color::IncreaseBrightness(int color, float k)
{
  unsigned int a = 0;
  unsigned int r = (int)(((color>>8)&0xff)*k);
  unsigned int g = (int)(((color>>16)&0xff)*k);
  unsigned int b = (int)(((color>>24)&0xff)*k);
  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
  return (b << 24) | (g << 16) | (r << 8) | a;
}

} // namespace anshub