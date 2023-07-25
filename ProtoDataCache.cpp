#include "ProtoDataCache.hpp"
#include "Utility.hpp"

namespace
{
	const char* log_tag = "ProtoDataCache";
}

namespace my
{
	ProtoDataCache::ProtoDataCache()
	{
		Log(log_tag, "%s().\n", __func__);
	}
	ProtoDataCache::~ProtoDataCache()
	{
		Log(log_tag, "%s() 0x%p.\n", __func__, this);
	}
}
