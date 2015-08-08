#include "Timer.h"

bool Timer::initialize()
{
	mNumFrames = 0;
	mElapsedTime = 0;
	mDt = 1.0f;
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0f / (float)countsPerSec;
	if (mSecondsPerCount)
		return 1;
	return 0;
}
void Timer::begin()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mA);
}
void Timer::end(const bool &stats, const float &timeStep)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mB);
	mDt = (mB - mA) * mSecondsPerCount;
	if (stats) {
		mNumFrames++;
		computeStats(timeStep);
	}
}
void Timer::computeStats(const float &timeStep)
{
	mElapsedTime += mDt;
	if (mElapsedTime >= timeStep) {
		mFPS = mNumFrames;
		mNumFrames = 0.0f;
		mElapsedTime = 0.0f;
		mMsPerFrame = 1000.0f / mFPS;
	}
}