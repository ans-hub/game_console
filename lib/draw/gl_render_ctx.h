// *************************************************************
// File:    gl_render_ctx.h
// Descr:   struct to hold context of rendering
// Author:  Novoselov Anton @ 2018
// URL:     https://github.com/ans-hub/game_console
// *************************************************************

#ifndef GL_RENDER_CTX_H
#define GL_RENDER_CTX_H

#include "gl_buffer.h"
#include "gl_z_buffer.h"

namespace anshub {

// Represents struct to hold different information about rendering
// and used in render functions

struct RenderContext
{
  RenderContext(int w, int h, int color)
    : is_wired_{false}
    , is_alpha_{false}
    , is_zbuf_{true}
    , is_bifiltering_{false}
    , is_mipmapping_{false}
    , clarity_{}
    , mipmap_dist_{}    
    , pixels_drawn_{}
    , triangles_drawn_{}
    , mipmaps_squares_{}
    , sbuf_{w, h, color}
    , zbuf_{w, h}
  { }

  // Context members

  bool    is_wired_;
  bool    is_alpha_;
  bool    is_zbuf_;
  bool    is_bifiltering_;
  bool    is_mipmapping_;
  int     clarity_;
  int     mipmap_dist_;
  int     pixels_drawn_;
  int     triangles_drawn_;
  V_Uint  mipmaps_squares_;

  // Context entities

  Buffer    sbuf_;
  ZBuffer   zbuf_;

}; // struct RenderContext

}  // namespace anshub

#endif  // GL_RENDER_CTX_H