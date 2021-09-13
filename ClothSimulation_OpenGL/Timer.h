#pragma once
class Timer
{
public:
	double currentTime;
	double AddValue;
	enum TimerStates
	{
		Start,
		Stop
	};
	Timer();
	void StartTimer();

	~Timer();
};

