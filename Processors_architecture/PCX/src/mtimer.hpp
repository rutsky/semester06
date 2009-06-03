/* mtimer.hpp
 * Some timer class.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.06.2007
 */

#include <cassert>

#include <windows.h>

class Timer
{
public:
  typedef INT64 ticks_type;

protected:
  ticks_type ticks_per_sec;

public:
  Timer()
  {
    QueryPerformanceFrequency((LARGE_INTEGER *)&ticks_per_sec);
  }

  ticks_type GetTicks( ticks_type *ticks = 0 )
  {
    ticks_type now_ticks;
    BOOL state = QueryPerformanceCounter((LARGE_INTEGER *)&now_ticks);
    assert(state == TRUE);

    if (ticks)
      *ticks = now_ticks;

    return now_ticks;
  }

  // Returns number of second since some event
  double GetTime( double *time = 0 )
  {
    double time_stamp = static_cast<double>(GetTicks()) / ticks_per_sec;

    if (time != 0)
      *time = time_stamp;

    return time_stamp;
  }
};
