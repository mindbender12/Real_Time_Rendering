#ifndef CLASS_FRAMETIMER
#define CLASS_FRAMETIMER

#include "Timer.h"

/// FPS timer and synchronization class
class FrameTimer: public Timer
{
	private:
		/// Frame rendered
		unsigned long int _nframes;
		/// Frames rendered since last print
		unsigned long int _print_nframes;
		/// Milliseconds elapsed since the previous frame
		Uint32 _frame_interval;
		/// Number of seconds between two prints
		Uint32 _print_interval;
		/// Number of milliseconds since the last average FPS calculation
		Uint32 _last_update;
		/// Number of milliseconds between two average FPS calculation
		Uint32 _update_interval;
		/// Average frame per seconds during the interval
		float _fps;
	public:
		/// Construct a timer which updates its fps counter every 
		/// update_interval milliseconds and prints on console 
		/// every print_interval seconds
		FrameTimer(Uint32 print_interval, Uint32 update_interval = 0);
		// Add a frame to the counter and calculate elapsed time since the previous one
		void AddFrame();
		// Reset timer and counters
		void Reset();
		/// Return current frame number
		unsigned long int GetFrame() { return _nframes; }
		/// Return the interval between two subsequent calls to AddFrame()
		Uint32 GetInterval() { return _frame_interval; };
		/// Return the average FPS on the update interval
		float GetFPS() { return _fps; };
};

#endif

