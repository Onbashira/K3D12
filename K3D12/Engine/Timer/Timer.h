#pragma once
class Timer
{
public :

private:
	
	double _secondsPerCount;

	double _deltaTime;

	__int64 _baseTime;

	__int64 _pausedTime;

	__int64 _stopTime;

	__int64 _prevTime;

	__int64 _currentTime;

	bool _stopped;

public:

	Timer();

	~Timer();

	float DeltaTime()const;

	float TotalTime()const;

	void Reset();

	void Start();

	void Stop();

	void Tick();

private:

};

