/*
 Name:		TaskQueue.cpp
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t stackSizeBytes, std::function<void()> onQueueEmpty):
	m_onQueueEmpty(onQueueEmpty)
{
	esp_pthread_cfg_t threadConfig = esp_pthread_get_default_config();
	threadConfig.stack_size = stackSizeBytes;
	esp_pthread_set_cfg(&threadConfig);

	m_worker = std::thread(
		[&]
		{
			while (!m_done)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					m_cv.wait(lock, [&] {return !m_tasks.empty() || m_done; });
					if (m_done && m_tasks.empty())
						return;
					task = std::move(m_tasks.front());
					m_tasks.pop();
				}
				task();

				if (!m_onQueueEmpty)
					continue;
				std::unique_lock<std::mutex> lock(m_mutex);
				if (m_tasks.empty())
				{
					lock.unlock();
					m_onQueueEmpty();
				}
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
