#include "ProtoSharedContext.hpp"
#include "ProtoDataCache.hpp"
#include "Utility.hpp"
#include <chrono>
#include <thread>

namespace
{
	const char* log_tag_impl = "ProtoSharedContext::Impl";
}

namespace my
{
	ProtoSharedContext::Impl::Impl() :
		mtx_(), mapccMap_(), taskRunner_()
	{
		Log(log_tag_impl, "%s().\n", __func__);
		taskRunner_ = std::make_unique<TaskRunner>();
		preFetchAsStartup();
	}

	ProtoSharedContext::Impl::~Impl()
	{
		Log(log_tag_impl, "%s().\n", __func__);
		auto itr = mapccMap_.begin();
		while (itr != mapccMap_.end()) {
			if (itr->second != nullptr) {
				delete itr->second;
			}
			itr = mapccMap_.erase(itr);
		}
	}

	void ProtoSharedContext::Impl::addContext(const std::string& tag)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		if (mapccMap_.find(tag) == mapccMap_.end()) {
			ProtoDataCache* mapcc = new (std::nothrow) ProtoDataCache;
			if (mapcc != nullptr) {
				mapccMap_[tag] = mapcc;
				Log(log_tag_impl, "%s() create ProtoDataCache %s=0x%p.\n", __func__, tag.c_str(), mapcc);
			}
			else {
				Log(log_tag_impl, "%s() failed to create ProtoDataCache.\n", __func__);
			}
		}
	}

	ProtoDataCache* ProtoSharedContext::Impl::getContext(const std::string& tag)
	{
		ProtoDataCache* mapcc = nullptr;
		std::lock_guard<std::mutex> lock(mtx_);
		if (mapccMap_.find(tag) != mapccMap_.end()) {
			mapcc = mapccMap_[tag];
		}
		return mapcc;
	}

	void ProtoSharedContext::Impl::removeContext(const std::string& tag)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		if (mapccMap_.find(tag) != mapccMap_.end()) {
			if (mapccMap_[tag] != nullptr) {
				delete mapccMap_[tag];
			}
			mapccMap_.erase(tag);
		}
	}

	void ProtoSharedContext::Impl::preFetchAsStartup()
	{
		Log(log_tag_impl, "%s().\n", __func__);

		std::function<void(std::chrono::milliseconds period, uint32_t count)> task_ = [](std::chrono::milliseconds period, uint32_t count) {
			Log(log_tag_impl, "\t\tpreFetchAsStartup() task start.\n");
			for (std::uint32_t c = 0; c < count; c++) {
				Log(log_tag_impl, "\t\tpreFetchAsStartup() count=%d.\n", c);
				std::this_thread::sleep_for(period);
			}
			Log(log_tag_impl, "\t\tpreFetchAsStartup() task end.\n");
		};

		taskRunner_->addTask(task_, std::chrono::milliseconds(1000), 5);
		taskRunner_->addTask(task_, std::chrono::milliseconds(1000), 3);
		taskRunner_->addTask(task_, std::chrono::milliseconds(1000), 2);
	}
}

namespace
{
	const char* log_tag = "ProtoSharedContext";
	std::once_flag g_init_flag;
	std::shared_ptr<my::ProtoSharedContext> g_instance = nullptr;
}

namespace my
{
	void ProtoSharedContext::createInstance()
	{
		Log(log_tag, "%s() IN SharedContext open=%d.\n", __func__, g_instance.use_count());
		g_instance = std::shared_ptr<ProtoSharedContext>(new (std::nothrow) ProtoSharedContext());
		std::atexit(ProtoSharedContext::destroyInstance);
		Log(log_tag, "%s() OUT SharedContext open=%d.\n", __func__, g_instance.use_count());
	}

	void ProtoSharedContext::destroyInstance()
	{
		Log(log_tag, "%s() SharedContext open=%d.\n", __func__, g_instance.use_count());
	}

	std::shared_ptr<ProtoSharedContext> ProtoSharedContext::getInstance()
	{
		Log(log_tag, "%s() IN SharedContext open=%d.\n", __func__, g_instance.use_count());
		std::call_once(g_init_flag, ProtoSharedContext::createInstance);
		Log(log_tag, "%s() OUT SharedContext open=%d.\n", __func__, g_instance.use_count());
		return g_instance;
	}

	ProtoSharedContext::ProtoSharedContext()
	{
		Log(log_tag, "%s().\n", __func__);
		impl_ = std::make_shared<Impl>();
	}

	ProtoSharedContext::~ProtoSharedContext()
	{
		Log(log_tag, "%s().\n", __func__);
	}

	void ProtoSharedContext::addContext(const std::string& tag)
	{
		impl_->addContext(tag);
	}
	ProtoDataCache* ProtoSharedContext::getContext(const std::string& tag)
	{
		return impl_->getContext(tag);
	}
	void ProtoSharedContext::removeContext(const std::string& tag)
	{
		impl_->removeContext(tag);
	}
}
