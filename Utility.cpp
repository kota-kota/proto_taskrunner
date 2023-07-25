#include "utility.hpp"
#include <sstream>
#include <thread>
#include <iomanip>

namespace my
{
	std::string GetThisThreadId()
	{
		std::ostringstream oss;
		oss.setf(std::ios::hex, std::ios::basefield);
		oss.setf(std::ios::right, std::ios::adjustfield);
		oss << std::setw(4) << std::this_thread::get_id();
		return oss.str();
	}
}
