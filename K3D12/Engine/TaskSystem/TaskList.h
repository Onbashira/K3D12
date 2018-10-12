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
		
		std::vector<std::function<void()>> _tasks;
		
	public:

	private:

	public:
		
		TaskList();
		
		~TaskList();
	};

}

