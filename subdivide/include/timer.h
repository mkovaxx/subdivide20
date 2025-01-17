#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>
//: for timing
static clock_t __timerstart, __timerend;

//: initialize timer
inline void InitTimer( void ) { clock(); }

//: start timer
inline void StartTimer( void ) { __timerstart = clock(); }

//: end timer
inline void EndTimer( void ) { __timerend = clock(); }

//: print timerend - timerstart
inline ostream&
PrintDeltaTimer( ostream& os )
{
  os << ( 1000.0f* ( float )( __timerend - __timerstart ) / (CLOCKS_PER_SEC )) <<
    " msec";
  return os;
}


#endif /* __TIMER_H__ */

