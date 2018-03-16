// *************************************************************
// File:    skybox.cc
// Descr:   generates skybox 
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#include "skybox.h"

namespace anshub {

// Creates GlObject that imitates skybox

Skybox::Skybox(const char* fname, cVector& pos)
  : GlObject{object::Make(fname, {1.0f, 1.0f, 1.0f}, pos)}
{
  // Invert face vertices order

  for (auto& f : faces_)
    std::swap(f.vxs_[0], f.vxs_[2]);

  // Set shading for lighting purposes

  this->shading_ = Shading::FLAT;
}

} // namespace anshub