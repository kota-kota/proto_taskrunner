#include "ProtoSharedContext.hpp"
#include "ProtoContext.hpp"
#include "ProtoClient.hpp"
#include "Utility.hpp"
#include <vector>
#include <thread>
#include <chrono>

using namespace my;

namespace {
	void test01()
	{
		{
			ProtoContext ctx1;
			ProtoContext ctx2;
			ProtoContext ctx3;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(7000));
		{
			ProtoContext ctx;
		}
	}
}

namespace
{
	std::vector<std::thread> threads(10);
	std::vector<std::thread> threads_mapcc(2);

	void test02()
	{
		for (auto& th : threads) {
			th = std::thread([] {
				ProtoContext ctx;
				ProtoClient client(ctx);
				PreFetchHandle handle = client.preFetch();
				Log("main", "future waiting.\n");
				std::future<PreFetchResult> future = handle.getFuture();
				PreFetchResult res = future.get();
				Log("main", "future finish res=%d.\n", res.getResult());
				});
		}

		for (auto& th : threads_mapcc) {
			th = std::thread([] {
				std::shared_ptr<ProtoSharedContext> ctx = ProtoSharedContext::getInstance();
				ctx->addContext("zero");
				ProtoDataCache* mapcc = ctx->getContext("zero");
				Log("main", "ProtoDataCache zero=0x%p.\n", mapcc);
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				});
		}

		for (auto& th : threads) {
			th.join();
		}
		for (auto& th : threads_mapcc) {
			th.join();
		}
	}
}

int main()
{
	Log("main", "start.\n");
	test01();
	test02();
	Log("main", "end.\n");
	return 0;
}
