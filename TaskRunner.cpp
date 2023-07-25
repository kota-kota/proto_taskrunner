#include "TaskRunner.hpp"
#include "Utility.hpp"

namespace
{
	const char* log_tag_task = "TaskRunner";
}

namespace my
{
	TaskRunner::TaskRunner(const std::uint32_t threadCount) :
		thread_count_{ (threadCount != 0) ? threadCount : std::thread::hardware_concurrency() },
		tasks_mtx_{}, running_{ true }, tasks_{}, threads_{}, condition_{}
	{
		my::Log(log_tag_task, "%s() threadCount=%d.\n", __func__, thread_count_);
		threads_.reset(new std::thread[thread_count_]);
		for (std::uint32_t i = 0; i < thread_count_; i++) {
			threads_[i] = std::thread(&TaskRunner::worker, this);
		}
	}

	TaskRunner::~TaskRunner()
	{
		my::Log(log_tag_task, "%s().\n", __func__);
		running_ = false;

		// スレッドを終了させるために、全てのスレッドを起床させる
		condition_.notify_all();

		for (std::uint32_t i = 0; i < thread_count_; i++) {
			threads_[i].join();
		}
	}

	void TaskRunner::worker()
	{
		my::Log(log_tag_task, "%s() start.\n", __func__);
		for (;;) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(tasks_mtx_);
				condition_.wait(lock, [&] {return !tasks_.empty() || !running_; });

				if (!running_ && tasks_.empty()) {
					break;
				}

				task = std::move(tasks_.front());
				tasks_.pop();
			}

			task();
		}
		my::Log(log_tag_task, "%s() end.\n", __func__);
	}
}
