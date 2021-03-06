// *************************************************************
// File:    gl_window.cc
// Descr:   implementation of opengl window
// Author:  Novoselov Anton @ 2017
// *************************************************************

#include "gl_window.h"

namespace anshub {

// Constructs the GlWindow with default root window

GlWindow::GlWindow(int x, int y, int w, int h, const char* name)
  : GlWindow(Pos(x,y), Size(w,h), name)
{ }

// Constructs the GlWindow with default gl attributes

GlWindow::GlWindow(const Pos& p, const Size& sz, const char* name)
  : GlWindow(
    io_helpers::GetDefaultFBAttribs(),
    io_helpers::GetDefaultGlContextAttributes(),
    p,
    sz,
    name)
{ }

// Constructs the GlWindow with custom root window with three steps

GlWindow::GlWindow(
    const FBAttrs& fb_cfg,
    const CTAttrs& ctx_cfg,
    const Pos& p, const Size& sz, const char* name)
  : BaseWindow()
  , glxver_ { io_helpers::GetGlxVersion(disp_) }
  , fbattr_ {fb_cfg}
  , ctxattr_{ctx_cfg}
  , glxself_{0}
  , fbcfgs_ {nullptr, 0}
{
  using namespace io_helpers;

  // New style context creating

  const int k_supposed_major = 1;
  const int k_supposed_minor = 3;

  const bool valid_ver_1 = glxver_.first > k_supposed_major;
  const bool valid_ver_2 = glxver_.first == k_supposed_major &&
                           glxver_.second >= k_supposed_minor;

  if (valid_ver_1 || valid_ver_2)
  {
    fbcfgs_   = GetFBConfigs(disp_, fbattr_);
    // int best  = GetBestMSAAVisual(disp_, fbcfgs_); // see note #3 why commented
    int best  = 0;
    auto cfg  = *(fbcfgs_[best]);
    auto fbvi = ChooseGlVisual(disp_, cfg);
    swa_      = GetDefaultWinAttribs(disp_, root_, fbvi.get());
    context_  = CreateGlContext(disp_, cfg, ctxattr_);
    self_     = CreateGlWindow(disp_, root_, fbvi.get(), swa_, p.x, p.y, sz.w, sz.h);
    if (FindFlagInFBAttrs(fb_cfg, GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT))
      pbuffer_  = CreatePBuffer(disp_, sz.w, sz.h, cfg);
    else
      pbuffer_ = 0;
    if (pbuffer_)
      glXMakeContextCurrent(disp_, self_, pbuffer_, context_);
    else {
      glxself_ = glXCreateWindow(disp_, cfg, self_, NULL);
      glXMakeContextCurrent(disp_, self_, glxself_, context_);
    }
  }

  // Old style context creating

  else
  {
    auto fbvi = ChooseGlVisual(disp_);
    swa_      = GetDefaultWinAttribs(disp_, root_, fbvi.get());
    context_  = CreateGlContext(disp_, fbvi.get());
    self_     = CreateGlWindow(disp_, root_, fbvi.get(), swa_, p.x, p.y, sz.w, sz.h);
    glXMakeCurrent(disp_, self_, context_);
  }

  PrintGlInfo(std::cerr);  
  ChangeWindowName(disp_, self_, name);
  this->Show();
  this->Move(p.x, p.y);       // see note #1
  this->NotifyWhenClose();    // see note #2

  io_helpers::GetWindowDimension(disp_, self_, &width_, &height_);
}

GlWindow::~GlWindow()
{
  if (fbcfgs_.front())
    XFree(fbcfgs_.front());
  if (glxself_)
    glXDestroyWindow(disp_, glxself_);  
  if (pbuffer_)
    glXDestroyPbuffer(disp_, pbuffer_);
  glXMakeCurrent(disp_, None, NULL);
  glXDestroyContext(disp_, context_);
}

void GlWindow::VerticalSync(bool on)
{
  io_helpers::ToggleVerticalSync(disp_, self_, on);
}

void GlWindow::Clear()
{
  glClearColor(0.5, 0.5, 0.5, 0.0); // todo: set clear color outside
  glClear(GL_COLOR_BUFFER_BIT);
}

void GlWindow::Redraw()
{ 
  glXSwapBuffers(disp_, self_);
}

void GlWindow::Exposed()
{
  XGetWindowAttributes(disp_, self_, &wa_);
  glViewport(0, 0, wa_.width, wa_.height);
}

} // namespace anshub

// Note 1: (hack) some window managers ignores our hints about window
//         position while creating window. Thus we move window after show
// Note 2: it is necessary call not in base class constructor since
//         there yet not created self_ instance  
// Note 3: when we draw in msaa something wrong with colors... I suppose
//         this is depends on compatibility profile of gl