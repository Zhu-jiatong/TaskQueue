/*
 Name:		TaskQueue.h
 Created:	6/6/2023 10:32:21 PM
 Author:	jiaji
 Editor:	http://www.visualmicro.com
*/

#ifndef _TaskQueue_h
#define _TaskQueue_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <esp_pthread.h>

class TaskQueue
{
public:
	using Task = std::function<void()>;

	TaskQueue(size_t stackSizeBytes = esp_pthread_get_default_config().stack_size,
		std::function<void()> onQueueEmpty = nullptr);
	~TaskQueue();

	void push(Task task);

private:
	std::queue<Task> m_tasks;
	std::condition_variable m_cv;
	std::mutex m_mutex;
	std::thread m_worker;
	bool m_done = false;
	std::function<void()> m_onQueueEmpty;
};

#endif

