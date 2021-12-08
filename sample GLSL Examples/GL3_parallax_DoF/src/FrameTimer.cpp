#include <SDL/SDL.h>
#include "FrameTimer.h"

FrameTimer::FrameTimer(Uint32 max_printerval)
{
	Reset();	
	_max_printerval = max_printerval;
}


Uint32 FrameTimer::AddFrame()
{
	_current = SDL_GetTicks();

	_nframes++;
	if (_max_printerval != 0 &&
	    _nframes != 0 &&
	    (_current - _printerval > _max_printerval * 1000))
	{
		printf("%u frames in %d seconds = %.1f FPS\n", _nframes, _max_printerval, (float)_nframes/(float)_max_printerval);
		_nframes = 0;
		_printerval = _current;
	}

	_interval = _current - _last;
	_last = _current;

	return _interval;
}


void FrameTimer::Reset()
{
	Timer::Reset();
	_nframes = 0;
	_printerval = 0;
}
