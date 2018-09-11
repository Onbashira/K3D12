#pragma once


namespace K3D12::TaskSystem {

	class Task;
	class TaskManager
	{
	public:
		void ExcuteTaskList();
		TaskManager();
		~TaskManager();
	};

}

