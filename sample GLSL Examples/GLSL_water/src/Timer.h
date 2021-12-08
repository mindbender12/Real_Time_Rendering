#ifndef CLASS_TIMER
#define CLASS_TIMER

#include <SDL/SDL.h>

/// Basic timer and synchronization class
class Timer
{
	protected:
		/// Is the timer running?
		bool _running;
		/// Start time mark
		Uint32 _starttime;
		/// Stop time mark
		Uint32 _stoptime;
		/// Total elapsed time
		Uint32 _total;
	public:
		/// Empty constructor
		Timer() : _running(false), _starttime(0), _stoptime(0), _total(0) {};
		/// Reset the timer
		void Reset();
		/// Start the timer
		void Start();
		/// Stop the timer without resetting it
		void Stop();
		/// Resume the timer
		void Continue();
		/// Return (stop - start) or (now - start) time interval
		Uint32 GetInterval();
		/// Return total elapsed time
		Uint32 GetTotal();
		/// Return current time
		Uint32 GetNow();
};

#endif
