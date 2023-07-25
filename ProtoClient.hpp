#ifndef INCLUDED_MY_PROTOCLIENT_HPP
#define INCLUDED_MY_PROTOCLIENT_HPP

#include "ProtoContext.hpp"
#include <future>

namespace my
{
	class PreFetchResult {
	public:
		enum class Result { NG = -1, OK };

	private:
		Result result_;

	public:
		PreFetchResult(Result result) : result_(result) {}
		Result getResult() const { return result_; }
	};

	class PreFetchHandle {
		std::future<PreFetchResult> result_;

	public:
		PreFetchHandle(std::future<PreFetchResult> result) : result_(std::move(result)) {}
		std::future<PreFetchResult> getFuture()
		{
			return std::move(result_);
		}
	};

	class ProtoClient
	{
	private:
		ProtoContext& ctx_;

	public:
		ProtoClient(ProtoContext& ctx);
		~ProtoClient();

	public:
		PreFetchHandle preFetch();
	};
}

#endif // INCLUDED_MY_PROTOCLIENT_HPP
