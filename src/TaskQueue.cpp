/*
 Name:		TaskQueue.cpp
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	worker = std::thread([&]
		{
			while (!done)
			{
				Task task;
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

TaskQueue::~TaskQueue()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		done = true;
	}
	cv.notify_one();
	worker.join();
}

void TaskQueue::push(Task task)
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		m_tasks.emplace(std::move(task));
	}
	cv.notify_one();
}
