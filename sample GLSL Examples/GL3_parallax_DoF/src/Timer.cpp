#include <SDL/SDL.h>
#include "Timer.h"

Timer::Timer()
{
	Reset();
}


void Timer::Stop()
{
	_last = SDL_GetTicks();
	_interval = _last - _current;
}


void Timer::Reset()
{
	_interval = 0;
	_current = _last = SDL_GetTicks();
}
