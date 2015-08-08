#pragma once
#include <windows.h>

struct FrameStats
{
public:
	float fps;
	float msPerFrame;
};

class Timer
{
public:
	bool initialize();
	void begin();
	void end(const bool &stats = 0, const float &timeStep = 1.0f);
	inline float getDelta()
	{
		return mDt;
	}
	inline FrameStats getStats()
	{
		FrameStats fs = { mFPS, mMsPerFrame };
		return fs;
	}
private:
	void computeStats(const float &timeStep);
private:
	float mSecondsPerCount;
	__int64 mA;
	__int64 mB;
	float mDt;

	float mElapsedTime;
	float mNumFrames;
	float mFPS;
	float mMsPerFrame;
};
