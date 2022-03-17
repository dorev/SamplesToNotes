#include <iostream>

#define ANSI_TXT_BLK "\033[0;30m"
#define ANSI_TXT_RED "\033[0;31m"
#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_YLW "\033[0;33m"
#define ANSI_TXT_BLU "\033[0;34m"
#define ANSI_TXT_MGT "\033[0;35m"
#define ANSI_TXT_CYA "\033[0;36m"
#define ANSI_TXT_WHI "\033[0;37m"
#define ANSI_TXT_DFT "\033[0;0m"    // Console default
#define GTEST_BOX "[     cout ] "
#define COUT_GTEST std::cout << ANSI_TXT_GRN << GTEST_BOX << ANSI_TXT_DFT
#define COUT std::cout
#define ENDL ANSI_TXT_DFT << '\n'

// =================================================================================================

#include <chrono>
#include <vector>
#include <numeric>
using TimePoint = std::chrono::steady_clock::time_point;
using Nanoseconds = long long;
using Milliseconds = double;
using Clock = std::chrono::high_resolution_clock;

class Stopwatch
{
public:

	Stopwatch()
	{
		Reset();
	}

	void Start()
	{
		_start = Now();		
	}

	Milliseconds Stop()
	{
		return MillisecondDifference(_start, _stop = Now());
	}

	Milliseconds Reset()
	{
		_laps.clear();
		return Stop();
	}

	Milliseconds Lap()
	{
		Milliseconds lapTime = Stop();
		Start();
		_laps.push_back(lapTime);
		return lapTime;
	}

	Milliseconds GetLapsAverage()
	{
		return  GetLapsTotal() / static_cast<double>(_laps.size());
	}

	Milliseconds GetLapsTotal()
	{
		return std::accumulate(_laps.begin(), _laps.end(), 0.0);
	}

	const std::vector<Milliseconds>& GetLaps() const
	{
		return _laps;
	}

	static TimePoint Now()
	{
		return _clock.now();
	}

	static Milliseconds MillisecondDifference(TimePoint start, TimePoint stop)
	{
		return static_cast<double>(NanosecondDifference(start, stop)) / 1000000;
	}

	static Nanoseconds NanosecondDifference(TimePoint start, TimePoint stop)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
	}

private:
	static Clock _clock;
	TimePoint _start;
	TimePoint _stop;
	TimePoint _lap;
	std::vector<Milliseconds> _laps;
};
