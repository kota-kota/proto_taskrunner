#include "ProtoClient.hpp"
#include "ProtoSharedContext.hpp"
#include "Utility.hpp"
#include <functional>
#include <chrono>

namespace
{
	const char* log_tag = "ProtoClient";
}

namespace my
{
	ProtoClient::ProtoClient(ProtoContext& ctx) :
		ctx_(ctx)
	{
		Log(log_tag, "%s.\n", __func__);
	}

	ProtoClient::~ProtoClient()
	{
		Log(log_tag, "%s.\n", __func__);
	}

	PreFetchHandle ProtoClient::preFetch()
	{
		Log(log_tag, "%s().\n", __func__);
		std::function<PreFetchResult(std::chrono::milliseconds)> task_ = [](std::chrono::milliseconds wait_time) {
			Log(log_tag, "\t\tpreFetch() task start.\n");
			std::this_thread::sleep_for(wait_time);
			Log(log_tag, "\t\tpreFetch() task end.\n");
			return PreFetchResult(PreFetchResult::Result::NG);
		};

		std::future<PreFetchResult> future = ctx_.shared_ctx_->preFetch(task_, std::chrono::milliseconds(5000));
		return PreFetchHandle(std::move(future));
	}
}
