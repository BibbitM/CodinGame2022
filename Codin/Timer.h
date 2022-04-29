#pragma once

#include "Utils.h"

#include <chrono>
#include <iostream>
#include <string_view>

#define MEASURE_EXECUTION_TIME 1

// https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c

class MeasureTimer
{
public:
	MeasureTimer(std::string_view scopeName)
		: start(std::chrono::high_resolution_clock::now())
		, functionName(scopeName)
	{}

	~MeasureTimer()
	{
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::cerr << functionName << ' ' << std::chrono::duration<float, std::milli>(end - start).count() << "ms" << std::endl;
	}

private:
	std::chrono::high_resolution_clock::time_point start;
	std::string_view functionName;
};

#if MEASURE_EXECUTION_TIME
#define FUNCTION_TIMER() MeasureTimer UNIQUE_NAME(functionTimer){ __FUNCTION__ }
#define SCOPE_TIMER(scopeName) MeasureTimer UNIQUE_NAME(scopeTimer){ scopeName }
#else
#define FUNCTION_TIMER() (void)(0)
#define SCOPE_TIMER() (void)(0)
#endif
