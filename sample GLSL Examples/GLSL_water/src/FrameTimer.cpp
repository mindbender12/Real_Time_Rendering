#include <iostream>
#include "FrameTimer.h"

using std::cout;
using std::endl;

FrameTimer::FrameTimer(Uint32 print_interval, Uint32 update_interval) : _nframes(0), _print_nframes(0),	_fps(0)
{
	Reset();
	_print_interval = print_interval;
	_update_interval = update_interval;
}


void FrameTimer::AddFrame()
{
	Stop();
	_frame_interval = Timer::GetInterval();

	_nframes++;
	_print_nframes++;

	// Self-implementation of partial timing
	if (_update_interval != 0 && (GetNow() - _last_update > _update_interval)) {
		_fps = float(_nframes)/(float(GetNow() - _last_update) / 1000.0f);

		_nframes = 0;
		_last_update = GetNow();
	}


	// Using partial timing of the Timer class (using GetTotal())
	if (_print_interval != 0 && _nframes != 0 && (GetTotal() > _print_interval * 1000))	{
		float _print_fps = float(_print_nframes)/float(_print_interval);
		cout << _print_nframes << " frames in " << _print_interval << " seconds = " << _print_fps << " FPS" << endl;	

		_print_nframes = 0;
		Timer::Reset();
	}

	Continue();
}


void FrameTimer::Reset()
{
	Timer::Reset();

	_nframes = 0;
	_print_nframes = 0;
	_fps = 0;

	_last_update = 0;
}
