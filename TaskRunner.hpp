#ifndef INCLUDED_MY_TASKRUNNER_HPP
#define INCLUDED_MY_TASKRUNNER_HPP

#include <mutex>
#include <future>
#include <queue>
#include <functional>
#include <condition_variable>

namespace my
{
	class TaskRunner final {
		const std::uint32_t thread_count_;
		std::mutex tasks_mtx_;
		std::atomic<bool> running_;
		std::queue<std::function<void()>> tasks_;
		std::unique_ptr<std::thread[]> threads_;
		std::condition_variable condition_;

	public:
		TaskRunner(const TaskRunner&) = delete;
		TaskRunner(TaskRunner&&) = delete;
		TaskRunner& operator=(const TaskRunner&) = delete;
		TaskRunner& operator=(TaskRunner&&) = delete;

		TaskRunner(const std::uint32_t threadCount = std::thread::hardware_concurrency());
		~TaskRunner();

	public:
		template <typename F, typename... Args>
		void addTask(F&& func, Args&&... args)
		{
			std::function<void()> f = std::bind(std::bind(std::forward<F>(func), std::forward<Args>(args)...));
			push_task(f);
		}

		template <typename F, typename... Args>
		std::future<std::result_of_t<F(Args...)>> addFutureTask(F&& func, Args&&... args)
		{
			using return_type = std::result_of_t<F(Args...)>;
			auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(func), std::forward<Args>(args)...));
			std::future<return_type> future = task->get_future();

			push_task([task]() { (*task)(); });
			return future;
		}

	private:
		template <typename F>
		void push_task(const F& task) {
			if (running_) {
				{
					const std::lock_guard<std::mutex> lock(tasks_mtx_);
					tasks_.push(std::function<void()>(task));
				}
				condition_.notify_one();
			}
		}

	private:
		void worker();
	};
}


#endif // INCLUDED_MY_TASKRUNNER_HPP
