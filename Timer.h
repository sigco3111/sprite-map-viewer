#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer
{
public:
	Timer();  // 생성자
	void SetFPS(DWORD fps);  // FPS를 설정한다.
	void Update();  // 각종 내부 변수의 값을 갱신한다.
	bool GetDrawChance();  // 그림을 그려야 하는 때인지를 반환한다.

private:
	DWORD FPS;  // Frame Per Second 값
	DWORD BeforeTime;  // 먼저 번에 설정된 시간 값
	DWORD CurrentTime; // 현재 설정된 시간 값
};

#endif _TIMER_H_