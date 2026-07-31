#include "Timer.h"

Timer::Timer()
{
	// 멤버 변수를 초기화한다.
	FPS = 0;
	BeforeTime = GetTickCount();
	CurrentTime = GetTickCount();
}

void Timer::SetFPS(DWORD fps)
{
	FPS = fps;
}

void Timer::Update()
{
	CurrentTime = GetTickCount();
}

bool Timer::GetDrawChance()
{
	if ( CurrentTime - BeforeTime > FPS )
	{
		BeforeTime = CurrentTime;
		return true;
	}
	else
	{
		return false;
	}
}