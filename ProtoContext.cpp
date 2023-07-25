#include "ProtoContext.hpp"
#include "ProtoSharedContext.hpp"
#include "Utility.hpp"

namespace
{
	const char* log_tag = "ProtoContext";
}

namespace my
{
	ProtoContext::ProtoContext()
	{
		Log(log_tag, "%s().\n", __func__);
		shared_ctx_ = ProtoSharedContext::getInstance();
		shared_ctx_->addContext("default");
		mapcc_ = shared_ctx_->getContext("default");
		Log(log_tag, "%s() SharedContext open=%d.\n", __func__, shared_ctx_.use_count());
		Log(log_tag, "%s() ProtoDataCache default=0x%p.\n", __func__, mapcc_);
	}

	ProtoContext::~ProtoContext()
	{
		Log(log_tag, "%s().\n", __func__);
		mapcc_ = nullptr;
		// ProtoSharedContext::removeContext()ÇÕÉRÅ[ÉãÇµÇ»Ç¢
	}
}
