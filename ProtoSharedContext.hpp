#ifndef INCLUDED_MY_PROTOSHAREDCONTEXT_HPP
#define INCLUDED_MY_PROTOSHAREDCONTEXT_HPP

#include "TaskRunner.hpp"
#include <future>
#include <mutex>
#include <map>

namespace my
{
	class ProtoDataCache;
}



namespace my
{
	class ProtoSharedContext
	{
	private:
		class Impl {
		private:
			std::mutex mtx_;
			std::map<std::string, ProtoDataCache*> mapccMap_;
			std::unique_ptr<TaskRunner> taskRunner_;

		public:
			Impl();
			~Impl();

		public:
			void addContext(const std::string& tag);
			ProtoDataCache* getContext(const std::string& tag);
			void removeContext(const std::string& tag);

		public:
			void preFetchAsStartup();
			template <typename F, typename... Args>
			std::future<std::result_of_t<F(Args...)>> preFetch(F&& func, Args&&... args)
			{
				return taskRunner_->addFutureTask(func, args...);
			}
		};

		std::shared_ptr<Impl> impl_;

	public:
		static std::shared_ptr<ProtoSharedContext> getInstance();

	private:
		ProtoSharedContext();
		static void createInstance();
		static void destroyInstance();

	public:
		~ProtoSharedContext();
		void addContext(const std::string& tag);
		ProtoDataCache* getContext(const std::string& tag);
		void removeContext(const std::string& tag);

	public:
		template <typename F, typename... Args>
		std::future<std::result_of_t<F(Args...)>> preFetch(F&& func, Args&&... args)
		{
			return impl_->preFetch(func, args...);
		}
	};
}

#endif // INCLUDED_MY_PROTOSHAREDCONTEXT_HPP
