#ifndef INCLUDED_MY_UTILITY_HPP
#define INCLUDED_MY_UTILITY_HPP

#include <cstdio>
#include <string>

namespace my
{
	std::string GetThisThreadId();

	template <class... Args>
	void Log(const char* tag, const char* format, Args&&... args)
	{
		std::string tid = GetThisThreadId();
		std::string f = std::string("[") + tid + std::string("] ") + std::string("[") + tag + std::string("] ") + format;
		printf(f.c_str(), args...);
	}
}

#endif // INCLUDED_MY_UTILITY_HPP
