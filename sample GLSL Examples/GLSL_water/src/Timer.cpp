#include "Timer.h"
#include <SDL/SDL.h>

void Timer::Reset()
{
	_running = false;
	_starttime = _stoptime = 0;
	_total = 0;
}


void Timer::Start()
{
	Reset();
	Continue();
}


void Timer::Stop()
{
	_running = false;
	_stoptime = SDL_GetTicks();
	_total += _stoptime - _starttime;
}


void Timer::Continue()
{
	_running = true;
	_starttime = SDL_GetTicks();
}


Uint32 Timer::GetInterval()
{
	if (_running) {
		return SDL_GetTicks() - _starttime;
	}
	else {
		return _stoptime - _starttime;
	}
}


Uint32 Timer::GetTotal()
{
	return _total;
}


Uint32 Timer::GetNow()
{
	return SDL_GetTicks();
}
