/*
 Name:		TaskQueue.cpp
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	m_worker = std::thread([&]
		{
			while (!m_done)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					m_cv.wait(lock, [&] {return !m_tasks.empty() || m_done; });
					if (m_done && m_tasks.empty()) return;
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
		std::lock_guard<std::mutex> lock(m_mutex);
		m_done = true;
	}
	m_cv.notify_one();
	m_worker.join();
}

void TaskQueue::push(Task task)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_tasks.emplace(std::move(task));
	}
	m_cv.notify_one();
}
