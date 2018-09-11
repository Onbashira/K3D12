#pragma once
#include <vector>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <memory>
#include <type_traits>

namespace K3D12::TaskSystem {

	class TaskList
	{
	private:
		std::mutex _mutex;
		std::condition_variable _condition;
		std::vector<std::function<void()>> _tasks;
		
	public:
	private:
	public:
		template < class F,class... Args>
		auto ThrowTask(F&& f, Args&&... args)
			->std::future<typename std::result_of<F(Args...)>::type>;
		TaskList();
		~TaskList();
	};

	template<class F, class ...Args>
	inline auto K3D12::TaskSystem::TaskList::ThrowTask(F && f, Args && ...args) -> std::future<typename std::result_of<F(Args ...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));


		return std::future<typename std::result_of<F(Args ...)>::type>();
	}
}

