#pragma once
#include <algorithm>
#include <iostream>

struct pair_hash 
{
	template<typename T1, typename T2>
	size_t operator() (const std::pair<T1, T2>& pair) const 
	{
		return (std::hash<T1>()(pair.first) * 0x1f1f1f1f) ^ std::hash<T2>()(pair.second);
	}
};

namespace Math
{
	inline int randi(int max)
	{
		if (max < 0)
			return -(rand() % (std::abs(max) + 1));

		return rand() % (max + 1);
	}

	inline int randf()
	{
		return (float)rand() / RAND_MAX;
	}
}

namespace Debug
{
	inline void Log(std::string str)
	{
		std::cout << str << std::endl;
	}
}
