//
//  Timer.h
//  SMan
//
//  Created by Ce Zhang on 9/7/12.
//  Copyright (c) 2012 HazyResearch. All rights reserved.
//

#ifndef SMan_Timer_h
#define SMan_Timer_h

#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifdef __MACH__
#include <sys/time.h>
//clock_gettime2 is not implemented on OSX
int clock_gettime2(int /*clk_id*/, struct timespec* t) {
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv) return rv;
  t->tv_sec  = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
}

#define CLOCK_MONOTONIC 0
#endif


#include <time.h>

namespace hazy{
  namespace utils{
      
      class Timer {
      public:
        
        struct timespec _start;
        struct timespec _end;
        
        Timer(){
          #ifdef __MACH__
          clock_gettime2(CLOCK_MONOTONIC, &_start);
          #else
          clock_gettime(CLOCK_MONOTONIC, &_start);
          #endif
        }
        
        virtual ~Timer(){}
        
        inline void restart(){
#ifdef __MACH__
          clock_gettime2(CLOCK_MONOTONIC, &_start);
#else
          clock_gettime(CLOCK_MONOTONIC, &_start);
#endif
        }
        
        inline float elapsed(){
#ifdef __MACH__
          clock_gettime2(CLOCK_MONOTONIC, &_end);
#else
          clock_gettime(CLOCK_MONOTONIC, &_start);
#endif
          return (_end.tv_sec - _start.tv_sec) + (_end.tv_nsec - _start.tv_nsec) / 1000000000.0;
        }
        
        
      };
      
  }
}

#endif
