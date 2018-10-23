#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future> 
#include <functional>
#include <stdexcept>



namespace K3D12 {
	class ThreadPool {
	private:

		std::vector< std::thread > _workers;

		std::queue< std::function<void()> > _tasks;

		std::mutex _queueMutex;

		std::condition_variable _condition;

		bool _stop;

	public:

	private:

	public:

		ThreadPool(size_t threads);

		~ThreadPool();

		//@fn
		//@brief		�L���[�Ƀ^�X�N�𓊂���B
		//@param (F)	�֐��I�u�W�F�N�g
		//@param (Args�j�֐��I�u�W�F�N�g���K�v�Ƃ������	
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
			->std::future<typename std::result_of<F(Args...)>::type>;

		void DiscardWorkers();

	};
}

inline K3D12::ThreadPool::ThreadPool(size_t threads)
	: _stop(false)
{
	for (size_t i = 0; i < threads; ++i)
		_workers.emplace_back([this]
	{
		for (;;)
		{
			std::function<void()> task;

			{
				std::unique_lock<std::mutex> lock(this->_queueMutex);
				this->_condition.wait(lock,
					[this] { return this->_stop || !this->_tasks.empty(); });
				if (this->_stop && this->_tasks.empty())
					return;
				task = std::move(this->_tasks.front());
				this->_tasks.pop();
			}

			task();
		}
	}
	);
}

inline K3D12::ThreadPool::~ThreadPool()
{
	DiscardWorkers();
}

template<class F, class... Args>
auto K3D12::ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(_queueMutex);

		if (_stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		_tasks.emplace([task]() { (*task)(); });
	}
	_condition.notify_one();
	return res;
}

inline void K3D12::ThreadPool::DiscardWorkers()
{	
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		_stop = true;
	}
	_condition.notify_all();
	for (std::thread &worker : _workers) {
		if (worker.joinable()) {
			worker.join();
		}
	}
}




