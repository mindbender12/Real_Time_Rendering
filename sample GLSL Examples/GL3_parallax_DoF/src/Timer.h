#ifndef CLASS_TIMER
#define CLASS_TIMER

#include <SDL/SDL.h>

/// Basic timer and synchronization class
class Timer
{
	protected:
		Uint32 _current;
		Uint32 _last;
		Uint32 _interval;
	public:
		Timer();
		void Reset();
		void Stop();
		Uint32 GetInterval() {return _interval;};
};

#endif

