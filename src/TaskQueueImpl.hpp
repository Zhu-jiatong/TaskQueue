/*
 Name:		TaskQueue.cpp
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#include "TaskQueue.h"

template<typename TaskType>
TaskQueue<TaskType>::TaskQueue()
{
	worker = std::thread([&]
		{
			while (!done)
			{
				TaskType task;
				{
					std::unique_lock<std::mutex> lock(mutex);
					cv.wait(lock, [&] {return !m_tasks.empty() || done; });
					if (done && m_tasks.empty()) return;
					task = std::move(m_tasks.front());
					m_tasks.pop();
				}
				task();
			}
		});
}

template<typename TaskType>
TaskQueue<TaskType>::~TaskQueue()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		done = true;
	}
	cv.notify_one();
	worker.join();
}

template<typename TaskType>
void TaskQueue<TaskType>::push(TaskType task)
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		m_tasks.emplace(std::move(task));
	}
	cv.notify_one();
}
