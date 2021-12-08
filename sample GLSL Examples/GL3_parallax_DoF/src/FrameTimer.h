#ifndef CLASS_FRAMETIMER
#define CLASS_FRAMETIMER

#include "Timer.h"

/// FPS timer and synchronization class
class FrameTimer: public Timer
{
	private:
		unsigned long int _nframes;
		Uint32 _printerval;
		Uint32 _max_printerval;
	public:
		FrameTimer(Uint32 max_printerval = 0);
		Uint32 AddFrame();
		void Reset();
};

#endif

