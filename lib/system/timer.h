// *************************************************************
// File:    timer.h
// Descr:   timer class
// Author:  Novoselov Anton @ 2017
// *************************************************************

#ifndef SYS_TIMER_H
#define SYS_TIMER_H

#include <iostream>
#include <chrono>

namespace anshub {

class Timer
{
public:
  using slong = signed long;
  
  Timer();
  explicit Timer(int frames);

  void  Wait();
  void  Start()  { start_time_ = GetCurrentClock(); }
  void  End()    { end_time_ = GetCurrentClock(); }
  slong GetStartTime() const { return start_time_; }
  slong GetEndTime() const   { return end_time_; }
  slong GetElapsed() const { return end_time_ - start_time_; }
  slong GetCurrentClock() const;
  void  SetMillisecondsToWait(int ms) { ms_wait_ = ms; }

private:
  int   ms_wait_;
  slong start_time_;
  slong end_time_;

}; // class Timer 

inline Timer::slong Timer::GetCurrentClock() const
{
  return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
     
}  // namespace anshub

#endif  // SYS_TIMER_H