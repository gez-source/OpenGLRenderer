#ifndef DISPLAY_MANAGER
#define DISPLAY_MANAGER

#include <chrono>

class TimeManager 
{
private:
	inline static long lastFrameTime;
	inline static float delta;
	inline static long begin;
public:
	static void Initilise() 
	{
		using namespace std::chrono;
		auto now = std::chrono::high_resolution_clock::now();
		auto now_ms = time_point_cast<milliseconds>(now);
		auto value = now_ms.time_since_epoch();
		begin = value.count();

		lastFrameTime = GetCurrentTime();


	}

	static void Update() 
	{
		long currentFrameTime = GetCurrentTime();
		delta = (currentFrameTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentFrameTime;
	}

	static float GetFrameTime() 
	{
		return delta;
	}

	static long GetCurrentTime() 
	{
		using namespace std::chrono;

		auto now = high_resolution_clock::now();
		auto now_ms = time_point_cast<milliseconds>(now);
		auto value = now_ms.time_since_epoch();
		long duration = value.count();
		long ms = duration - begin;
		return ms;
	}

};

#endif